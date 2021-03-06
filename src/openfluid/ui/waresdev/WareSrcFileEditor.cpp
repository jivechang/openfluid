/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.

 */

/**
 @file WareSrcFileEditor.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/ui/waresdev/WareSrcFileEditor.hpp>

#include <QDir>
#include <QPainter>
#include <QTextStream>
#include <QMenu>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QToolTip>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/ui/waresdev/WareSrcSyntaxHighlighter.hpp>
#include <openfluid/tools/QtHelpers.hpp>


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareSrcFileEditor::WareSrcFileEditor(const QString& FilePath, QWidget* Parent) :
    QPlainTextEdit(Parent), m_FilePath(FilePath), mp_SyntaxHighlighter(0)
{
  m_SelectionTagsRegExp.setPattern("%%SEL_(START|END)%%");
  m_AllTagsRegExp.setPattern("%%(SEL_(START|END)|CURSOR|INDENT)%%");
  //TODO get "word part" characters from scanning xml completion strings ?
  m_WordPartRegExp.setPattern("\\w|:");

  // Line number management

  mp_LineNumberArea = new LineNumberArea(this);
  updateLineNumberAreaWidth(0);

  connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
  connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));


  // Syntax highlighting management

  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();

  if (PrefMgr->isCurrentlineHighlightingEnabled() && QColor::isValidColor(PrefMgr->getCurrentlineColor()))
  {
    m_LineColor.setNamedColor(PrefMgr->getCurrentlineColor());
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    highlightCurrentLine();
  }

  if (PrefMgr->isSyntaxHighlightingEnabled())
  {
    mp_SyntaxHighlighter = new WareSrcSyntaxHighlighter(
        document(), WareSrcFiletypeManager::instance()->getHighlightingRules(m_FilePath));
  }

  if (!PrefMgr->isLineWrappingEnabled())
  {
    QTextOption Option = document()->defaultTextOption();
    Option.setWrapMode(QTextOption::NoWrap);
    document()->setDefaultTextOption(Option);
  }


  // Font management

  QFont Font;
  Font.setFamily(PrefMgr->getFontName());
  Font.setFixedPitch(true);
  Font.setPointSize(10);
  setFont(Font);

  m_SpaceCharWidth = fontMetrics().width(' ');


  // Code insertion and code completion management

  m_CompletionRules = WareSrcFiletypeManager::instance()->getCompletionRules(m_FilePath);

  mp_SignalMapper = new QSignalMapper(this);
  mp_CompletionModel = new QStandardItemModel();

  int row = 0;
  for (WareSrcFiletypeManager::CompletionRules_t::iterator it = m_CompletionRules.begin();
      it != m_CompletionRules.end(); ++it)
  {
    QString Title = it->Title;
    QString Content = it->Content;

    // initialize actions for code insertion

    if (!Title.isEmpty())
    {
      QString MenuPath = it->MenuPath;
      QStringList MenuPathList = MenuPath.split("/");

      if (MenuPathList.isEmpty())
        MenuPathList << "Other";
      else
        for (int i=0;i<MenuPathList.size();i++)
          MenuPathList[i] = openfluid::tools::decodeXMLEntities(MenuPathList[i]);


      QString MainMenuTitle = MenuPathList.first();
      if (!m_InsertionMenus.contains(MainMenuTitle))
      {
        m_InsertionMenus[MainMenuTitle] = new QMenu(MainMenuTitle);
        m_InsertionMenus[MainMenuTitle]->setObjectName(MainMenuTitle);
      }

      QMenu* Menu = m_InsertionMenus[MainMenuTitle];

      for (int i = 1; i < MenuPathList.size(); i++)
      {
        QString SubPathName = MenuPath.section("/", 0, i);
        QMenu* SubMenu = Menu->findChild<QMenu*>(SubPathName);
        if (!SubMenu)
        {
          SubMenu = Menu->addMenu(MenuPathList[i]);
          SubMenu->setObjectName(SubPathName);
        }
        Menu = SubMenu;
      }

      QAction* a = new QAction(Title, this);
      mp_SignalMapper->setMapping(a, Content);
      connect(a, SIGNAL(triggered()), mp_SignalMapper, SLOT(map()));
      Menu->addAction(a);
    }

    // initialize model for code completion

    QString ContentWoTags = Content;
    ContentWoTags.remove(m_AllTagsRegExp);
    QStringList L = ContentWoTags.split("\n");
    QString ContentForList, ContentForDetail;

    if (L.size() > 1)
    {
      ContentForList = L[0].append("...");
      ContentForDetail = ContentWoTags;
    }
    else
    {
      ContentForList = ContentWoTags;
      ContentForDetail = "";
    }

    mp_CompletionModel->setItem(row, 0, new QStandardItem(ContentForList));
    // setItem(...QIcon()...) doesn't allow to resize icon
    QPixmap PM = QPixmap::fromImage(QImage(it->IconPath));
    mp_CompletionModel->setData(mp_CompletionModel->index(row, 0), PM.scaled(QSize(15, 15), Qt::KeepAspectRatio),
                                Qt::DecorationRole);
    mp_CompletionModel->setItem(row, 1, new QStandardItem(ContentForDetail));
    mp_CompletionModel->setItem(row, 2, new QStandardItem(Content));

    row++;
  }

  mp_CompletionModel->setSortRole(Qt::DisplayRole);
  mp_CompletionModel->sort(0);

  mp_Completer = new QCompleter(this);
  mp_Completer->setModel(mp_CompletionModel);
  mp_Completer->setCaseSensitivity(Qt::CaseInsensitive);
  mp_Completer->setWidget(this);
  mp_Completer->setCompletionMode(QCompleter::PopupCompletion);
  mp_Completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);

  connect(mp_Completer->popup()->selectionModel(), SIGNAL(currentRowChanged (const QModelIndex&, const QModelIndex&)),
          this, SLOT(onCompletionPopupCurrentRowChanged(const QModelIndex&, const QModelIndex&)));

  connect(mp_Completer, SIGNAL(activated(QString)), this, SLOT(insertCompletion()));

  connect(mp_SignalMapper, SIGNAL(mapped(QString)), this, SLOT(onInsertRequested(QString)));


  updateContent();

  connect(document(), SIGNAL(modificationChanged(bool)), this, SLOT(onChanged(bool)));
}


// =====================================================================
// =====================================================================


WareSrcFileEditor::~WareSrcFileEditor()
{
  delete mp_SyntaxHighlighter;
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::keyPressEvent(QKeyEvent* Event)
{
  if (mp_Completer->popup()->isVisible())
  {
    switch (Event->key())
    {
      case Qt::Key_Enter:
      case Qt::Key_Return:
      case Qt::Key_Escape:
      case Qt::Key_Tab:
      case Qt::Key_Backtab:
        Event->ignore();
        return;
      default:
        break;
    }
  }

  // ! 8239 == 0x202F == "narrow no-break space", may happen as a key with Ctrl modifier
  bool IsShortcut = ((Event->modifiers() & Qt::ControlModifier)
      && (Event->key() == Qt::Key_Space || Event->key() == 8239));

  if (!IsShortcut)
    QPlainTextEdit::keyPressEvent(Event);


  // to skip if only Ctrl or Shift alone
  const bool CtrlOrShift = Event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
  if (CtrlOrShift && Event->text().isEmpty())
    return;

  QTextCursor TestCursor = textCursor();
  int TestPosition = TestCursor.position();
  QString TestSelection;
  bool RemoveFirstChar = true;

  do
  {
    TestPosition--;
    if (TestPosition < 0)
    {
      RemoveFirstChar = false;
      break;
    }

    TestCursor.setPosition(TestPosition, QTextCursor::KeepAnchor);
    TestSelection = TestCursor.selectedText();
  }
  while (m_WordPartRegExp.exactMatch(TestSelection.left(1)));

  if (RemoveFirstChar)
    TestSelection.remove(0, 1);

  QString CompletionPrefix = TestSelection;

  if (IsShortcut || (mp_Completer->popup()->isVisible() && !CompletionPrefix.isEmpty()
                     && CompletionPrefix != mp_Completer->completionPrefix()))
  {
    mp_Completer->setCompletionPrefix(CompletionPrefix);

    QRect cr = cursorRect();
    cr.setWidth(
        mp_Completer->popup()->sizeHintForColumn(0) + mp_Completer->popup()->verticalScrollBar()->sizeHint().width());

    mp_Completer->complete(cr);

    // to do after the call of complete() for the signal "currentRowChanged" to be sent
    mp_Completer->popup()->setCurrentIndex(mp_Completer->completionModel()->index(0, 0));

    return;
  }

  mp_Completer->popup()->hide();
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::insertCompletion()
{
  QTextCursor Cursor = textCursor();

  Cursor.setPosition(Cursor.position() - mp_Completer->completionPrefix().size(), QTextCursor::KeepAnchor);
  Cursor.removeSelectedText();

  onInsertRequested(
      mp_Completer->completionModel()->data(
          mp_Completer->completionModel()->index(mp_Completer->popup()->currentIndex().row(), 2)).toString());
}


// =====================================================================
// =====================================================================


int WareSrcFileEditor::lineNumberAreaWidth()
{
  int Digits = 1;
  int Max = qMax(1, blockCount());
  while (Max >= 10)
  {
    Max /= 10;
    ++Digits;
  }

  int Space = 3 + fontMetrics().width(QLatin1Char('9')) * Digits;

  return Space;
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::updateLineNumberAreaWidth(int /*NewBlockCount*/)
{
  setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::updateLineNumberArea(const QRect& Rect, int dy)
{
  if (dy)
    mp_LineNumberArea->scroll(0, dy);
  else
    mp_LineNumberArea->update(0, Rect.y(), mp_LineNumberArea->width(), Rect.height());

  if (Rect.contains(viewport()->rect()))
    updateLineNumberAreaWidth(0);
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::resizeEvent(QResizeEvent* Event)
{
  QPlainTextEdit::resizeEvent(Event);

  QRect CR = contentsRect();
  mp_LineNumberArea->setGeometry(QRect(CR.left(), CR.top(), lineNumberAreaWidth(), CR.height()));
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::contextMenuEvent(QContextMenuEvent* Event)
{
  QMenu* Menu = createStandardContextMenu();
  Menu->addSeparator();

  foreach(QMenu* SubMenu,m_InsertionMenus){
  Menu->addMenu(SubMenu);
}

  Menu->exec(Event->globalPos());


  delete Menu;
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::onCompletionPopupCurrentRowChanged(const QModelIndex& Current, const QModelIndex& /*Previous*/)
{
  QString Str =
      mp_Completer->completionModel()->data(mp_Completer->completionModel()->index(Current.row(), 1)).toString();

  QToolTip::showText(mp_Completer->popup()->mapToGlobal(mp_Completer->popup()->rect().topRight()), Str);
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::onInsertRequested(const QString& Str)
{
  int InitialIndentInSpaceNb = cursorRect().x() / m_SpaceCharWidth;

  QStringList L = Str.split("%%CURSOR%%");
  if (L.size() > 1)
  {
    writeString(L[0], InitialIndentInSpaceNb);
    int Pos = textCursor().position();
    writeString(L[1], InitialIndentInSpaceNb);

    QTextCursor Cursor = textCursor();
    Cursor.setPosition(Pos);
    setTextCursor(Cursor);
    return;
  }

  L = Str.split(m_SelectionTagsRegExp);
  if (L.size() > 2)
  {
    writeString(L[0], InitialIndentInSpaceNb);
    int PosStart = textCursor().position();
    writeString(L[1], InitialIndentInSpaceNb);
    int PosEnd = textCursor().position();
    writeString(L[2], InitialIndentInSpaceNb);

    QTextCursor Cursor = textCursor();
    Cursor.setPosition(PosStart);
    Cursor.setPosition(PosEnd, QTextCursor::KeepAnchor);
    setTextCursor(Cursor);
    return;
  }

  writeString(Str, InitialIndentInSpaceNb);
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::writeString(const QString& Str, int InitialIndentInSpaceNb)
{
  QString ToInsert(Str);
  insertPlainText(ToInsert.replace("%%INDENT%%", QString(InitialIndentInSpaceNb, ' ')));
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::highlightCurrentLine()
{
  QList<QTextEdit::ExtraSelection> ExtraSelections;

  if (!isReadOnly())
  {
    QTextEdit::ExtraSelection Selection;

    Selection.format.setBackground(m_LineColor);
    Selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    Selection.cursor = textCursor();
    Selection.cursor.clearSelection();
    ExtraSelections.append(Selection);
  }

  setExtraSelections(ExtraSelections);
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::lineNumberAreaPaintEvent(QPaintEvent* Event)
{
  QPainter painter(mp_LineNumberArea);
  painter.fillRect(Event->rect(), Qt::lightGray);


  QTextBlock Block = firstVisibleBlock();
  int BlockNumber = Block.blockNumber();
  int Top = (int) blockBoundingGeometry(Block).translated(contentOffset()).top();
  int Bottom = Top + (int) blockBoundingRect(Block).height();

  while (Block.isValid() && Top <= Event->rect().bottom())
  {
    if (Block.isVisible() && Bottom >= Event->rect().top())
    {
      QString Number = QString::number(BlockNumber + 1);
      painter.setPen(Qt::black);
      painter.drawText(0, Top, mp_LineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, Number);
    }

    Block = Block.next();
    Top = Bottom;
    Bottom = Top + (int) blockBoundingRect(Block).height();
    ++BlockNumber;
  }
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::onChanged(bool Changed)
{
  emit editorTxtChanged(this, Changed);
}


// =====================================================================
// =====================================================================


QString WareSrcFileEditor::getFilePath()
{
  return m_FilePath;
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::saveContent()
{
  saveContentToPath(m_FilePath);

  document()->setModified(false);
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::saveContentToPath(const QString& Path)
{
  if (Path.isEmpty())
    return;

  QFile File(Path);

  if (!File.open(QIODevice::WriteOnly | QIODevice::Text))
    throw openfluid::base::FrameworkException("WareSrcFileEditor::setContentToPath",
                                              QString("Cannot open file %1 in write mode").arg(Path).toStdString());

  QTextStream Str(&File);
  Str << toPlainText();
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::updateContent()
{
  QFile File(m_FilePath);

  if (!File.open(QIODevice::ReadOnly | QIODevice::Text))
    throw openfluid::base::FrameworkException("WareSrcFileEditor::updateContent",
                                              QString("Cannot open file %1").arg(m_FilePath).toStdString());

  setPlainText(QTextStream(&File).readAll());

  document()->setModified(false);
}


// =====================================================================
// =====================================================================


} } }  // namespaces

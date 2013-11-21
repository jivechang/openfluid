/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file EditAttributeName.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "ui_EditAttributeNameDialog.h"
#include "EditAttributeNameDialog.hpp"

#include "builderconfig.hpp"

#include <QPushButton>


EditAttributeNameDialog::EditAttributeNameDialog(EditMode Mode,
                                                 const QStringList& AttrsList,
                                                 QWidget* Parent):
  OpenFLUIDDialog(Parent),ui(new Ui::EditAttributeNameDialog),
  m_Mode(Mode), m_AttrsNames(AttrsList)
{
  ui->setupUi(this);

  if (m_Mode == EDIT_ADD)
  {
    ui->NewNameLabel->setText(tr("Attribute name:"));
    ui->DefaultValueLabel->setText(tr("Default value:"));
    m_DefaultMsg = tr("Add new attribute");
    connect(ui->NewNameEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
    connect(ui->DefaultValueEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
  }
  else if (m_Mode == EDIT_RENAME)
  {
    ui->OriginalNameLabel->setText(tr("Attribute to rename:"));
    ui->OriginalNameComboBox->addItems(AttrsList);
    ui->NewNameLabel->setText(tr("New attribute name:"));
    m_DefaultMsg = tr("Rename attribute");
    connect(ui->OriginalNameComboBox,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(checkGlobal()));
    connect(ui->NewNameEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
  }
  else if (m_Mode == EDIT_REMOVE)
  {
    ui->OriginalNameLabel->setText(tr("Attribute to remove:"));
    ui->OriginalNameComboBox->addItems(AttrsList);
    m_DefaultMsg = tr("Remove attribute");
  }

  ui->OriginalNameWidget->setVisible(m_Mode == EDIT_REMOVE ||
                                     m_Mode == EDIT_RENAME);

  ui->NewNameWidget->setVisible(m_Mode == EDIT_RENAME ||
                                m_Mode == EDIT_ADD);

  ui->DefaultValueWidget->setVisible(m_Mode == EDIT_ADD);

  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));

  adjustSize();

  checkGlobal();
}


// =====================================================================
// =====================================================================


EditAttributeNameDialog::~EditAttributeNameDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void EditAttributeNameDialog::checkGlobal()
{
  if (m_Mode == EDIT_ADD)
  {
    if (m_AttrsNames.contains(ui->NewNameEdit->text()))
      setMessage(tr("New attribute name already exists"));
    else if (ui->NewNameEdit->text().isEmpty())
      setMessage(tr("New attribute name is empty"));
    else if (ui->DefaultValueEdit->text().isEmpty())
      setMessage(tr("Default attribute value is empty"));
    else
      setMessage();
  }
  else if (m_Mode == EDIT_RENAME)
  {
    if (m_AttrsNames.contains(ui->NewNameEdit->text()))
      setMessage(tr("New attribute name already exists"));
    else if (ui->NewNameEdit->text().isEmpty())
      setMessage(tr("New attribute name is empty"));
    else
      setMessage();
  }
  else if (m_Mode == EDIT_REMOVE)
    setMessage();
}


// =====================================================================
// =====================================================================


void EditAttributeNameDialog::setMessage(const QString& Msg)
{
  if (Msg.isEmpty())
  {
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                    .arg(BUILDER_DIALOGBANNER_BGCOLOR));
    ui->MessageLabel->setText(m_DefaultMsg);
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
  }
  else
  {
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                    .arg(BUILDER_DIALOGBANNER_WARNBGCOLOR));
    ui->MessageLabel->setText(Msg);
  }
}


// =====================================================================
// =====================================================================


QString EditAttributeNameDialog::getSelectedOriginalName() const
{
  return ui->OriginalNameComboBox->currentText();
}


// =====================================================================
// =====================================================================


QString EditAttributeNameDialog::getNewName() const
{
  return ui->NewNameEdit->text();
}


// =====================================================================
// =====================================================================


QString EditAttributeNameDialog::getDefaultValue() const
{
  return ui->DefaultValueEdit->text();
}

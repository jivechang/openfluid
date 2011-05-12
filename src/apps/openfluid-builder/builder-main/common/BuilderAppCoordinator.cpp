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
 \file BuilderAppCoordinator.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "BuilderAppCoordinator.hpp"

#include <openfluid/guicommon/DialogBoxFactory.hpp>

#include "BuilderAppWindow.hpp"
#include "BuilderModuleFactory.hpp"
#include "BuilderAppHomeState.hpp"
#include "BuilderAppProjectState.hpp"
#include "PreferencesModel.hpp"

#include "BuilderProjectWithExplorer.hpp"

#include "openfluid/base/ProjectManager.hpp"
#include "EngineProjectNewDialog.hpp"
#include "EngineProjectOpenDialog.hpp"

#include <openfluid/guicommon/PreferencesManager.hpp>
#include "BuilderHomeModule.hpp"

#include "builderconfig.hpp"

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::unsetCurrentModule()
{
  m_MainWindow.unsetCurrentModuleWidget();
  delete mp_CurrentModule;
}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::setCurrentModule(BuilderModule* Module)
{
  unsetCurrentModule();
  mp_CurrentModule = Module;
  mp_CurrentModule->initialize();
  m_MainWindow.setCurrentModuleWidget(
      *mp_CurrentModule->composeAndGetAsWidget());
}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::whenNewProjectAsked()
{
  mp_CurrentState->whenNewProjectAsked();
}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::whenOpenProjectAsked()
{
  mp_CurrentState->whenOpenProjectAsked();
}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::whenCloseProjectAsked()
{
  mp_CurrentState->whenCloseProjectAsked();
}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::whenQuitAsked()
{
  mp_CurrentState->whenQuitAsked();
}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::whenRunAsked()
{
  mp_CurrentState->whenRunAsked();
}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::whenMarketAsked()
{
  mp_CurrentState->whenMarketAsked();
}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::whenPreferencesAsked()
{
  mp_CurrentState->whenPreferencesAsked();
}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::whenAboutAsked()
{
  mp_CurrentState->whenAboutAsked();
}


// =====================================================================
// =====================================================================


void BuilderAppCoordinator::whenOnlineAsked(const std::string& URL)
{
  mp_CurrentState->whenOnlineAsked(URL);
}


// =====================================================================
// =====================================================================


void BuilderAppCoordinator::whenSaveAsked()
{
  mp_CurrentState->whenSaveAsked();
}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::whenMapViewAsked()
{
  mp_CurrentState->whenMapViewAsked();
}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::whenSaveAsAsked()
{
  mp_CurrentState->whenSaveAsAsked();
}

// =====================================================================
// =====================================================================


BuilderAppCoordinator::BuilderAppCoordinator(BuilderAppWindow& MainWindow,
    BuilderAppActions& Actions, PreferencesModel& PrefModel) :
  m_MainWindow(MainWindow), m_Actions(Actions), m_PreferencesModel(PrefModel),
      m_HasToBeSaved(true)
{
  mp_CurrentModule = 0;

  mp_HomeState = new BuilderAppHomeState(*this);
  mp_ProjectState = new BuilderAppProjectState(*this);

  mp_NewProjectDialog = new EngineProjectNewDialog();
  mp_OpenProjectDialog = new EngineProjectOpenDialog();

  m_Actions.getFileNewAction()->signal_activate().connect(sigc::mem_fun(*this,
      &BuilderAppCoordinator::whenNewProjectAsked));
  m_Actions.getFileOpenAction()->signal_activate().connect(sigc::mem_fun(*this,
      &BuilderAppCoordinator::whenOpenProjectAsked));
  m_Actions.getFileCloseAction()->signal_activate().connect(sigc::mem_fun(
      *this, &BuilderAppCoordinator::whenCloseProjectAsked));
  m_Actions.getFileQuitAction()->signal_activate().connect(sigc::mem_fun(*this,
      &BuilderAppCoordinator::whenQuitAsked));

  m_Actions.getSimulationRunAction()->signal_activate().connect(sigc::mem_fun(
      *this, &BuilderAppCoordinator::whenRunAsked));

  m_Actions.getAppMarketAction()->signal_activate().connect(sigc::mem_fun(
      *this, &BuilderAppCoordinator::whenMarketAsked));

  m_Actions.getEditPreferencesAction()->signal_activate().connect(
      sigc::mem_fun(*this, &BuilderAppCoordinator::whenPreferencesAsked));

  m_Actions.getAppAboutAction()->signal_activate().connect(sigc::mem_fun(*this,
      &BuilderAppCoordinator::whenAboutAsked));

  m_Actions.getAppOnlineWebsiteAction()->signal_activate().connect(
      sigc::bind<std::string>(sigc::mem_fun(*this,&BuilderAppCoordinator::whenOnlineAsked),BUILDER_URL_WEBSITE));

  m_Actions.getAppOnlineCommunityAction()->signal_activate().connect(
      sigc::bind<std::string>(sigc::mem_fun(*this,&BuilderAppCoordinator::whenOnlineAsked),BUILDER_URL_COMMUNITY));

  m_Actions.getAppOnlineDevAction()->signal_activate().connect(
      sigc::bind<std::string>(sigc::mem_fun(*this,&BuilderAppCoordinator::whenOnlineAsked),BUILDER_URL_DEV));

  m_Actions.getAppOnlineBugAction()->signal_activate().connect(
      sigc::bind<std::string>(sigc::mem_fun(*this,&BuilderAppCoordinator::whenOnlineAsked),BUILDER_URL_BUG));

  m_Actions.getSaveAction()->signal_activate().connect(sigc::mem_fun(*this,
      &BuilderAppCoordinator::whenSaveAsked));

  m_Actions.getSaveAsAction()->signal_activate().connect(sigc::mem_fun(*this,
      &BuilderAppCoordinator::whenSaveAsAsked));

  m_Actions.getMapViewAction()->signal_activate().connect(sigc::mem_fun(*this,
      &BuilderAppCoordinator::whenMapViewAsked));

  setState(*mp_HomeState);
}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::setState(BuilderAppState& State)
{
  mp_CurrentState = &State;
}

// =====================================================================
// =====================================================================


BuilderAppState* BuilderAppCoordinator::getHomeState()
{
  return mp_HomeState;
}

BuilderAppState* BuilderAppCoordinator::getProjectState()
{
  return mp_ProjectState;
}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::setHomeModule()
{
  BuilderModule* HomeModule = BuilderModuleFactory::createHomeModule(m_Actions);
  setCurrentModule(HomeModule);
  ((BuilderHomeModule*) HomeModule)->signal_OpenProjectAsked().connect(
      sigc::mem_fun(*this, &BuilderAppCoordinator::openProject));
  m_Actions.setProjectActionGroupVisible(false);
  m_MainWindow.setToolBarVisible(false);
  m_MainWindow.setStatusBarVisible(false);
  m_MainWindow.set_title("OpenFLUID-Builder");
}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::setProjectModule(std::string ProjectFolder)
{
  if (openfluid::base::ProjectManager::getInstance()->isOpened())
  {
    BuilderProjectWithExplorer* ProjectModule = new BuilderProjectWithExplorer(
        ProjectFolder);

    setCurrentModule(ProjectModule);

    ProjectModule->signal_CheckHappened().connect(sigc::mem_fun(*this,
        &BuilderAppCoordinator::onCheckHappened));

    ProjectModule->signal_ChangeHappened().connect(sigc::mem_fun(*this,
        &BuilderAppCoordinator::onChangeHappened));

    ProjectModule->signal_SaveHappened().connect(sigc::mem_fun(*this,
        &BuilderAppCoordinator::onSaveHappened));

    ProjectModule->checkAsked();

    ProjectModule->saveAsked();

    m_Actions.setProjectActionGroupVisible(true);
    m_MainWindow.setToolBarVisible(true);
    m_MainWindow.setStatusBarVisible(true);
    m_MainWindow.set_title("OpenFLUID-Builder  ["
        + openfluid::base::ProjectManager::getInstance()->getName() + "]");

    std::string CurrentPrjStr =
        openfluid::base::ProjectManager::getInstance()->getPath();

    if (CurrentPrjStr.empty())
      CurrentPrjStr = std::string("<span color='red'>") + _("(unsaved!)")
          + std::string("</span>");
    m_MainWindow.setStatusBarMessage("Current project path: " + CurrentPrjStr);

    openfluid::guicommon::PreferencesManager::getInstance()->addRecentProject(
        openfluid::base::ProjectManager::getInstance()->getPath(),
        openfluid::base::ProjectManager::getInstance()->getName());
    openfluid::guicommon::PreferencesManager::getInstance()->save();

  } else
  {
    throw openfluid::base::OFException("OpenFLUID Builder",
        "BuilderAppCoordinator::setProjectModule",
        "ProjectModule needs an opened Project Manager");
  }

}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::onCheckHappened(bool IsCheckOk)
{
  m_Actions.getSimulationRunAction()->set_sensitive(IsCheckOk);
}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::onChangeHappened()
{
  m_Actions.getSaveAction()->set_sensitive(true);
  m_HasToBeSaved = true;
}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::onSaveHappened()
{
  m_Actions.getSaveAction()->set_sensitive(false);
  m_HasToBeSaved = false;
}

// =====================================================================
// =====================================================================


BuilderModule* BuilderAppCoordinator::getCurrentModule()
{
  return mp_CurrentModule;
}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::quitApp()
{
  unsetCurrentModule();
  m_MainWindow.hide();
}

// =====================================================================
// =====================================================================


bool BuilderAppCoordinator::showCloseProjectDialog()
{
  int Res = openfluid::guicommon::DialogBoxFactory::showCloseProjectDialog(
      m_HasToBeSaved);

  if (Res == 0)
    return false;

  if (Res == 1)
    return true;

  whenSaveAsked();
  return true;
}

// =====================================================================
// =====================================================================


bool BuilderAppCoordinator::showQuitAppDialog()
{
  return openfluid::guicommon::DialogBoxFactory::showSimpleOkCancelQuestionDialog(
      _("Are you sure you want to quit ?"));
}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::showPreferencesDialog()
{
  m_PreferencesModel.showAsked();
}

// =====================================================================
// =====================================================================

void BuilderAppCoordinator::openProject(std::string ProjectPath)
{
  std::string ProjectFolder;

  if (ProjectPath == "")
    ProjectFolder = mp_OpenProjectDialog->show();
  else
    ProjectFolder = ProjectPath;

  if (ProjectFolder.empty())
    return;

  if (!openfluid::base::ProjectManager::getInstance()->isProject(ProjectFolder))
  {
    std::cerr << ProjectPath << " is not an OpenFLUID project" << std::endl;
    return;
  }

  openfluid::base::ProjectManager::getInstance()->open(ProjectFolder);

  setProjectModule(ProjectFolder);

  setState(*getProjectState());

}

// =====================================================================
// =====================================================================

void BuilderAppCoordinator::createProject()
{
  std::string ProjectFolder = mp_NewProjectDialog->show();

  if (ProjectFolder.empty())
    return;

  openfluid::base::ProjectManager::getInstance()->open(ProjectFolder);

  if (mp_NewProjectDialog->getImportDir().empty())
    setProjectModule("");
  else
    setProjectModule(ProjectFolder);

  setState(*getProjectState());

}

// =====================================================================
// =====================================================================


void BuilderAppCoordinator::closeProject()
{
  openfluid::base::ProjectManager::getInstance()->close();
  setHomeModule();
  setState(*getHomeState());
}

// =====================================================================
// =====================================================================


BuilderAppCoordinator::~BuilderAppCoordinator()
{
  delete mp_NewProjectDialog;
}

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
  \file PrimitivesProdFunc.cpp
  \brief Implements ...
*/


#include "MessagesFunc.h"


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(MessagesFunction)


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.messages"));
  DECLARE_SIGNATURE_NAME(("test function for messages"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));

END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


MessagesFunction::MessagesFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


MessagesFunction::~MessagesFunction()
{


}


// =====================================================================
// =====================================================================


bool MessagesFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{

  m_RepeatMessages = 1;

  OPENFLUID_GetFunctionParameter(Params,"rptmsgs",&m_RepeatMessages);

  return true;
}

// =====================================================================
// =====================================================================


bool MessagesFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool MessagesFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool MessagesFunction::initializeRun(const openfluid::base::SimulationInfo* /*SimInfo*/)
{

  OPENFLUID_RaiseWarning("tests.messages","initializeRun()","Message from tests.messages function");

  return true;
}

// =====================================================================
// =====================================================================


bool MessagesFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{
  openfluid::core::Unit* TU;

  int i = 0;

  std::string RptStr, IDStr, TSStr;

  openfluid::tools::ConvertValue(SimStatus->getCurrentStep(),&TSStr);

  DECLARE_UNITS_ORDERED_LOOP(1);

  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)

    openfluid::tools::ConvertValue(TU->getID(),&IDStr);

    for (i = 0; i< m_RepeatMessages; i++)
    {
      openfluid::tools::ConvertValue((i+1),&RptStr);
      OPENFLUID_RaiseWarning("tests.messages","runStep()",SimStatus->getCurrentStep(),"["+TSStr+"|"+IDStr+"|"+RptStr+"] Message from tests.messages function");
    }

  END_LOOP


  return true;
}

// =====================================================================
// =====================================================================


bool MessagesFunction::finalizeRun(const openfluid::base::SimulationInfo* /*SimInfo*/)
{

  OPENFLUID_RaiseWarning("tests.messages","finalizeRun()","Message from tests.messages function");

  return true;
}

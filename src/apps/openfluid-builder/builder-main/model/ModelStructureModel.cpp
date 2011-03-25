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
 \file ModelStructureModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ModelStructureModel.hpp"


#include "ModelItemInstanceFactory.hpp"

bool ModelStructureModelImpl::isModelInstance()
{
  if (!mp_ModelInstance)
  {
    throw openfluid::base::OFException("OpenFLUID Builder",
        "ModelStructureModel::isModelInstance",
        "no Model Instance. Operation is impossible.");
    return false;
  }
  return true;
}
bool ModelStructureModelImpl::areMoveIndexesValid(unsigned int From,
    unsigned int To)
{
  unsigned int ModelListSize = getFctCount();
  if (!(From < ModelListSize && To < ModelListSize))
  {
    throw openfluid::base::OFException("OpenFLUID Builder",
        "ModelStructureModel::areMoveIndexesValid",
        "Bad indexes of items to move");
    return false;
  }
  if (From == To)
    return false;
  return true;
}
bool ModelStructureModelImpl::isModelEmpty()
{
  return getFctCount() == 0;
}
int ModelStructureModelImpl::getLastPosition()
{
  return isModelEmpty() ? -1 : getFctCount() - 1;
}

ModelStructureModelImpl::ModelStructureModelImpl()
{
  mp_ModelInstance = 0;
  m_CurrentSelection = -1;
  m_AppRequestedSelection = -1;
}
sigc::signal<void> ModelStructureModelImpl::signal_FromUserSelectionChanged()
{
  return m_signal_FromUserSelectionChanged;
}
sigc::signal<void> ModelStructureModelImpl::signal_FromAppModelChanged()
{
  return m_signal_FromAppModelChanged;
}
sigc::signal<void> ModelStructureModelImpl::signal_FromAppSelectionRequested()
{
  return m_signal_FromAppSelectionRequested;
}
void ModelStructureModelImpl::setEngineRequirements(
    openfluid::machine::ModelInstance& ModelInstance)
{
  mp_ModelInstance = &ModelInstance;
  signal_FromAppModelChanged().emit();
  requestSelectionByAppAt(getLastPosition());
}
openfluid::machine::ModelInstance* ModelStructureModelImpl::getModelInstance()
{
  if (isModelInstance())
    return mp_ModelInstance;
  return 0;
}
void ModelStructureModelImpl::appendFunction(
    openfluid::machine::SignatureItemInstance& Signature)
{
  if (isModelInstance())
  {
    mp_ModelInstance->appendItem(
        ModelItemInstanceFactory::createModelItemInstanceFromSignature(
            Signature));

    signal_FromAppModelChanged().emit();
    requestSelectionByAppAt(getLastPosition());
  }
}
void ModelStructureModelImpl::moveFunction(unsigned int From, unsigned int To)
{
  if (isModelInstance() && areMoveIndexesValid(From, To))
  {
    std::list<openfluid::machine::ModelItemInstance*>::const_iterator itFrom;
    itFrom = mp_ModelInstance->getItems().begin();
    std::advance(itFrom, From);
    openfluid::machine::ModelItemInstance* Item = *itFrom;

    mp_ModelInstance->deleteItem(From);

    if (To == getFctCount())
      mp_ModelInstance->appendItem(Item);
    else
      mp_ModelInstance->insertItem(Item, To);

    signal_FromAppModelChanged().emit();
    requestSelectionByAppAt(To);
  }
}
void ModelStructureModelImpl::moveTowardTheBegin()
{
  if (getCurrentSelection() > -1)
  {
    int From = getCurrentSelection();
    int To = From == 0 ? getLastPosition() : From - 1;
    moveFunction(From, To);
  }
}
void ModelStructureModelImpl::moveTowardTheEnd()
{
  if (getCurrentSelection() > -1)
  {
    int From = getCurrentSelection();
    int To = From == getLastPosition() ? 0 : From + 1;
    moveFunction(From, To);
  }
}
void ModelStructureModelImpl::removeFunctionAt(int Position)
{
  if (isModelInstance() && Position > -1)
  {
    mp_ModelInstance->deleteItem(Position);
    signal_FromAppModelChanged().emit();
    if (isModelEmpty())
      requestSelectionByAppAt(-1);
    else if (Position == (int) getFctCount())
      requestSelectionByAppAt(Position - 1);
    else
      requestSelectionByAppAt(Position);
  }
}
void ModelStructureModelImpl::setCurrentSelectionByUserAt(int Position)
{
  m_CurrentSelection = Position;
  m_signal_FromUserSelectionChanged.emit();
}
int ModelStructureModelImpl::getCurrentSelection()
{
  return m_CurrentSelection;
}
unsigned int ModelStructureModelImpl::getFctCount()
{
  if (mp_ModelInstance)
    return mp_ModelInstance->getItemsCount();
  return 0;
}
void ModelStructureModelImpl::requestSelectionByAppAt(int Position)
{
  if (Position != m_CurrentSelection)
  {
    m_AppRequestedSelection = Position;
    m_signal_FromAppSelectionRequested.emit();
  }
}
int ModelStructureModelImpl::getAppRequestedSelection()
{
  return m_AppRequestedSelection;
}
/**
  \file HayamiSUPlug.h
  \brief implementation of HayamiSUPlug

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "HayamiSUFunc.h"
#include "HayamiSUPlug.h"


mhydasdk::base::Plugin* GetMHYDASPlugin()
{
  return new HayamiSUPlug();
}


// =====================================================================
// =====================================================================


HayamiSUPlug::HayamiSUPlug()
          : mhydasdk::base::Plugin()
{
  mp_Signature = new mhydasdk::base::Signature();

  mp_Signature->Author = wxT("Jean-Christophe FABRE");
  mp_Signature->AuthorEmail = wxT("fabrejc@ensam.inra.fr");
  mp_Signature->ID = wxT("hayamisu");
  mp_Signature->FunctionType = mhydasdk::base::SIMULATION;
  mp_Signature->Name = wxT("");
  mp_Signature->Description = wxT("");

}


// =====================================================================
// =====================================================================


HayamiSUPlug::~HayamiSUPlug()
{

}


// =====================================================================
// =====================================================================


mhydasdk::base::Function* HayamiSUPlug::getFunction(mhydasdk::core::CoreRepository* CoreData)
{
  return new HayamiSUFunction(CoreData);
}



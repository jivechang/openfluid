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
  @file SVGFileGeneratorWorker.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#include <openfluid/tools/ExternalProgram.hpp>

#include "SVGFileGeneratorWorker.hpp"

#include <QTextStream>
#include <QProcess>
#include <QFile>



SVGFileGeneratorWorker::SVGFileGeneratorWorker(const QString& GVFileName, const QString& SVGFileName,
                                             openfluid::fluidx::AdvancedDomainDescriptor* AdvDomainDesc):
 m_GVFileName(GVFileName),m_SVGFileName(SVGFileName),
 mp_AdvDomainDesc(AdvDomainDesc)
{
  m_Options["rankdir"] = "TB";
}


// =====================================================================
// =====================================================================


SVGFileGeneratorWorker::~SVGFileGeneratorWorker()
{

}


// =====================================================================
// =====================================================================


void SVGFileGeneratorWorker::setOption(const QString& Name, const QString& Value)
{
  m_Options[Name] = Value;
}

// =====================================================================
// =====================================================================


void SVGFileGeneratorWorker::run()
{
  QFile::remove(m_GVFileName);
  QFile::remove(m_SVGFileName);

  QFile GVFile(m_GVFileName);
  if (!GVFile.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    emit finished();
    return;
  }

  QTextStream GVOut(&GVFile);

  GVOut << "digraph {\n";


  // options

  QMapIterator<QString, QString> itO(m_Options);
  while (itO.hasNext()) {
      itO.next();
      GVOut << itO.key() << " = \"" << itO.value() << "\"\n";
  }

  // spatial units
  std::set<std::string> ClassesNames = mp_AdvDomainDesc->getClassNames();

  std::set<std::string>::const_iterator itC;
  std::set<std::string>::const_iterator itCb = ClassesNames.begin();
  std::set<std::string>::const_iterator itCe = ClassesNames.end();

  for (itC=itCb;itC!= itCe;++itC)
  {
    std::set<int> UnitsIDs = mp_AdvDomainDesc->getIDsOfClass(*itC);

    std::set<int>::const_iterator itI;
    std::set<int>::const_iterator itIb = UnitsIDs.begin();
    std::set<int>::const_iterator itIe = UnitsIDs.end();

    for (itI=itIb;itI!= itIe;++itI)
      GVOut << "\"" << QString::fromStdString(*itC) << "#" << (*itI) << "\"\n" ;

  }


  // connections

  for (itC=itCb;itC!= itCe;++itC)
  {
    std::set<int> UnitsIDs = mp_AdvDomainDesc->getIDsOfClass(*itC);

    std::set<int>::const_iterator itI;
    std::set<int>::const_iterator itIb = UnitsIDs.begin();
    std::set<int>::const_iterator itIe = UnitsIDs.end();

    // To connections
    for (itI=itIb;itI!= itIe;++itI)
    {
      const std::list<openfluid::core::UnitClassID_t>& ToUnits =
          mp_AdvDomainDesc->toSpatialUnits(std::make_pair<std::string,int>(*itC,*itI));

      std::list<openfluid::core::UnitClassID_t>::const_iterator itT;
      std::list<openfluid::core::UnitClassID_t>::const_iterator itTb = ToUnits.begin();
      std::list<openfluid::core::UnitClassID_t>::const_iterator itTe = ToUnits.end();

      for (itT=itTb;itT!= itTe;++itT)
      {
        GVOut << "\"" << QString::fromStdString(*itC) << "#" << (*itI) << "\""
              << " -> "
              << "\"" << QString::fromStdString((*itT).first) << "#" << ((*itT).second) << "\"\n";
      }
    }


    // Parent connections

    for (itI=itIb;itI!= itIe;++itI)
    {
      const std::list<openfluid::core::UnitClassID_t>& ParentUnits =
          mp_AdvDomainDesc->parentSpatialUnits(std::make_pair<std::string,int>(*itC,*itI));

      std::list<openfluid::core::UnitClassID_t>::const_iterator itP;
      std::list<openfluid::core::UnitClassID_t>::const_iterator itPb = ParentUnits.begin();
      std::list<openfluid::core::UnitClassID_t>::const_iterator itPe = ParentUnits.end();

      for (itP=itPb;itP!= itPe;++itP)
      {
        GVOut << "\"" << QString::fromStdString((*itP).first) << "#" << ((*itP).second) << "\""
              << " -> "
              << "\"" << QString::fromStdString(*itC) << "#" << (*itI) << "\" [arrowhead=odiamond,color=gray50,style=dashed]\n";
      }
    }


  }

  GVOut << "}\n";

  GVFile.close();


  // Generation of SVG file

#if defined(Q_OS_WIN32)
  openfluid::tools::ExternalProgram DotProgram("dot.exe");
#else
  openfluid::tools::ExternalProgram DotProgram("dot");
#endif

  if (DotProgram.isFound())
  {

    QString DotCommand = QString("\"%1\" -Tsvg \"%2\" -o \"%3\"")
                         .arg(DotProgram.getFullProgramPath())
                         .arg(m_GVFileName)
                         .arg(m_SVGFileName);

    QProcess::execute(DotCommand);
  }


  emit finished();
}

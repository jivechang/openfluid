

CONFIGURE_FILE(builderconfig.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/builderconfig.hpp" @ONLY)
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")


INCLUDE_DIRECTORIES(base common datastore model monitoring spatial outputs runconfig extensions)
  

### BASE #

SET(BUILDER_BASE_CPP base/AppCoordinator.cpp base/AppActions.cpp
                     base/BuilderApp.cpp base/MainWindow.cpp 
                     base/HomeModule.cpp base/HomeWidget.cpp                       
                     base/ProjectModule.cpp base/ProjectWidget.cpp                     
                     base/ProjectCentral.cpp base/ProjectCheckInfos.cpp
                     base/NewProjectDialog.cpp
                     base/OpenExampleProjectDialog.cpp
                     base/StatusMessagesWidget.cpp
                     base/DashboardFrame.cpp 
                     base/DashboardWidget.cpp base/DashboardInfosWidget.cpp base/DashboardStatusWidget.cpp
                     base/SaveAsDialog.cpp
                     base/NewslineWidget.cpp base/NewsItemWidget.cpp base/NewslineDownloadWorker.cpp
                     base/EditProjectPropertiesDialog.cpp
                     )
                     
SET(BUILDER_BASE_HPP base/AppCoordinator.hpp base/AppActions.hpp
                     base/BuilderApp.hpp base/MainWindow.hpp
                     base/HomeModule.hpp base/HomeWidget.hpp
                     base/ProjectModule.hpp base/ProjectWidget.hpp                     
                     base/ProjectCentral.hpp base/ProjectCheckInfos.hpp
                     base/NewProjectDialog.hpp
                     base/OpenExampleProjectDialog.hpp
                     base/StatusMessagesWidget.hpp
                     base/DashboardFrame.hpp 
                     base/DashboardWidget.hpp base/DashboardInfosWidget.hpp base/DashboardStatusWidget.hpp
                     base/SaveAsDialog.hpp
                     base/NewslineWidget.hpp base/NewsItemWidget.hpp base/NewslineDownloadWorker.hpp
                     base/EditProjectPropertiesDialog.hpp         
                     )

QT4_WRAP_UI(BUILDER_BASE_UI base/HomeWidget.ui base/ProjectWidget.ui
                            base/NewProjectDialog.ui
                            base/OpenExampleProjectDialog.ui
                            base/StatusMessagesWidget.ui
                            base/DashboardInfosWidget.ui base/DashboardStatusWidget.ui
                            base/SaveAsDialog.ui
                            base/NewslineWidget.ui base/NewsItemWidget.ui
                            base/EditProjectPropertiesDialog.ui
                            )



### COMMON #

SET(BUILDER_COMMON_CPP common/AppTools.cpp                       
                       common/WaresManagementWidget.cpp common/WareWidget.cpp common/ClickableWareWidget.cpp 
                       common/SignatureWidget.cpp common/ParameterWidget.cpp
                       common/AddWareDialog.cpp
                       common/AddParamDialog.cpp
                       )
                     
SET(BUILDER_COMMON_HPP common/AbstractModule.hpp
                       common/WorkspaceWidget.hpp
                       common/AppTools.hpp
                       common/WaresManagementWidget.hpp common/WareWidget.hpp common/ClickableWareWidget.hpp 
                       common/SignatureWidget.hpp common/ParameterWidget.hpp  
                       common/AddWareDialog.hpp
                       common/AddParamDialog.hpp
                      )

QT4_WRAP_UI(BUILDER_COMMON_UI common/WaresManagementWidget.ui common/WareWidget.ui common/SignatureWidget.ui common/ParameterWidget.ui
                              common/AddWareDialog.ui
                              common/AddParamDialog.ui
                              )



### MODEL #

SET(BUILDER_MODEL_CPP model/ModelWidget.cpp
                      model/SimulatorWidget.cpp
                      model/GeneratorWidget.cpp
                      model/ModelScene.cpp model/ModelView.cpp
                      model/ModelItemGraphics.cpp 
                      model/SimulatorGraphics.cpp model/GeneratorGraphics.cpp
                      model/ConnectorGraphics.cpp
                      model/AddSimulatorDialog.cpp model/AddGeneratorDialog.cpp
                     )

SET(BUILDER_MODEL_HPP model/ModelWidget.hpp
                      model/SimulatorWidget.hpp
                      model/GeneratorWidget.hpp
                      model/ModelScene.hpp model/ModelView.hpp
                      model/ModelItemGraphics.hpp 
                      model/SimulatorGraphics.hpp model/GeneratorGraphics.hpp
                      model/ConnectorGraphics.hpp 
                      model/AddSimulatorDialog.hpp model/AddGeneratorDialog.hpp
                     )

QT4_WRAP_UI(BUILDER_MODEL_UI model/ModelWidget.ui
                             model/AddGeneratorDialog.ui
                           )


### SPATIAL #

SET(BUILDER_SPATIAL_CPP spatial/SpatialDomainWidget.cpp
                        spatial/UnitsClassWidget.cpp
                        spatial/MapScene.cpp spatial/MapView.cpp
                        spatial/MapItemGraphics.cpp 
                        spatial/PunctualGraphics.cpp spatial/LinearGraphics.cpp spatial/SurfacicGraphics.cpp
                        spatial/PointGraphics.cpp spatial/MultiPointGraphics.cpp
                        spatial/LineStringGraphics.cpp spatial/MultiLineStringGraphics.cpp 
                        spatial/PolygonGraphics.cpp spatial/MultiPolygonGraphics.cpp 
                        spatial/EditAttributesValuesDialog.cpp
                        spatial/EditAttributeNameDialog.cpp
                        spatial/AddUnitDialog.cpp spatial/AddUnitsClassDialog.cpp spatial/AddUnitToClassDialog.cpp
                        spatial/AddConnectionDialog.cpp
                        spatial/EditEventDialog.cpp
                        spatial/AddEventDialog.cpp
                       )

SET(BUILDER_SPATIAL_HPP spatial/SpatialDomainWidget.hpp
                        spatial/UnitsClassWidget.hpp
                        spatial/MapScene.hpp spatial/MapView.hpp
                        spatial/MapItemGraphics.hpp 
                        spatial/PunctualGraphics.hpp spatial/LinearGraphics.hpp spatial/SurfacicGraphics.hpp
                        spatial/PointGraphics.hpp spatial/MultiPointGraphics.hpp
                        spatial/LineStringGraphics.hpp spatial/MultiLineStringGraphics.hpp 
                        spatial/PolygonGraphics.hpp spatial/MultiPolygonGraphics.hpp 
                        spatial/EditAttributesValuesDialog.hpp
                        spatial/EditAttributeNameDialog.hpp
                        spatial/AddUnitDialog.hpp spatial/AddUnitsClassDialog.hpp spatial/AddUnitToClassDialog.hpp
                        spatial/AddConnectionDialog.hpp
                        spatial/EditEventDialog.hpp
                        spatial/AddEventDialog.hpp
                       )

QT4_WRAP_UI(BUILDER_SPATIAL_UI spatial/SpatialDomainWidget.ui
                               spatial/UnitsClassWidget.ui
                               spatial/EditAttributesValuesDialog.ui
                               spatial/EditAttributeNameDialog.ui
                               spatial/AddUnitDialog.ui
                               spatial/AddConnectionDialog.ui
                               spatial/EditEventDialog.ui
                             )


### DATASTORE #

SET(BUILDER_DATASTORE_CPP datastore/DatastoreWidget.cpp
                          datastore/AddDatastoreItemDialog.cpp
                          )

SET(BUILDER_DATASTORE_HPP datastore/DatastoreWidget.hpp
                          datastore/AddDatastoreItemDialog.hpp
                          )

QT4_WRAP_UI(BUILDER_DATASTORE_UI datastore/DatastoreWidget.ui
                                 datastore/EditDatastoreItemDialog.ui
                                 )

### MONITORING #

SET(BUILDER_MONITORING_CPP monitoring/MonitoringWidget.cpp
                           monitoring/ObserverWidget.cpp
                           monitoring/AddObserverDialog.cpp
                          )

SET(BUILDER_MONITORING_HPP monitoring/MonitoringWidget.hpp
                           monitoring/ObserverWidget.hpp
                           monitoring/AddObserverDialog.hpp
                           )

QT4_WRAP_UI(BUILDER_MONITORING_UI monitoring/MonitoringWidget.ui
                                  )


### SIMCONFIG #

SET(BUILDER_RUNCONFIG_CPP runconfig/RunConfigurationWidget.cpp)

SET(BUILDER_RUNCONFIG_HPP runconfig/RunConfigurationWidget.hpp)

QT4_WRAP_UI(BUILDER_RUNCONFIG_UI runconfig/RunConfigurationWidget.ui)



### OUTPUTS #

SET(BUILDER_OUTPUTS_CPP outputs/OutputsWidget.cpp
                          )

SET(BUILDER_OUTPUTS_HPP outputs/OutputsWidget.hpp
                           )

QT4_WRAP_UI(BUILDER_OUTPUTS_UI outputs/OutputsWidget.ui
                                 )


### EXTENSION #

SET(BUILDER_EXTENSIONS_CPP extensions/ExtensionPluginsManager.cpp 
                           extensions/ExtensionContainer.cpp
                           extensions/ExtensionsRegistry.cpp
                          )

SET(BUILDER_EXTENSIONS_HPP extensions/ExtensionPluginsManager.hpp 
                           extensions/ExtensionContainer.hpp
                           extensions/ExtensionsRegistry.hpp
                           )

QT4_WRAP_UI(BUILDER_EXTENSIONS_UI )


### RESOURCES #

QT4_ADD_RESOURCES(BUILDER_QRC resources/openfluidbuilder.qrc)

SET(BUILDER_WIN32RC)

IF(WIN32)
  IF(MINGW)
    # resource compilation for MinGW
    ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources/openfluidbuilder-win32.o
                       COMMAND windres.exe 
                       ARGS -I${CMAKE_CURRENT_SOURCE_DIR}/resources 
                            -i${CMAKE_CURRENT_SOURCE_DIR}/resources/openfluidbuilder-win32.rc 
                            -o${CMAKE_CURRENT_BINARY_DIR}/resources/openfluidbuilder-win32.o)
    SET(BUILDER_WIN32RC ${CMAKE_CURRENT_BINARY_DIR}/resources/openfluidbuilder-win32.o)
  ELSE()
    SET(BUILDER_WIN32RC ${CMAKE_CURRENT_SOURCE_DIR}/resources/openfluidbuilder-win32.rc)
  ENDIF()
ENDIF()


### MAIN LIB AND EXECUTABLE #


IF(ENABLE_WARESDEV_BUILDER_INTEGRATION)
  ADD_DEFINITIONS(-DENABLE_WARESDEV_INTEGRATION)
ENDIF()  


IF(ENABLE_MARKET_BUILDER_INTEGRATION)
  ADD_DEFINITIONS(-DENABLE_MARKET_INTEGRATION)
ENDIF()  


ADD_LIBRARY(openfluid-builder-main SHARED ${BUILDER_BASE_CPP} ${BUILDER_BASE_HPP} ${BUILDER_BASE_UI}
                                          ${BUILDER_COMMON_CPP} ${BUILDER_COMMON_HPP} ${BUILDER_COMMON_UI}
                                          ${BUILDER_MODEL_CPP} ${BUILDER_MODEL_HPP} ${BUILDER_MODEL_UI}
                                          ${BUILDER_SPATIAL_CPP} ${BUILDER_SPATIAL_HPP} ${BUILDER_SPATIAL_UI}
                                          ${BUILDER_DATASTORE_CPP} ${BUILDER_DATASTORE_HPP} ${BUILDER_DATASTORE_UI}
                                          ${BUILDER_MONITORING_CPP} ${BUILDER_MONITORING_HPP} ${BUILDER_MONITORING_UI}
                                          ${BUILDER_RUNCONFIG_CPP} ${BUILDER_RUNCONFIG_HPP} ${BUILDER_RUNCONFIG_UI}
                                          ${BUILDER_OUTPUTS_CPP} ${BUILDER_OUTPUTS_HPP} ${BUILDER_OUTPUTS_UI}
                                          ${BUILDER_EXTENSIONS_CPP} ${BUILDER_EXTENSIONS_HPP} ${BUILDER_EXTENSIONS_UI}
                                          ${BUILDER_QRC})


TARGET_LINK_LIBRARIES(openfluid-builder-main 
                      ${Boost_SYSTEM_LIBRARY} 
                      ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} 
                      ${Boost_REGEX_LIBRARY}
                      ${GDAL_LIBRARIES}
                      ${QT_QTCORE_LIBRARIES}
                      ${QT_QTGUI_LIBRARIES}
                      ${QT_QTSVG_LIBRARIES}
                      openfluid-base
                      openfluid-buddies
                      openfluid-core
                      openfluid-machine
                      openfluid-ware
                      openfluid-tools
                      openfluid-utils
                      openfluid-market
                      openfluid-ui-common
                      openfluid-ui-market
                      openfluid-builderext
                      openfluid-fluidx
                      openfluid-ui-waresdev
                      openfluid-waresdev)
       
       
                      
SET_TARGET_PROPERTIES(openfluid-builder-main PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        AUTOMOC ON
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")


SET(BUILDER_GUI_OPTION)

IF(WIN32)
  SET(BUILDER_GUI_OPTION WIN32)
ENDIF()

ADD_EXECUTABLE(openfluid-builder ${BUILDER_GUI_OPTION} main.cpp ${BUILDER_WIN32RC})

SET_TARGET_PROPERTIES(openfluid-builder PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_PATH}"
                                                   AUTOMOC ON)

IF(APPLE)
  SET_TARGET_PROPERTIES(openfluid-builder PROPERTIES OUTPUT_NAME "OpenFLUID-Builder")
ENDIF()

TARGET_LINK_LIBRARIES(openfluid-builder
                      ${Boost_SYSTEM_LIBRARY} 
                      ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} 
                      ${Boost_REGEX_LIBRARY}
                      ${GDAL_LIBRARIES}
                      openfluid-base
                      openfluid-buddies
                      openfluid-core
                      openfluid-ware
                      openfluid-machine
                      openfluid-tools
                      openfluid-utils
                      openfluid-ui-common
                      openfluid-ui-market
                      openfluid-builder-main
                      openfluid-fluidx
                      openfluid-ui-waresdev
                      openfluid-waresdev)
                                   
INSTALL(TARGETS openfluid-builder-main
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
               
INSTALL(TARGETS openfluid-builder
        RUNTIME DESTINATION ${BIN_INSTALL_PATH})       
                                   
IF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))
  ADD_SUBDIRECTORY(extensions/tests)
ENDIF()         
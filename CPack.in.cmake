SET(OPENFLUID_PACKAGE_TITLE "OpenFLUID")
SET(OPENFLUID_PACKAGE_SUMMARY "OpenFLUID software environment for modelling landscapes functionning")
SET(OPENFLUID_PACKAGE_DESCRIPTION "This package contains the OpenFLUID software environment for modelling landscapes functionning, mainly for spatial fluxes modelling. It also includes API and tools for development.")
SET(OPENFLUID_PACKAGE_LICENSE "GPLv3")

SET(CPACK_CMAKE_GENERATOR "${CMAKE_GENERATOR}")

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/resources/packaging/readme.txt ${CMAKE_BINARY_DIR}/readme.txt COPYONLY)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/COPYING ${CMAKE_BINARY_DIR}/copyright.txt COPYONLY)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/resources/packaging/welcome.txt ${CMAKE_BINARY_DIR}/welcome.txt COPYONLY)


SET(CPACK_INSTALL_CMAKE_PROJECTS "${PROJECT_BINARY_DIR};${PROJECT_NAME};ALL;/") 

SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_BINARY_DIR}/copyright.txt")
SET(CPACK_RESOURCE_FILE_README "${CMAKE_BINARY_DIR}/readme.txt")
SET(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_BINARY_DIR}/welcome.txt")  

SET(CPACK_PACKAGE_NAME "openfluid")
SET(CPACK_PACKAGE_VERSION "${FULL_VERSION}")
SET(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
SET(CPACK_PACKAGE_CONTACT "OpenFLUID contact <contact@openfluid-project.org>")
SET(CPACK_PACKAGE_VENDOR "LISAH (INRA-IRD-SupAgro)")
SET(CPACK_PACKAGE_DESCRIPTION "${OPENFLUID_PACKAGE_TITLE}.\n ${OPENFLUID_PACKAGE_DESCRIPTION}")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${OPENFLUID_PACKAGE_SUMMARY}")



##### UNIX
IF(UNIX AND NOT APPLE AND NOT CYGWIN)
  INCLUDE(CPack.linux.in.cmake)
ENDIF(UNIX AND NOT APPLE AND NOT CYGWIN)


##### Windows
IF(WIN32)
  INCLUDE(CPack.win32.in.cmake)
ENDIF(WIN32)


##### Apple
IF(APPLE)
  INCLUDE(CPack.osx.in.cmake)  
ENDIF(APPLE)



SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}_src")
SET(CPACK_SOURCE_IGNORE_FILES "${CMAKE_BINARY_DIR}/*;/\\\\.svn/" "/\\\\..+$")

INCLUDE(CPack)


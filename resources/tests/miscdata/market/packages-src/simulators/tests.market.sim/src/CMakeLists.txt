CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

FIND_PACKAGE(OpenFLUIDHelpers REQUIRED)


SET(SIM_ID tests.market.sim)
SET(SIM_CPP MarketUseSim.cpp)

OPENFLUID_SHOW_CMAKE_VARIABLES()

OPENFLUID_ADD_DEFAULT_SIMULATOR()

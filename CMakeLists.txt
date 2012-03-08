# Project name
PROJECT(DCL_CameraNUI)

# CMake required version must be >= 2.6
cmake_minimum_required(VERSION 2.6)

# Find DisCODe installation
FIND_PACKAGE(DisCODe REQUIRED)

# Use CMake macros provided with DisCODe
set(CMAKE_MODULE_PATH ${DisCODe_DIR} ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/src/cmake/Modules/")
include(DisCODeMacros)

# Set default install prefix to dist folder
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX
      ${CMAKE_SOURCE_DIR}/dist CACHE PATH "${CMAKE_PROJECT_NAME} install prefix" FORCE
  )
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

ADD_SUBDIRECTORY(src)

OPTION(WITH_OPENNI "OpenNI" OFF)

FILE(WRITE "${PROJECT_SOURCE_DIR}/src/Components/CameraNUI/conf.hpp" "")
IF(WITH_OPENNI)
  configure_file (
    "${PROJECT_SOURCE_DIR}/src/Components/CameraNUI/conf.hpp.in"
    "${PROJECT_SOURCE_DIR}/src/Components/CameraNUI/conf.hpp"
  )
  include_directories("${PROJECT_SOURCE_DIR}")
ENDIF(WITH_OPENNI)

set(CMAKE_CXX_FLAGS "-fpermissive" CACHE STRING "Flags" FORCE)

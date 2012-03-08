SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(WITH_OPENNI ON)

include_directories(.)

# ##############################################################################
# Find required libraries
# ##############################################################################

# Find Boost, at least ver. 1.41
FIND_PACKAGE(Boost 1.41.0 REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

# Find another necessary libraries

# # OpenCV library
FIND_PACKAGE( OpenCV REQUIRED )

FIND_PACKAGE( OpenNI REQUIRED )
include_directories(${OpenNI_INCLUDE_DIR})
set(LIBS ${LIBS} ${OpenNI_LIBRARY})

# # libfreenect
find_package(libfreenect REQUIRED)
include_directories(${LIBFREENECT_INCLUDE_DIR})
set(LIBS ${LIBS} ${LIBFREENECT_LIBRARY})

# Find another DCLs our one depends on
# Use macro: DISCODE_FIND_DCL(<DCL_NAME>)

# ##############################################################################
# Add source directories
# ##############################################################################

# CvBlobs components
ADD_SUBDIRECTORY(Components)

# CvBlobs types
ADD_SUBDIRECTORY(Types)

# Prepare config file to use from another DCLs
CONFIGURE_FILE(DCL_CameraNUIConfig.cmake.in ${CMAKE_INSTALL_PREFIX}/DCL_CameraNUIConfig.cmake @ONLY)


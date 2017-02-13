##---------------------------------------------------------------------------
## Author:      Jean-Eudes Onfray
## Copyright:
## License:     wxWidgets License
##---------------------------------------------------------------------------


# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)
# This should be 2.8.0 to have FindGTK2 module

PROJECT(trackpoint_pi)

SET(PACKAGE_NAME trackpoint_pi)
SET(VERBOSE_NAME Trackpoint)
SET(TITLE_NAME Trackpoint)
SET(CPACK_PACKAGE_CONTACT "Ilias Daradimos")

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "2")

# This is for installing the library in /usr/lib (permissions needed)
# This way a plugin can be tested with package maintained versions of opencpn
#SET(CMAKE_INSTALL_PREFIX "/usr")

MESSAGE (STATUS "*** Building ${PACKAGE_NAME} ***")

#SET(CMAKE_BUILD_TYPE Debug)
INCLUDE("cmake/PluginConfigure.cmake")

SET(TRACKPOINT
            src/icons.h
            src/icons.cpp
            src/trackpoint_pi.h
            src/trackpoint_pi.cpp
            src/nmea/pore.cpp
            src/nmea/pore.hpp
            src/nmea/nmea0183.cpp
            src/nmea/nmea0183.hpp
 	)

ADD_LIBRARY(${PACKAGE_NAME} SHARED ${TRACKPOINT} )


INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")

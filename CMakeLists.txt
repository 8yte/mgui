#
# mgui (marvell gui) project cmake
###################################

cmake_minimum_required(VERSION 2.6)

PROJECT(MGUI)

ADD_DEFINITIONS(-Os -ggdb -Wall -Werror -Wmissing-declarations -DMARVELL_EXTENDED)

# define resource files target install
SET(MGUI_DATADIR ${CMAKE_INSTALL_PREFIX}/share/ilixi-1.0.0)
ADD_DEFINITIONS(-DDATADIR="${MGUI_DATADIR}")

SET_DIRECTORY_PROPERTIES(PROPERTIES
	ADDITIONAL_MAKE_CLEAN_FILES "Makefile;ipkg-install;ipkg-pxa1826;Makefile;CMakeCache.txt;CMakeFiles;cmake_install.cmake;install_manifest.txt;")

# find necessary libraries
INCLUDE(CheckLibraryExists)

CHECK_LIBRARY_EXISTS(rilutil rilutil_parseResponse "" MGUI_RILUTIL)
IF(MGUI_RILUTIL)
	LIST(APPEND LIBS rilutil)
ENDIF(MGUI_RILUTIL)

CHECK_LIBRARY_EXISTS(prop2uci property_get "" MGUI_PROP2UCI)
IF(MGUI_PROP2UCI)
	LIST(APPEND LIBS prop2uci)
ENDIF(MGUI_PROP2UCI)

find_library(json NAMES json-c json)
LIST(APPEND LIBS ubox ubus uci ${json} blobmsg_json)

find_library(librt NAMES rt librt)
LIST(APPEND LIBS ${librt})

# find ilixi-1.0.0
INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(ILIXI REQUIRED ilixi>=1.0.0)
ADD_DEFINITIONS(${ILIXI_CFLAGS})
LIST(APPEND LIBS ${ILIXI_LIBRARIES})

CONFIGURE_FILE(config.h.in config.h @ONLY)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(data)

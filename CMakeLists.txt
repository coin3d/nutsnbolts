cmake_minimum_required(VERSION 3.0)

project(NutsnBolts VERSION 0.1.0)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

# ############################################################################
# these will be removed after upgrading CMake minimum version
set(PROJECT_DESCRIPTION "An extension library for Coin")
# ############################################################################

string(TIMESTAMP NUTSNBOLTS_BUILD_YEAR "%Y")
math(EXPR NUTSNBOLTS_SO_VERSION ${PROJECT_VERSION_MAJOR}*20)

if(POLICY CMP0072)
  # get rid of OpenGL GLVND warning from CMake 3.11
  cmake_policy(SET CMP0072 NEW)
endif()

# ############################################################################
# Prevent in-source builds, as they often cause severe build problems
# ############################################################################

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "${CMAKE_PROJECT_NAME} requires an out of source build. Please create a separate build directory and run 'cmake <path_to_${CMAKE_PROJECT_NAME}> [options]' from there.")
endif()

# ############################################################################
# Include necessary submodules
# ############################################################################

include(CheckIncludeFiles)
include(CMakeDependentOption)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# ############################################################################
# Provide options to customise the build
# ############################################################################

function(report_prepare)
  set(multiValueArgs IF_APPLE IF_WIN32)
  cmake_parse_arguments(REPORT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
  if (REPORT_IF_APPLE AND APPLE)
    list(APPEND res ${REPORT_IF_APPLE})
  endif()
  if (REPORT_IF_WIN32 AND WIN32)
    list(APPEND res ${REPORT_IF_WIN32})
  endif()
  list(APPEND res ${REPORT_UNPARSED_ARGUMENTS})
  list(APPEND PACKAGE_OPTIONS ${res})
  set(PACKAGE_OPTIONS "${PACKAGE_OPTIONS}" PARENT_SCOPE)
endfunction(report_prepare)

option(HAVE_DEBUG                   "Add debugging information during the configure process")
option(NUTSNBOLTS_BUILD_SHARED_LIBS "Build shared libraries" ON)
option(NUTSNBOLTS_USE_QT6 "Prefer Qt6 over Qt5 if available" ON)
option(NUTSNBOLTS_USE_QT5 "Prefer Qt5 over Qt4 if available" ON)
option(NUTSNBOLTS_BUILD_DOCUMENTATION "Build and install API documentation (requires Doxygen)." OFF)
cmake_dependent_option(NUTSNBOLTS_BUILD_INTERNAL_DOCUMENTATION "Document internal code not part of the API." OFF "NUTSNBOLTS_BUILD_DOCUMENTATION" OFF)
cmake_dependent_option(NUTSNBOLTS_BUILD_DOC_MAN "Build NutsnBolts man pages." OFF "NUTSNBOLTS_BUILD_DOCUMENTATION" OFF)
cmake_dependent_option(NUTSNBOLTS_BUILD_DOC_QTHELP "Build QtHelp documentation." OFF "NUTSNBOLTS_BUILD_DOCUMENTATION" OFF)
cmake_dependent_option(NUTSNBOLTS_BUILD_DOC_CHM "Build compressed HTML help manual (requires HTML help compiler)" OFF "NUTSNBOLTS_BUILD_DOCUMENTATION" OFF)
option(NUTSNBOLTS_USE_CPACK "If enabled the cpack subrepo is mandatory" OFF)

report_prepare(
  NUTSNBOLTS_BUILD_SHARED_LIBS
  NUTSNBOLTS_USE_QT6
  NUTSNBOLTS_USE_QT5
  NUTSNBOLTS_BUILD_DOCUMENTATION
  NUTSNBOLTS_BUILD_INTERNAL_DOCUMENTATION
  NUTSNBOLTS_BUILD_DOC_MAN
  NUTSNBOLTS_BUILD_DOC_QTHELP
  NUTSNBOLTS_BUILD_DOC_CHM
)

# ############################################################################
# Find all necessary and optional NutsnBolts dependencies
# ############################################################################

# Fail early if one of the required packages cannot be found

find_package(OpenGL REQUIRED)
find_package(Coin REQUIRED)

set(NUTSNBOLTS_PKG_DEPS "Coin")

if(NUTSNBOLTS_USE_QT6)
  find_package(Qt6 COMPONENTS Widgets QUIET)
endif()

if(NOT Qt6_FOUND AND NUTSNBOLTS_USE_QT5)
  find_package(Qt5 COMPONENTS Widgets QUIET)
endif()

if(NOT Qt6_FOUND AND NOT Qt5_FOUND)
  set(QT_USE_IMPORTED_TARGETS ON)
  find_package(Qt4 COMPONENTS QtGui REQUIRED)
endif()

if(Qt6_FOUND)
  set(NUTSNBOLTS_QT_TARGETS Qt6::Widgets)
  string(REGEX REPLACE "^([0-9]+)\\.[0-9]+\\.[0-9]+.*" "\\1" QT_VERSION_MAJOR "${Qt6Core_VERSION_STRING}")
  string(REGEX REPLACE "^[0-9]+\\.([0-9]+)\\.[0-9]+.*" "\\1" QT_VERSION_MINOR "${Qt6Core_VERSION_STRING}")
  string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" QT_VERSION_PATCH "${Qt6Core_VERSION_STRING}")
  set(NUTSNBOLTS_PKG_DEPS "${NUTSNBOLTS_PKG_DEPS} Qt6Widgets")
elseif(Qt5_FOUND)
  set(NUTSNBOLTS_QT_TARGETS Qt5::Widgets)
  string(REGEX REPLACE "^([0-9]+)\\.[0-9]+\\.[0-9]+.*" "\\1" QT_VERSION_MAJOR "${Qt5Core_VERSION_STRING}")
  string(REGEX REPLACE "^[0-9]+\\.([0-9]+)\\.[0-9]+.*" "\\1" QT_VERSION_MINOR "${Qt5Core_VERSION_STRING}")
  string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" QT_VERSION_PATCH "${Qt5Core_VERSION_STRING}")
  set(NUTSNBOLTS_PKG_DEPS "${NUTSNBOLTS_PKG_DEPS} Qt5Widgets")
else()
  set(NUTSNBOLTS_QT_TARGETS Qt4::QtGui)
  include(${QT_USE_FILE})
  set(NUTSNBOLTS_PKG_DEPS "${NUTSNBOLTS_PKG_DEPS} QtGui")
endif()

set(NUTSNBOLTS_PKG_LIBS "" CACHE INTERNAL "Link libraries for package config")
set(NUTSNBOLTS_PKG_FLAGS "" CACHE INTERNAL "Compilation flags for package config")

find_package(SoQt REQUIRED)

# ##########################################################################
# Setup build environment
# ##########################################################################

if(NOT CMAKE_BUILD_TYPE)
  # Has no effect for multi configuration generators (VisualStudio, Xcode).
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose type of build, options are Debug, Release, RelWithDebInfo, MinSizeRel." FORCE)
endif()
# Set common output directories for all targets built.
# First for the generic no-config case (e.g. with mingw)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
# Second, for multi-config builds (e.g. msvc)
foreach (_config ${CMAKE_CONFIGURATION_TYPES})
  string(TOUPPER ${_config} _config)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${_config} "${CMAKE_BINARY_DIR}/lib")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${_config} "${CMAKE_BINARY_DIR}/lib")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${_config} "${CMAKE_BINARY_DIR}/bin")
endforeach()

function(versionize)
  foreach(dir ${ARGN})
    set(name      "CMAKE_INSTALL_${dir}")
    set(full-name "CMAKE_INSTALL_FULL_${dir}")
    set(value      "${${name}}/${PROJECT_NAME}")
    set(full-value "${CMAKE_INSTALL_PREFIX}/${value}")
    set(${name}      ${value}      PARENT_SCOPE)
    set(${full-name} ${full-value} PARENT_SCOPE)
  endforeach()
endfunction()

function(dump_variable)
  if (HAVE_DEBUG)
    foreach(f ${ARGN})
      if (DEFINED ${f})
        message("${f} = ${${f}}")
      else()
        message("${f} = ***UNDEF***")
      endif()
    endforeach()
  endif()
endfunction()

###############################################################################
# Build setup

dump_variable(
  CMAKE_INSTALL_FULL_INCLUDEDIR
  CMAKE_INSTALL_FULL_DOCDIR
)
versionize(INCLUDEDIR)
dump_variable(
  CMAKE_INSTALL_FULL_INCLUDEDIR
  CMAKE_INSTALL_FULL_DOCDIR
)

# ############################################################################
# Setup targets in subdirectories
# ############################################################################

add_subdirectory(lib)
add_subdirectory(apps)
add_subdirectory(docs)

# ############################################################################
# Install pkgconfig file and CMake config package
# ############################################################################

if(UNIX OR MINGW)
  configure_file("${CMAKE_SOURCE_DIR}/NutsnBolts.pc.cmake.in" NutsnBolts.pc @ONLY)
  install(FILES "${CMAKE_BINARY_DIR}/NutsnBolts.pc" DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()

configure_package_config_file("${CMAKE_SOURCE_DIR}/${PROJECT_NAME_LOWER}-config.cmake.in" ${PROJECT_NAME_LOWER}-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
  PATH_VARS CMAKE_INSTALL_DOCDIR CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file("${CMAKE_BINARY_DIR}/${PROJECT_NAME_LOWER}-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY ExactVersion
)

# To make the component usable not only from the install directory but also from the build directory
export(
  TARGETS ${PROJECT_NAME}
  FILE ${PROJECT_NAME_LOWER}-export.cmake
)

install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME_LOWER}-config.cmake" "${CMAKE_BINARY_DIR}/${PROJECT_NAME_LOWER}-config-version.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
  COMPONENT development
)

install(EXPORT ${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}::
  FILE ${PROJECT_NAME_LOWER}-export.cmake
  COMPONENT development
)

# CPACK section moved in the package-config directory. 
# Please see the README file inside.
if (NUTSNBOLTS_USE_CPACK)
  add_subdirectory(cpack.d)
endif()

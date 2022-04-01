configure_file(config.h.cmake.in config.h)
configure_file(NutsnBolts/Basic.h.cmake.in NutsnBolts/Basic.h)

file(GLOB_RECURSE HDRS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE BIN_HDRS ${CMAKE_CURRENT_BINARY_DIR}/*.h)

set(SRCS
  NutsnBolts/NutsnBolts.cpp
  NutsnBolts/NbSceneManager.cpp
  NutsnBolts/NbSceneManagerP.cpp
  NutsnBolts/misc/NbNamedEvent.cpp
  NutsnBolts/misc/SoEvent.cpp
  NutsnBolts/navigation/extra.cpp
  NutsnBolts/navigation/NbCenterMode.cpp
  NutsnBolts/navigation/NbIdleMode.cpp
  NutsnBolts/navigation/NbNavigationControl.cpp
  NutsnBolts/navigation/NbNavigationMode.cpp
  NutsnBolts/navigation/NbNavigationState.cpp
  NutsnBolts/navigation/NbNavigationSystem.cpp
  NutsnBolts/navigation/NbNavigationVehicle.cpp
  NutsnBolts/navigation/NbOrthoCenterMode.cpp
  NutsnBolts/navigation/NbPanMode.cpp
  NutsnBolts/navigation/NbPickMode.cpp
  NutsnBolts/navigation/NbPitchMode.cpp
  NutsnBolts/navigation/NbRollMode.cpp
  NutsnBolts/navigation/NbRotateMode.cpp
  NutsnBolts/navigation/NbYawMode.cpp
  NutsnBolts/navigation/NbZoomMode.cpp
  NutsnBolts/nodes/NbEventNode.cpp
  NutsnBolts/nodes/NbScriptEvent.cpp
  NutsnBolts/nodes/NbViewerNavigationMode.cpp
)

if(WIN32)
  if(MINGW)
    set(NUTSNBOLTS_DEFAULT_SHARED_POSTFIX "")
    set(NUTSNBOLTS_DEFAULT_STATIC_POSTFIX "")
  else()
    # On Windows the major version number is part of the library name
    set(NUTSNBOLTS_DEFAULT_SHARED_POSTFIX ${PROJECT_VERSION_MAJOR})
    set(NUTSNBOLTS_DEFAULT_STATIC_POSTFIX ${PROJECT_VERSION_MAJOR}s)
  endif()
  if(NUTSNBOLTS_BUILD_SHARED_LIBS)
    set(NUTSNBOLTS_DEFAULT_POSTFIX ${NUTSNBOLTS_DEFAULT_SHARED_POSTFIX})
    set(NUTSNBOLTS_PKG_FLAGS "-DNB_DLL" CACHE INTERNAL "Compilation flags for package config" FORCE)
  else()
    set(NUTSNBOLTS_DEFAULT_POSTFIX ${NUTSNBOLTS_DEFAULT_STATIC_POSTFIX})
    set(NUTSNBOLTS_PKG_FLAGS "-DNB_NOT_DLL" CACHE INTERNAL "Compilation flags for package config" FORCE)
  endif()
  set(CMAKE_RELEASE_POSTFIX ${NUTSNBOLTS_DEFAULT_POSTFIX})
  set(CMAKE_MINSIZEREL_POSTFIX ${NUTSNBOLTS_DEFAULT_POSTFIX})
  set(CMAKE_RELWITHDEBINFO_POSTFIX ${NUTSNBOLTS_DEFAULT_POSTFIX})
  set(CMAKE_DEBUG_POSTFIX ${NUTSNBOLTS_DEFAULT_POSTFIX}d)
elseif(APPLE)
else()
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()
set(NUTSNBOLTS_PKG_LIBS "-l${PROJECT_NAME}" CACHE INTERNAL "Link libraries for package config" FORCE)

# define additional documentation
set(NB_DOCUMENTATION_FILES ${NB_DOCUMENTATION_FILES}
  ${CMAKE_CURRENT_SOURCE_DIR}/NutsnBolts
  ${CMAKE_CURRENT_SOURCE_DIR}/NutsnBolts/misc
  ${CMAKE_CURRENT_SOURCE_DIR}/NutsnBolts/navigation
  ${CMAKE_CURRENT_SOURCE_DIR}/NutsnBolts/nodes
  CACHE INTERNAL "${PROJECT_NAME} documentation files")

if(NUTSNBOLTS_BUILD_SHARED_LIBS)
  add_library(${PROJECT_NAME} SHARED ${HDRS} ${BIN_HDRS} ${SRCS})
else()
  add_library(${PROJECT_NAME} STATIC ${HDRS} ${BIN_HDRS} ${SRCS})
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_CONFIG_H NB_INTERNAL NUTSNBOLTS_DEBUG=$<CONFIG:Debug>)
if(WIN32)
  if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_NONSTDC_NO_DEPRECATE _CRT_SECURE_NO_DEPRECATE _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
  endif()
  if(NUTSNBOLTS_BUILD_SHARED_LIBS)
    configure_file(${PROJECT_NAME}.rc.cmake.in ${PROJECT_NAME}.rc)
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.rc")
    target_compile_definitions(${PROJECT_NAME} PRIVATE NB_MAKE_DLL INTERFACE NB_DLL)
  else()
    target_compile_definitions(${PROJECT_NAME} INTERFACE NB_NOT_DLL)
    if(MSVC)
      set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Z7")                       # Override default /Zi to embed
      set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Z7")     # debugging info in the .lib.
    endif()
  endif()
endif()

target_include_directories(${PROJECT_NAME}
PUBLIC
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib>
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/lib>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(WIN32 AND NOT MINGW)
  set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${NUTSNBOLTS_SO_VERSION})
else()
  set_target_properties(${PROJECT_NAME} PROPERTIES VERSION "${NUTSNBOLTS_SO_VERSION}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" SOVERSION ${NUTSNBOLTS_SO_VERSION})
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC Coin::Coin ${OPENGL_LIBRARIES})

# ############################################################################
# Install targets
# ############################################################################

if(NOT CMAKE_VERSION VERSION_LESS 3.12)
  install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtime NAMELINK_COMPONENT development
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
  )
  
  if(NUTSNBOLTS_BUILD_SHARED_LIBS)
    if(MSVC)
      install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION ${CMAKE_INSTALL_BINDIR} CONFIGURATIONS Debug RelWithDebInfo COMPONENT debug)
    endif()
  endif()
else()
  install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtime NAMELINK_SKIP
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
  )
  
  if(NUTSNBOLTS_BUILD_SHARED_LIBS)
    install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development NAMELINK_ONLY)
    if(MSVC)
      install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION ${CMAKE_INSTALL_BINDIR} CONFIGURATIONS Debug RelWithDebInfo COMPONENT debug)
    endif()
  endif()
endif()

set(INSTALL_HDRS ${HDRS})
list(REMOVE_ITEM INSTALL_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/NutsnBolts/misc/SoEvent.h)

install(FILES ${INSTALL_HDRS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT development)
install(FILES ${BIN_HDRS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT development)

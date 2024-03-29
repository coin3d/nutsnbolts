file(GLOB_RECURSE DOX "*.dox*")

set(NB_DOCUMENTATION_FILES ${CMAKE_SOURCE_DIR}/README.md ${DOX} ${NB_DOCUMENTATION_FILES} CACHE INTERNAL "${PROJECT_NAME} documentation files")

# Add a target to generate API documentation with Doxygen
if(NUTSNBOLTS_BUILD_DOCUMENTATION)
  find_package(Doxygen)
  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation.")
  endif()

  if(NOT "${Coin_DOC_DIR}" STREQUAL "")
    get_filename_component(_coin_versioned_dir ${Coin_DOC_DIR} NAME)
    set(DOXYGEN_TAGFILES "${Coin_DOC_DIR}/html/Coin.tag=../../${_coin_versioned_dir}/html")
  endif()

# ############################################################################
# Setup documentation options
# ############################################################################
  set(GENERATE_HTMLHELP NO)
  set(DOXYGEN_GENERATE_MAN NO)
  set(GENERATE_QHP NO)
  set(GENERATE_TREEVIEW YES)
  set(HHC_PROGRAM)
  if(WIN32)
    if(NUTSNBOLTS_BUILD_DOC_CHM)
      find_program(HHC_PROGRAM NAMES hhc.exe PATHS "C:/Program Files/HTML Help Workshop" "C:/Program Files (x86)/HTML Help Workshop" DOC "HTML Help Compiler program")
      if(NOT HHC_PROGRAM)
        message(FATAL_ERROR "Missing program HTML Help Compiler")
      else()
        set(GENERATE_HTMLHELP YES)
        set(GENERATE_TREEVIEW NO)
        mark_as_advanced(HHC_PROGRAM)
      endif()
    endif()
  else()
    if(NUTSNBOLTS_BUILD_DOC_MAN)
      set(DOXYGEN_GENERATE_MAN YES)
    endif()
  endif()
  if(NUTSNBOLTS_BUILD_DOC_QTHELP)
    find_program(QHG_LOCATION NAMES qhelpgenerator qhelpgenerator-qt5 DOC "Qt qhelpgenerator")
    if(NOT QHG_LOCATION)
      message(FATAL_ERROR "Missing program Qt qhelpgenerator")
    else()
      set(GENERATE_QHP YES)
      mark_as_advanced(QHG_LOCATION)
    endif()
  endif()

  set(DOXYGEN_INTERNAL_DOCS NO)
  set(DOXYGEN_EXTRACT_PRIVATE NO)
  set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
  set(DOXYGEN_EXCLUDE)
  if(NUTSNBOLTS_BUILD_INTERNAL_DOCUMENTATION)
    set(DOXYGEN_EXTRACT_PRIVATE YES)
    set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)
    set(DOXYGEN_INTERNAL_DOCS YES)
    set(NB_DOCUMENTATION_FILES
      ${NB_DOCUMENTATION_FILES}
      ${NB_INTERNAL_DOCUMENTATION_FILES}
      CACHE INTERNAL "NB_DOCUMENTATION_FILES"
    )
  else()
    set(NB_DOCUMENTATION_EXCLUDE_SYMBOLS SoProfilerP)
    foreach(XF ${NB_INTERNAL_DOCUMENTATION_FILES})
      set(DOXYGEN_EXCLUDE "${DOXYGEN_EXCLUDE}                         ${XF} \\\n")
    endforeach(XF)
  endif()
  set(DOXYGEN_INPUT)
  list(REMOVE_DUPLICATES NB_DOCUMENTATION_FILES)
  foreach(XF ${NB_DOCUMENTATION_FILES})
    set(DOXYGEN_INPUT "${DOXYGEN_INPUT}                         ${XF} \\\n")
  endforeach(XF)
  dump_variable(
  NB_DOCUMENTATION_FILES
  DOXYGEN_INPUT
  )
  set(DOXYGEN_EXCLUDE_SYMBOLS)
  foreach(XF ${NB_DOCUMENTATION_EXCLUDE_SYMBOLS})
    set(DOXYGEN_EXCLUDE_SYMBOLS "${DOXYGEN_EXCLUDE_SYMBOLS}                         ${XF} \\\n")
  endforeach(XF)

  set(DOXYFILE "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
  set(DOXYGEN_OUTPUT "${CMAKE_BINARY_DIR}/html/index.html")
  dump_variable(DOXYGEN_TAGFILES)
  configure_file(nutsnbolts.doxygen.cmake.in ${DOXYFILE} @ONLY)

# ############################################################################
# Setup documentation targets
# ############################################################################

  add_custom_command(
    OUTPUT ${DOXYGEN_OUTPUT}
    COMMAND ${CMAKE_COMMAND} -E echo_append "Generating API documentation with Doxygen "
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
    COMMAND ${CMAKE_COMMAND} -E echo "done."
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    DEPENDS ${DOXYFILE}
  )
  add_custom_target(documentation ALL DEPENDS ${DOXYGEN_OUTPUT})

# ############################################################################
# Install built documentation files
# ############################################################################
  install(DIRECTORY "${CMAKE_BINARY_DIR}/html" DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT documentation REGEX ".*\\.(chm|qch)" EXCLUDE)
  if(GENERATE_HTMLHELP)
    install(FILES "${CMAKE_BINARY_DIR}/html/${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.chm" DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT documentation)
  endif()
  if(GENERATE_QHP)
    install(FILES "${CMAKE_BINARY_DIR}/html/${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.qch" DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT documentation)
  endif()
  if(DOXYGEN_GENERATE_MAN)
    install(DIRECTORY "${CMAKE_BINARY_DIR}/man/man3" DESTINATION ${CMAKE_INSTALL_MANDIR} COMPONENT documentation)
    message(STATUS "CMAKE_INSTALL_MANDIR ${CMAKE_INSTALL_MANDIR}")
  endif()
endif()

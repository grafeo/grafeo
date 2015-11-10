# ===================================================================
#   Copyright (C) 2015-2015
#   Anderson Tavares <nocturne.pe at gmail.com> PK 0x38e7bfc5c2def8ff
#   Lucy Mansilla    <lucyacm at gmail.com>
#   Caio de Braz     <caiobraz at gmail.com>
#   Hans Harley      <hansbecc at gmail.com>
#   Paulo Miranda    <pavmbr at yahoo.com.br>
#
#   Institute of Mathematics and Statistics - IME
#   University of Sao Paulo - USP
#
#   This file is part of Grafeo.
#
#   Grafeo is free software: you can redistribute it and/or
#   modify it under the terms of the GNU General Public License
#   as published by the Free Software Foundation, either version
#   3 of the License, or (at your option) any later version.
#
#   Grafeo is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty
#   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#   See the GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public
#   License along with Grafeo.  If not, see
#   <http://www.gnu.org/licenses/>.
# ===================================================================
cmake_minimum_required(VERSION 2.8)
project(grafeo)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(PkgConfig REQUIRED)

set(EXTERNAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/include)
set(EXTERNAL_LIB_DIR ${CMAKE_SOURCE_DIR}/external/lib)
# Getting GPL Header
FILE(READ "GPLHEADER" GRF_GPL_HEADER)

# -----------------
# Contents
# - Options (to enable/disable components at compilation)
# - Custom Functions and Macros
# - Third Party Libraries
# - Tests setup (before compilation)
# - Library setup (e.g. constants)
# - Compilation of modules
# - Documentation
# - Debug or not debug?
# - Compilation of tests
# -----------------

#====================================================================== OPTIONS
option(BUILD_CORE    "Core Module (data structures, config, macros, types...)" ON)
option(BUILD_DISPLAY "GUI Module (Requires Gtk)" ON)
option(BUILD_IMGPROC "Image Processing Module (Requires LibJPEG and LibPNG)" ON)
option(BUILD_IFT     "Image Foresting Transform methods" ON)
option(BUILD_CHART   "Component for creating charts" ON)
option(BUILD_TESTING "" ON)
option(BUILD_DOCS    "Build doxygen documentation" OFF)
option(COVERALLS     "Generate coveralls data" OFF)
option(COVERALLS_UPLOAD "Upload the generated coveralls json" ON)
option(DEBUG_INFO    "Compile Grafeo with debug information?" ON)
#================================================== CUSTOM FUNCTIONS AND MACROS

# ------------------
# If we don't find a required third party library, we can compile its source code
# and use them
# ------------------
MACRO (load_submodule nome nome_lib)
    set(${nome}CMakeArgs
       -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
       -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/external
       -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
       -DUNIT_TESTING=OFF
       )

    ExternalProject_Add(${nome_lib}Download
        PREFIX ${CMAKE_SOURCE_DIR}/external
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/${nome_lib}
        BINARY_DIR ${CMAKE_BINARY_DIR}/external/${nome_lib}-build
        STAMP_DIR ${CMAKE_BINARY_DIR}/external/${nome_lib}-stamp
        TMP_DIR ${CMAKE_BINARY_DIR}/external/${nome_lib}-tmp
        INSTALL_DIR ${CMAKE_BINARY_DIR}/external
        set(libnome ${nome}CMakeArgs)
        CMAKE_ARGS ${libnome}
        # GIT_REPOSITORY "git://git.cryptomilk.org/projects/cmocka.git"
        #GIT_TAG "origin/master"
        #GIT_SUBMODULES ""
        UPDATE_COMMAND cd ${CMAKE_SOURCE_DIR} && git submodule update --init --recursive external/${nome_lib}
        DOWNLOAD_COMMAND cd ${CMAKE_SOURCE_DIR} && git submodule update --init --recursive external/${nome_lib}
    )
    STRING(TOUPPER ${nome_lib} NOME_LIB_UPPER)
    set(${NOME_LIB_UPPER}_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/include)
    set(${NOME_LIB_UPPER}_LIB_DIR ${CMAKE_SOURCE_DIR}/external/lib)
    add_library(${nome_lib} SHARED IMPORTED)
    set_target_properties(${nome_lib} PROPERTIES
        IMPORTED_LOCATION ${EXTERNAL_LIB_DIR}/lib${nome_lib}.so)
ENDMACRO()
# ------------------
# Concatenate header files from a module
# ------------------
function(GRF_CAT IN_FILE OUT_FILE lowername)
  file(READ ${IN_FILE} CONTENTS)
  # Remove gpl header
  string(REGEX REPLACE "/\\*[^*]*\\*/(.*)" "\\1" CONTENTS2 "${CONTENTS}")
  string(REGEX REPLACE "#include <grafeo/${lowername}.h>" "" CONTENTS2 "${CONTENTS2}")
  file(APPEND ${OUT_FILE} "${CONTENTS2}")
endfunction()

# ------------------
# Get current Year
# ------------------
MACRO (TODAY RESULT)
    IF (WIN32)
        EXECUTE_PROCESS(COMMAND "date" "/T" OUTPUT_VARIABLE ${RESULT})
        string(REGEX REPLACE "(..)/(..)/(....).*" "\\3" ${RESULT} ${${RESULT}})
    ELSEIF(UNIX)
        EXECUTE_PROCESS(COMMAND "date" "+%Y" OUTPUT_VARIABLE ${RESULT})
        #string(REGEX REPLACE "(..)/(..)/..(..).*" "\\3\\2\\1" ${RESULT} ${${RESULT}})
    ELSE (WIN32)
        MESSAGE(SEND_ERROR "date not implemented")
        SET(${RESULT} 000000)
    ENDIF (WIN32)
ENDMACRO (TODAY)
# ------------------
# Create the library components
# ------------------
MACRO(GRF_CREATE_LIBRARY lowername UPPERNAME)
  # Prepare a temporary file to "cat" to:
  FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${lowername}.h.in "${GRF_GPL_HEADER}")

  SET(HDRS GRF_${UPPERNAME}_HDRS)
  SET(SRCS GRF_${UPPERNAME}_SRCS)
  SET(INCLUDE_DIR GRF_${UPPERNAME}_INCLUDE_DIR)
  SET(SOURCE_DIR  GRF_${UPPERNAME}_SOURCE_DIR)
  FOREACH(HDR ${${HDRS}})
    # Append directory to headers
    LIST(APPEND HDRS_ABSOLUTE ${${INCLUDE_DIR}}/${HDR})
    SET(GRF_HDRS_DOXYGEN "${GRF_HDRS_DOXYGEN}\n INPUT+=${${INCLUDE_DIR}}/${HDR}")
    # Concatenate header files
    GRF_CAT(${${INCLUDE_DIR}}/${HDR} ${CMAKE_CURRENT_BINARY_DIR}/${lowername}.h.in ${lowername})
  ENDFOREACH()
  LIST(APPEND GRF_HDRS ${HDRS_ABSOLUTE})

  # Copy the temporary file to the final location
  CONFIGURE_FILE(${CMAKE_CURRENT_BINARY_DIR}/${lowername}.h.in ${GRF_INCLUDE_DIR}/${lowername}.h)

  # Append directory to sources
  SET(SRCS_ABSOLUTE)
  FOREACH(SOURCE ${${SRCS}})
    LIST(APPEND SRCS_ABSOLUTE ${${SOURCE_DIR}}/${SOURCE})
  ENDFOREACH()
  LIST(APPEND GRF_SRCS ${SRCS_ABSOLUTE})

  # Create the library
  ADD_LIBRARY(grafeo_${lowername} SHARED ${SRCS_ABSOLUTE} ${HDRS_ABSOLUTE})
  # Create the links
  SET_TARGET_PROPERTIES(grafeo_${lowername} PROPERTIES
    VERSION   ${GRF_VERSION_STRING}
    SOVERSION ${GRF_VERSION_MAJOR})
  # Add to install script
  INSTALL(TARGETS grafeo_${lowername} LIBRARY DESTINATION lib)
  INSTALL(FILES ${GRF_INCLUDE_DIR}/${lowername}.h DESTINATION include/grafeo)
  Message(STATUS "${UPPERNAME} module compilation configured...")
ENDMACRO()
#======================================================== THIRD PARTY LIBRARIES
IF(CMAKE_VERSION  VERSION_LESS "2.8.11")
  TODAY(GRF_CURRENT_YEAR)
  MESSAGE(${GRF_CURRENT_YEAR})
ELSE()
  STRING(TIMESTAMP GRF_CURRENT_YEAR "%Y")
ENDIF()

# ------------------
# PNG
# ------------------
if(BUILD_IMGPROC OR BUILD_DISPLAY)
    find_package(PNG QUIET)
    if(NOT PNG_FOUND)
        load_submodule(PNG png)
    endif(NOT PNG_FOUND)
endif()
# ------------------
# JPG
# ------------------
if(BUILD_IMGPROC OR BUILD_DISPLAY)
    find_package(JPEG QUIET)
    if(NOT JPEG_FOUND)

    endif(NOT JPEG_FOUND)
endif()
# ------------------
# GObject
# ------------------
IF(BUILD_CHART)
  pkg_check_modules(gobject REQUIRED gobject-2.0)
  INCLUDE_DIRECTORIES(${gobject_INCLUDE_DIRS})
ENDIF(BUILD_CHART)
# ------------------
# GTK
# ------------------
IF(BUILD_DISPLAY)
  pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
  pkg_check_modules(goocanvas REQUIRED goocanvas-2.0)
  INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS} ${goocanvas_INCLUDE_DIRS})
endif()
# ------------------
# CMOCKA: for testing
# ------------------
IF(BUILD_TESTING)
  INCLUDE(CTest)
  INCLUDE(ExternalProject)
  FIND_PACKAGE(CMocka QUIET)
  #MESSAGE(STATUS Cmocka Found:${CMOCKA_FOUND})
  IF(NOT CMOCKA_FOUND)
      # Tell message
      # message(STATUS "Não pode compilar os testes")
      # Compile it from git submodule
      #load_submodule(CMocka cmocka)
      set(CMockaCMakeArgs
         -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
         -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/external
         -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
         -DUNIT_TESTING=OFF
         )

      ExternalProject_Add(cmockaDownload
          PREFIX ${CMAKE_SOURCE_DIR}/external
          SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/cmocka
          BINARY_DIR ${CMAKE_BINARY_DIR}/external/cmocka-build
          STAMP_DIR ${CMAKE_BINARY_DIR}/external/cmocka-stamp
          TMP_DIR ${CMAKE_BINARY_DIR}/external/cmocka-tmp
          INSTALL_DIR ${CMAKE_BINARY_DIR}/external
          CMAKE_ARGS ${CMockaCMakeArgs}
          # GIT_REPOSITORY "git://git.cryptomilk.org/projects/cmocka.git"
          #GIT_TAG "origin/master"
          #GIT_SUBMODULES ""
          UPDATE_COMMAND cd ${CMAKE_SOURCE_DIR} && git submodule update --init --recursive external/cmocka
          DOWNLOAD_COMMAND cd ${CMAKE_SOURCE_DIR} && git submodule update --init --recursive external/cmocka
      )
      set(CMOCKA_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/include)
      set(CMOCKA_LIB_DIR ${CMAKE_SOURCE_DIR}/external/lib)
      add_library(cmocka SHARED IMPORTED)
      set_target_properties(cmocka PROPERTIES IMPORTED_LOCATION ${CMOCKA_LIB_DIR}/libcmocka.so)
  ENDIF(NOT CMOCKA_FOUND)
ENDIF(BUILD_TESTING)
# ------------------
# COVERALLS: for test coverage
# ------------------
IF(BUILD_TESTING)
  IF(COVERALLS)
    INCLUDE(Coveralls)
    SET(COVERALLS_REPO_TOKEN "jcNqZoqi87ip0EoPWLy5b8nzfFm4Z0MlL")
  ENDIF()
  set(CMAKE_EXE_LINKER_FLAGS "-g -Wall -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -pg -fprofile-arcs -ftest-coverage")
  set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${CMAKE_EXE_LINKER_FLAGS})
  Message(STATUS "Tests parameters setup...")
ENDIF()
Message(STATUS "Third party libraries loaded...")
#====================================================================== LIBRARY
# Setting some constants for all modules
SET(GRF_INCLUDE_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/include/grafeo)
SET(GRF_SOURCE_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/src)
SET(GRF_MODULES_LOWER  core display ift imgproc chart)
SET(GRF_MODULES_UPPER  CORE DISPLAY IFT IMGPROC CHART)
list(LENGTH GRF_MODULES_LOWER len1)
math(EXPR   len2 "${len1} - 1")
FOREACH(module_index RANGE ${len2})
  list(GET GRF_MODULES_LOWER ${module_index} GRF_MODULE_LOWER)
  list(GET GRF_MODULES_UPPER ${module_index} GRF_MODULE_UPPER)
  SET(GRF_${GRF_MODULE_UPPER}_INCLUDE_DIR ${GRF_INCLUDE_DIR}/${GRF_MODULE_LOWER})
  SET(GRF_${GRF_MODULE_UPPER}_SOURCE_DIR ${GRF_SOURCE_DIR}/${GRF_MODULE_LOWER})
ENDFOREACH()
include_directories(include)
set(GRF_VERSION_MAJOR 0)
set(GRF_VERSION_MINOR 0)
set(GRF_VERSION_PATCH 7)
set(GRF_VERSION_STRING ${GRF_VERSION_MAJOR}.${GRF_VERSION_MINOR}.${GRF_VERSION_PATCH})
Message(STATUS "Library parameters setup...")
#================================================================== CORE MODULE
# Even though BUILD_CORE is OFF, if for example BUILD_IMGPROC is ON,
# the CORE module will be compiled
IF(BUILD_CORE OR BUILD_IMGPROC OR BUILD_IFT OR BUILD_DISPLAY)
  SET(GRF_CORE_HDRS     config.h i18n-lib.h  macro.h  type.h    hal.h  range.h
                         slist.h     list.h  array.h queue.h bucket.h pqueue.h)
  SET(GRF_CORE_SRCS       type.c config.c  range.c slist.c list.c array.c
                         queue.c bucket.c pqueue.c)
  # configure a header file to pass some of the CMake settings
  # to the source code
  CONFIGURE_FILE("${GRF_CORE_INCLUDE_DIR}/config.h.in"
                 "${GRF_CORE_INCLUDE_DIR}/config.h")
  GRF_CREATE_LIBRARY(core CORE)
ENDIF()
#=============================================================== IMGPROC MODULE
IF(BUILD_IMGPROC OR BUILD_DISPLAY)
  SET(GRF_IMGPROC_HDRS  image.h  drawing.h)
  SET(GRF_IMGPROC_SRCS  image.c  drawing.c)
  GRF_CREATE_LIBRARY(imgproc IMGPROC)
ENDIF()
#=================================================================== IFT MODULE
IF(BUILD_IFT)
  SET(GRF_IFT_HDRS  ift.h)
  SET(GRF_IFT_SRCS  ift.c)
  GRF_CREATE_LIBRARY(ift IFT)
ENDIF()
#=============================================================== DISPLAY MODULE
IF(BUILD_DISPLAY)
  SET(GRF_DISPLAY_HDRS idisplaywidget.h imagewidget.h chartwidget.h trackbar.h
                       displaywindow.h display.h)
  SET(GRF_DISPLAY_SRCS idisplaywidget.c imagewidget.c chartwidget.c trackbar.c
                       displaywindow.c display.c)
  GRF_CREATE_LIBRARY(display DISPLAY)
ENDIF()
#================================================================= CHART MODULE
IF(BUILD_CHART)
  SET(GRF_CHART_HDRS fontproperties.h config.h chartcomponent.h axis.h plot.h
      plotline.h legend.h chartpanel.h chartcontainer.h chart.h)
  SET(GRF_CHART_SRCS fontproperties.c config.c chartcomponent.c axis.c plot.c
      plotline.c legend.c chartpanel.c chartcontainer.c chart.c)
  GRF_CREATE_LIBRARY(chart CHART)
ENDIF()
#================================================================ DOCUMENTATION
find_package(Doxygen)
if(Doxygen_FOUND)
    if(BUILD_DOCS)
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc/)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                       ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile @ONLY)
        add_custom_target(doc ALL
          ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile
          WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
          COMMENT "Generating API documentation with Doxygen" VERBATIM
        )
        add_custom_command(TARGET doc PRE_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/tutorials
            ${CMAKE_CURRENT_SOURCE_DIR}/doc)
        SET_DIRECTORY_PROPERTIES(PROPERTIES
          ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/doc/*")
        Message(STATUS "Documentation generation configured...")
    endif(BUILD_DOCS)
endif(Doxygen_FOUND)
#=================================================================== DEBUG INFO
if(DEBUG_INFO)
    SET(GRF_COMPILE_TYPE Debug)
else()
    SET(GRF_COMPILE_TYPE Release)
endif(DEBUG_INFO)
SET(CMAKE_BUILD_TYPE ${GRF_COMPILE_TYPE} CACHE STRING
        "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or
        CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
Message(STATUS "Compilation mode: ${GRF_COMPILE_TYPE}")
#======================================================================== TESTS
if(BUILD_TESTING)
    ENABLE_TESTING()
    # profiling directives (GCov and GProf)
    ADD_DEFINITIONS(-fprofile-arcs -ftest-coverage -pg)

    # Setup coveralls
    IF(COVERALLS)
      coveralls_setup("${GRF_SRCS}" ${COVERALLS_UPLOAD} "${CMAKE_SOURCE_DIR}/cmake/")
    ENDIF()

    # Array
    LIST(APPEND TESTS_NAMES
      array
      image
      list
      slist
      queue
      pqueue
      ift
      drawing
      hal
    )
    IF(BUILD_DISPLAY)
      LIST(APPEND TESTS_NAMES imagewidget)
      LIST(APPEND TESTS_NAMES chartwidget)
      LIST(APPEND TESTS_NAMES display)
      LIST(APPEND TESTS_NAMES displaywindow)
      set(test_imagewidget_deps   grafeo_core grafeo_imgproc grafeo_ift grafeo_display grafeo_chart ${goocanvas_LIBRARIES} png jpeg ${GTK3_LIBRARIES})
      set(test_chartwidget_deps   grafeo_core grafeo_imgproc grafeo_ift grafeo_display grafeo_chart ${goocanvas_LIBRARIES} png jpeg ${GTK3_LIBRARIES})
      set(test_display_deps       grafeo_core grafeo_imgproc grafeo_ift grafeo_display grafeo_chart ${goocanvas_LIBRARIES} png jpeg ${GTK3_LIBRARIES})
      set(test_displaywindow_deps grafeo_core grafeo_imgproc grafeo_ift grafeo_display grafeo_chart ${goocanvas_LIBRARIES} png jpeg ${GTK3_LIBRARIES})
    endif(BUILD_DISPLAY)
    set(test_array_deps   grafeo_core)
    set(test_image_deps   grafeo_core grafeo_imgproc png jpeg)
    set(test_list_deps    grafeo_core)
    set(test_slist_deps   grafeo_core)
    set(test_queue_deps   grafeo_core)
    set(test_pqueue_deps  grafeo_core)
    set(test_ift_deps     grafeo_core grafeo_imgproc grafeo_ift png jpeg)
    set(test_drawing_deps grafeo_core grafeo_imgproc png jpeg)
    set(test_hal_deps     grafeo_core)
    if(BUILD_CHART)
      LIST(APPEND TESTS_NAMES axis)
      LIST(APPEND TESTS_NAMES plot)
      LIST(APPEND TESTS_NAMES chartpanel)
      LIST(APPEND TESTS_NAMES chartcontainer)
      LIST(APPEND TESTS_NAMES chart)
      set(test_axis_deps            grafeo_core grafeo_chart ${gobject_LIBRARIES})
      set(test_plot_deps            grafeo_core grafeo_chart ${gobject_LIBRARIES})
      set(test_chartpanel_deps      grafeo_core grafeo_chart ${gobject_LIBRARIES})
      set(test_chartcontainer_deps  grafeo_core grafeo_chart ${gobject_LIBRARIES})
      set(test_chart_deps           grafeo_core grafeo_chart ${gobject_LIBRARIES})
    endif(BUILD_CHART)
    
    include (CTest)
    list(LENGTH TESTS_NAMES len1)

    math(EXPR len2 "${len1}-1")

    foreach(val RANGE ${len2})
      list(GET TESTS_NAMES ${val} testname)
      set(test_deps test_${testname}_deps)
      
      if(NOT CMOCKA_FOUND)
          include_directories(${EXTERNAL_INCLUDE_DIR})
          #link_directories(${CMAKE_SOURCE_DIR}/external/lib)
          add_executable       (test_${testname} tests/test_${testname}.c)
          
          target_link_libraries(test_${testname} ${${test_deps}} cmocka m)
          add_dependencies     (test_${testname} cmockaDownload)
          #target_link_libraries(test_array grafeo cmocka m)
      else()
          add_executable       (test_${testname} tests/test_${testname}.c)
          target_link_libraries(test_${testname} ${${test_deps}} cmocka m)
      endif(NOT CMOCKA_FOUND)
      add_test             (test_${testname}       test_${testname})
    endforeach()
    Message(STATUS "Tests compilation configured...")
endif(BUILD_TESTING)

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
option(BUILD_GL      "OpenGL engine (Requires epoxy)" ON)
option(BUILD_IFT     "Image Foresting Transform methods" ON)
option(BUILD_CHART   "Component for creating charts" OFF)
option(BUILD_MEDICAL "Component for medical imaging solutions" ON)
option(BUILD_TESTING "" ON)
option(BUILD_DOCS    "Build doxygen documentation" OFF)
option(COVERALLS     "Generate coveralls data" OFF)
option(COVERALLS_UPLOAD "Upload the generated coveralls json" ON)
option(DEBUG_INFO    "Compile Grafeo with debug information?" ON)

#================================================== CUSTOM FUNCTIONS AND MACROS
include(cmake/grfmacros.cmake)
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
    add_definitions(-DHAVE_PNG)
endif()
# ------------------
# JPG
# ------------------
if(BUILD_IMGPROC OR BUILD_DISPLAY)
    find_package(JPEG QUIET)
    if(NOT JPEG_FOUND)

    endif(NOT JPEG_FOUND)
    add_definitions(-DHAVE_JPEG)
endif()
# ------------------
# Zlib
# ------------------
if(BUILD_MEDICAL)
    pkg_check_modules(ZLIB REQUIRED zlib)
    INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIRS})
    ADD_DEFINITIONS(-DHAVE_ZLIB)
endif()
# ------------------
# GObject
# ------------------
IF(BUILD_CHART OR BUILD_GL)
  pkg_check_modules(GOBJECT REQUIRED gobject-2.0)
  INCLUDE_DIRECTORIES(${GOBJECT_INCLUDE_DIRS})
  add_definitions(-DHAVE_GOBJECT)
ENDIF()
# ------------------
# GTK
# ------------------
IF(BUILD_DISPLAY)
  pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
  pkg_check_modules(GOOCANVAS REQUIRED goocanvas-2.0)
  INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS} ${GOOCANVAS_INCLUDE_DIRS})
  add_definitions(-DHAVE_GTK3)
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
  set(CMAKE_EXE_LINKER_FLAGS "-g -Wall -std=gnu99 -Wshadow -Wunused-variable -Wno-unused-function -Wunused -Wno-system-headers -pg -fprofile-arcs -ftest-coverage")
  set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${CMAKE_EXE_LINKER_FLAGS})
  Message(STATUS "Tests parameters setup...")
ENDIF()
Message(STATUS "Third party libraries loaded...")
#====================================================================== LIBRARY
# Setting some constants for all modules
SET(GRF_INCLUDE_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/include/grafeo)
SET(GRF_SOURCE_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/src)
SET(GRF_MODULES_LOWER  core display ift imgproc chart medical gl)
SET(GRF_MODULES_UPPER  CORE DISPLAY IFT IMGPROC CHART MEDICAL GL)
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
set(GRF_INSTALL_PATH "\"${CMAKE_INSTALL_PREFIX}\"")
set(GRF_RESOURCE_PATH "\"${CMAKE_INSTALL_PREFIX}/share/grafeo\"")
Message(STATUS "Library parameters setup...")
#================================================================== CORE MODULE
# Even though BUILD_CORE is OFF, if for example BUILD_IMGPROC is ON,
# the CORE module will be compiled
IF(BUILD_CORE OR BUILD_IMGPROC OR BUILD_IFT OR BUILD_DISPLAY)
  SET(GRF_CORE_HDRS     config.h i18n-lib.h  macro.h  type.h  hal.h  range.h
                         slist.h     list.h  array.h vec3.h vec4.h vec6.h vec7.h mat4.h
                        queue.h bucket.h pqueue.h)
  SET(GRF_CORE_SRCS       type.c config.c  range.c slist.c list.c array.c vec3.c
                         vec4.c vec6.c vec7.h mat4.c queue.c bucket.c pqueue.c)
  SET(GRF_CORE_TESTS    array vec3 vec4 vec6 vec7 mat4 list slist queue pqueue hal)
  SET(GRF_CORE_LIBRARIES grafeo_core m)
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
  SET(GRF_IMGPROC_TESTS image    drawing)
  SET(GRF_IMGPROC_LIBRARIES ${GRF_CORE_LIBRARIES} grafeo_imgproc jpeg png)
  GRF_CREATE_LIBRARY(imgproc IMGPROC)
ENDIF()
#=================================================================== IFT MODULE
IF(BUILD_IFT)
  SET(GRF_IFT_HDRS  ift.h)
  SET(GRF_IFT_SRCS  ift.c)
  SET(GRF_IFT_TESTS ift)
  SET(GRF_IFT_LIBRARIES ${GRF_CORE_LIBRARIES} ${GRF_IMGPROC_LIBRARIES} grafeo_ift)
  GRF_CREATE_LIBRARY(ift IFT)  
ENDIF()
#==================================================================== GL MODULE
IF(BUILD_GL)
  SET(GRF_GL_HDRS  glshader.h glprogram.h glmaterial.h gllight.h
                   globject.h glcontainer.h glmesh.h glloader.h glloader_obj.h
                   glloader_dae.h glloader_3ds.h glloader_x3d.h glloader_md3.h
                   glscene.h glprojection.h glprojection_perspective.h
                   glprojection_orthographic.h glcamera.h glrenderer.h)
  SET(GRF_GL_SRCS  glshader.c glprogram.c glmaterial.c gllight.c
                   globject.c glcontainer.c glmesh.c glloader.c glloader_obj.c
                   glloader_dae.c glloader_3ds.c glloader_x3d.c glloader_md3.c
                   glscene.c glprojection.c glprojection_perspective.c
                   glprojection_orthographic.c glcamera.c glrenderer.c)
  SET(GRF_GL_TESTS glshader glprogram glmaterial gllight
                   globject glcontainer glmesh glloader glloader_obj
                   glloader_dae glloader_3ds glloader_x3d glloader_md3
                   glscene glprojection glprojection_perspective
                   glprojection_orthographic glcamera glrenderer)
  SET(GRF_GL_LIBRARIES ${GRF_CORE_LIBRARIES} grafeo_gl ${GOBJECT_LIBRARIES})
  GRF_CREATE_LIBRARY(gl GL)
ENDIF()
#=============================================================== DISPLAY MODULE
IF(BUILD_DISPLAY)
  SET(GRF_DISPLAY_HDRS glshader.h glprogram.h idisplaywidget.h imagewidget.h
                       chartwidget.h trackbar.h displaywindow.h display.h)
  SET(GRF_DISPLAY_SRCS glshader.c glprogram.c idisplaywidget.c imagewidget.c
                       chartwidget.c trackbar.c displaywindow.c display.c)
  SET(GRF_DISPLAY_TESTS imagewidget chartwidget trackbar displaywindow display)
  SET(GRF_DISPLAY_LIBRARIES ${GRF_CORE_LIBRARIES} ${GRF_IMGPROC_LIBRARIES} grafeo_display ${GTK3_LIBRARIES} ${GOOCANVAS_LIBRARIES})
  GRF_CREATE_LIBRARY(display DISPLAY)
ENDIF()
#=============================================================== MEDICAL MODULE
IF(BUILD_MEDICAL)
  SET(GRF_MEDICAL_HDRS  znzlib.h dicom.h nifti1.h nifti1_io.h)
  SET(GRF_MEDICAL_SRCS  znzlib.c dicom.c nifti1_io.c)
  SET(GRF_MEDICAL_TESTS dicom nifti)
  SET(GRF_MEDICAL_LIBRARIES ${GRF_CORE_LIBRARIES} ${GRF_DISPLAY_LIBRARIES} grafeo_medical ${ZLIB_LIBRARIES})
  GRF_CREATE_LIBRARY(medical MEDICAL)
ENDIF()
#================================================================= CHART MODULE
IF(BUILD_CHART)
  SET(GRF_CHART_HDRS fontproperties.h config.h chartcomponent.h axis.h plot.h
      plotline.h legend.h chartpanel.h chartcontainer.h chart.h)
  SET(GRF_CHART_SRCS fontproperties.c config.c chartcomponent.c axis.c plot.c
      plotline.c legend.c chartpanel.c chartcontainer.c chart.c)
  SET(GRF_CHART_TESTS fontproperties config chartcomponent axis plot plotline
      legend chartpanel chartcontainer chart)
  SET(GRF_CHART_LIBRARIES ${GRF_CORE_LIBRARIES} grafeo_chart ${GOBJECT_LIBRARIES})
  GRF_CREATE_LIBRARY(chart CHART)
ENDIF()
#==================================================================== RESOURCES
INSTALL(DIRECTORY data DESTINATION share/grafeo)
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

    FOREACH(GRF_MODULE ${GRF_MODULES_LOWER})
      GRF_CREATE_TESTS(${GRF_MODULE})
    ENDFOREACH()
    Message(STATUS "Tests compilation configured...")
endif(BUILD_TESTING)

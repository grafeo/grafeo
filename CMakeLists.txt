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
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    "${CMAKE_SOURCE_DIR}/cmake/")

set(EXTERNAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/include)
set(EXTERNAL_LIB_DIR ${CMAKE_SOURCE_DIR}/external/lib)

function (load_submodule nome nome_lib)
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
        UPDATE_COMMAND git submodule update --init --recursive
        DOWNLOAD_COMMAND git submodule update --init --recursive
    )
    
    add_library(${nome_lib} SHARED IMPORTED)
    set_target_properties(${nome_lib} PROPERTIES IMPORTED_LOCATION ${EXTERNAL_LIB_DIR}/lib${nome_lib}.so)
endfunction()
#====================================================== DOCUMENTATION
find_package(Doxygen)
if(Doxygen_FOUND)
    option(BUILD_DOCS      "Build doxygen documentation" ON)
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
    endif(BUILD_DOCS)
endif(Doxygen_FOUND)
#================================================================ PNG
find_package(PNG QUIET)
if(NOT PNG_FOUND)
    load_submodule(PNG png)
endif(NOT PNG_FOUND)
#================================================================ JPG
find_package(JPEG QUIET)
if(NOT JPEG_FOUND)

endif(NOT JPEG_FOUND)

#============================================= TESTS (BEFORE LIBRARY)
include(CTest)
option(COVERALLS "Generate coveralls data" OFF)
option(COVERALLS_UPLOAD "Upload the generated coveralls json" ON)
if(BUILD_TESTING)
    if (COVERALLS)
        include(Coveralls)
    endif(COVERALLS)
    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-g -Wall -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -pg -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "-g -Wall -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -pg -fprofile-arcs -ftest-coverage")
endif(BUILD_TESTING)
#============================================================ LIBRARY
include_directories(include)
set(GRAFEO_HDR
    include/grafeo/ift.h
    include/grafeo/pqueue.h
    include/grafeo/bucket.h
    include/grafeo/slist.h
    include/grafeo/list.h
    include/grafeo/type.h
    include/grafeo/array.h
    include/grafeo/queue.h
    include/grafeo/range.h
    include/grafeo/image.h
)
set(GRAFEO_SRC
 src/ift.c
 src/pqueue.c
 src/bucket.c
 src/slist.c
 src/array.c
 src/list.c
 src/queue.c
 src/range.c
 src/image.c)
add_library(grafeo SHARED ${GRAFEO_SRC} ${GRAFEO_HDR})
install(FILES ${GRAFEO_HDR} DESTINATION include/grafeo)
install(TARGETS grafeo LIBRARY DESTINATION lib)

#========================================================= DEBUG INFO
option(DEBUG_INFO "Compile Grafeo with debug information?" ON)
if(DEBUG_INFO)
    SET(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or 
        CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
else()
    SET(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or 
        CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
endif(DEBUG_INFO)

#============================================================== TESTS
if(BUILD_TESTING)
    # find cmocka library
    INCLUDE(ExternalProject)
    if (COVERALLS)
        set(COVERALLS_REPO_TOKEN "jcNqZoqi87ip0EoPWLy5b8nzfFm4Z0MlL")
        coveralls_setup(
            "${GRAFEO_SRC}" 
            ${COVERALLS_UPLOAD}
            "${CMAKE_SOURCE_DIR}/cmake/")
    endif(COVERALLS)

    find_package(CMocka QUIET)

    if(NOT CMOCKA_FOUND)
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
    endif(NOT CMOCKA_FOUND)

    enable_testing()
    # profiling directives (GCov and GProf)
    add_definitions(-fprofile-arcs -ftest-coverage -pg)

    # Array
    list(APPEND TESTS_NAMES
      array
      image
      list
      slist
      queue
      pqueue
      ift
    )
    include (CTest)
    foreach(testname ${TESTS_NAMES})
        if(NOT CMOCKA_FOUND)
            include_directories(${EXTERNAL_INCLUDE_DIR})
            #link_directories(${CMAKE_SOURCE_DIR}/external/lib)
            add_executable       (test_${testname} tests/test_${testname}.c)
            target_link_libraries(test_${testname} grafeo cmocka png jpeg m)
            add_dependencies     (test_${testname} cmockaDownload)
            #target_link_libraries(test_array grafeo cmocka m)
        else()
            add_executable       (test_${testname} tests/test_${testname}.c)
            target_link_libraries(test_${testname} grafeo cmocka png jpeg m)
        endif(NOT CMOCKA_FOUND)
        add_test             (test_${testname}       test_${testname})
    endforeach()    
endif(BUILD_TESTING)

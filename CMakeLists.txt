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

#====================================================== DOCUMENTATION
find_package(Doxygen)
if(Doxygen_FOUND)
	option(BUILD_DOCS      "Build doxygen documentation" ON)
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
endif(Doxygen_FOUND)
#============================================================ LIBRARY
set(GRAFEO_HDR
	include/grafeo/list.h
	include/grafeo/array.h
	include/grafeo/queue.h
)
set(GRAFEO_SRC
 src/array.c
 src/list.c
 src/queue.c)
add_library(grafeo SHARED ${GRAFEO_SRC})
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
option(BUILD_TESTS "CMocka tests with GCov and GProf support" ON)
if(BUILD_TESTS)
	enable_testing()
	set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}
	    "-pg -fprofile-arcs -ftest-coverage")

	# Array
	add_executable       (test_array tests/test_array.c)
	add_test             (test_array       test_array)
	target_link_libraries(test_array grafeo cmocka m)
endif(BUILD_TESTS)
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
macro(GRF_USE_C99)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_C_FLAGS "--std=gnu99 ${CMAKE_C_FLAGS}")
    endif ()
  else ()
    set (CMAKE_C_STANDARD 99)
  endif ()
endmacro(GRF_USE_C99)
macro(GRF_CREATE_TESTS module)
  string(TOUPPER ${module} MODULEU)
  GRF_USE_C99()
  IF(BUILD_${MODULEU})
    FOREACH(TEST ${GRF_${MODULEU}_TESTS})
      add_executable       (test_${TEST} tests/${module}/test_${TEST}.c)
      target_link_libraries(test_${TEST} ${GRF_${MODULEU}_LIBRARIES} cmocka)
      add_test             (test_${TEST} test_${TEST})
    ENDFOREACH()
  ENDIF()
endmacro()
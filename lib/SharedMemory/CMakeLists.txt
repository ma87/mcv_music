cmake_minimum_required(VERSION 2.8)

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

## Export shared memory library to python using Swig
SET(MCV_WEB_SERVER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../webserver")
SET(CMAKE_SWIG_FLAGS "")
SET(CMAKE_SWIG_OUTDIR ${MCV_WEB_SERVER_PATH})

# Set swig input files
SET_SOURCE_FILES_PROPERTIES(mcv_shared_memory.i PROPERTIES C ON)
set_property(SOURCE mcv_shared_memory.i PROPERTY SWIG_MODULE_NAME mcv_shared_memory)

# Configure swig process
SWIG_ADD_LIBRARY(mcv_shared_memory TYPE SHARED LANGUAGE python SOURCES mcv_shared_memory.i shared_memory.c property.c)
SWIG_LINK_LIBRARIES(mcv_shared_memory ${PYTHON_LIBRARIES})

# Copy shared object file produced by swig
add_custom_command(TARGET _mcv_shared_memory POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:_mcv_shared_memory> ${MCV_WEB_SERVER_PATH}
)


## Export shared memory library for another targets in project
ADD_LIBRARY (shared_memory shared_memory.c property.c)

cmake_minimum_required(VERSION 2.8)

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(CMAKE_SWIG_FLAGS "")

SET_SOURCE_FILES_PROPERTIES(mcv_shared_memory.i PROPERTIES C ON)
set_property(SOURCE mcv_shared_memory.i PROPERTY SWIG_MODULE_NAME mcv_shared_memory)
SWIG_ADD_LIBRARY(mcv_shared_memory TYPE SHARED LANGUAGE python SOURCES mcv_shared_memory.i shared_memory.c property.c)
SWIG_LINK_LIBRARIES(mcv_shared_memory ${PYTHON_LIBRARIES})

ADD_LIBRARY (shared_memory shared_memory.c property.c)

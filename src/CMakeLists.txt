cmake_minimum_required(VERSION 2.8)

file(
		GLOB
		plugins_files
		./plugins/*.cpp
)

file(
		GLOB
		tools_files
		./tools/*.c
)

file(
    GLOB
    jack_source_files
    ./jack_source/*.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/plugins)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/tools)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../lib/SharedMemory)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../lib/freeverb3-3.1.1/)
LINK_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/../bin)

ADD_LIBRARY (mcv_plugins STATIC ${plugins_files} ${tools_files})

ADD_EXECUTABLE (mcv_jack_sources ${jack_source_files})
TARGET_LINK_LIBRARIES(mcv_jack_sources jack mcv_plugins shared_memory pthread freeverb3)

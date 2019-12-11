cmake_minimum_required(VERSION 3.12)
include(ExternalProject)
project(pb_battleroyale)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

find_package(SFML COMPONENTS graphics window system audio)

include_directories(
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/thirdparty/angelscript/sdk/angelscript/include
	${CMAKE_SOURCE_DIR}/thirdparty/angelscript/sdk/add_on
)

set(source ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/angelscript/sdk/angelscript/projects/cmake/)
add_executable(pb_battleroyale
	${source}/app.cpp
	${source}/helpers.cpp
	${source}/ini.c
	${source}/main.cpp
	${source}/spdlog.cpp
	${source}/texturepacker.cpp

	${source}/handlers/mainhandler.cpp

	${source}/managers/modmanager.cpp
	${source}/managers/musicmanager.cpp
	${source}/managers/obstaclemanager.cpp
	${source}/managers/texturemanager.cpp

	${source}/map/location.cpp
	${source}/map/mapmanager.cpp

	${source}/mod/mod.cpp
	${source}/mod/reg_stdlib.cpp

	${CMAKE_SOURCE_DIR}/thirdparty/angelscript/sdk/add_on/scriptbuilder/scriptbuilder.cpp
	${CMAKE_SOURCE_DIR}/thirdparty/angelscript/sdk/add_on/scriptstdstring/scriptstdstring.cpp
	${CMAKE_SOURCE_DIR}/thirdparty/angelscript/sdk/add_on/scriptstdstring/scriptstdstring_utils.cpp
	${CMAKE_SOURCE_DIR}/thirdparty/angelscript/sdk/add_on/scriptarray/scriptarray.cpp
	${CMAKE_SOURCE_DIR}/thirdparty/angelscript/sdk/add_on/scriptdictionary/scriptdictionary.cpp
	${CMAKE_SOURCE_DIR}/thirdparty/angelscript/sdk/add_on/scriptmath/scriptmath.cpp
	${CMAKE_SOURCE_DIR}/thirdparty/angelscript/sdk/add_on/datetime/datetime.cpp
)

add_definitions(-DSPDLOG_COMPILED_LIB)

target_link_libraries(pb_battleroyale sfml-graphics sfml-window sfml-system sfml-audio)
target_link_libraries(pb_battleroyale ${ANGELSCRIPT_LIBRARY_NAME})

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
target_link_libraries(pb_battleroyale pthread)
endif()

cmake_minimum_required(VERSION 3.12)
include(ExternalProject)
project(pb_battleroyale)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -DSPDLOG_COMPILED_LIB")

include_directories(
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/thirdparty/angelscript/angelscript/include
	${CMAKE_SOURCE_DIR}/thirdparty/angelscript/add_on
)

set(source ${CMAKE_SOURCE_DIR}/src)

ExternalProject_Add(
	angelscript

	URL https://www.angelcode.com/angelscript/sdk/files/angelscript_2.34.0.zip
	URL_MD5 c62a482912d23f5b6d290190303fc3f4

	SOURCE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/angelscript"
	SOURCE_SUBDIR "angelscript/projects/cmake"

	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	INSTALL_COMMAND ""

	BUILD_IN_SOURCE True
)

add_executable(pb_battleroyale
	${source}/app.cpp
	${source}/helpers.cpp
	${source}/ini.c
	${source}/main.cpp
	${source}/spdlog.cpp
	${source}/texturepacker.cpp

	${source}/handlers/mainhandler.cpp

	${source}/managers/musicmanager.cpp
	${source}/managers/obstaclemanager.cpp
	${source}/managers/texturemanager.cpp

	${source}/map/location.cpp
	${source}/map/mapmanager.cpp

	${CMAKE_SOURCE_DIR}/thirdparty/angelscript/add_on/scriptbuilder/scriptbuilder.cpp
	${CMAKE_SOURCE_DIR}/thirdparty/angelscript/add_on/scriptstdstring/scriptstdstring.cpp
	${CMAKE_SOURCE_DIR}/thirdparty/angelscript/add_on/scriptstdstring/scriptstdstring_utils.cpp
	${CMAKE_SOURCE_DIR}/thirdparty/angelscript/add_on/scriptarray/scriptarray.cpp
)

target_link_libraries(pb_battleroyale pthread sfml-graphics sfml-window sfml-system sfml-audio)
target_link_libraries(pb_battleroyale "${CMAKE_SOURCE_DIR}/thirdparty/angelscript/angelscript/projects/cmake/libangelscript.a")
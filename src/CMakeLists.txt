cmake_minimum_required(VERSION 2.8)

project(openvr_api)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	add_definitions(-DOSX -DPOSIX)
	set_source_files_properties(vrcommon/pathtools_public.cpp vrcommon/vrpathregistry_public.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(. ../headers)
add_library(openvr_api STATIC openvr_api_public.cpp jsoncpp.cpp vrcommon/dirtools_public.cpp vrcommon/envvartools_public.cpp vrcommon/pathtools_public.cpp vrcommon/sharedlibtools_public.cpp vrcommon/hmderrors_public.cpp vrcommon/vrpathregistry_public.cpp vrcommon/strtools_public.cpp vrcommon/vrpathregistry_public.cpp)

install(TARGETS openvr_api DESTINATION lib)
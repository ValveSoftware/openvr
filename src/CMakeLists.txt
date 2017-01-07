# Project name.
project(openvr_api)

# Set some properies for specific files.
if(APPLE)
  if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set_source_files_properties(vrcommon/pathtools_public.cpp vrcommon/vrpathregistry_public.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
  endif()
endif()

# Add include folders.
include_directories(${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/headers)

if(USE_CUSTOM_LIBCXX)
	link_directories(
		${LIBCXX_LIB_DIR}
	)
endif()

# Set the source group and files.
set(CORE_FILES
	openvr_api_public.cpp
	jsoncpp.cpp
)
set(VRCOMMON_FILES
	vrcommon/dirtools_public.cpp
	vrcommon/envvartools_public.cpp
	vrcommon/pathtools_public.cpp
	vrcommon/sharedlibtools_public.cpp
	vrcommon/hmderrors_public.cpp
	vrcommon/vrpathregistry_public.cpp
	vrcommon/strtools_public.cpp
	vrcommon/vrpathregistry_public.cpp
)

set(SOURCE_FILES
	${CORE_FILES}
	${VRCOMMON_FILES}
)

source_group("Src" FILES
	${CORE_FILES}
)

source_group("VRCommon" FILES
	${VRCOMMON_FILES}
)

# Build the library.
if(BUILD_SHARED)
	add_library(openvr_api SHARED ${SOURCE_FILES})
else()
	add_library(openvr_api STATIC ${SOURCE_FILES})
endif()

if(USE_CUSTOM_LIBCXX)
	target_link_libraries( openvr_api c++ c++abi)
endif()

install(TARGETS openvr_api DESTINATION lib)

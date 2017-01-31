# Project name.
project(openvr_api)

set( LIBNAME "openvr_api" )

# Set some properies for specific files.
if(APPLE)
  set(CMAKE_MACOSX_RPATH 0)
  if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set_source_files_properties(vrcommon/pathtools_public.cpp vrcommon/vrpathregistry_public.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
  endif()
  if(BUILD_SHARED)
    find_library(FOUNDATION_FRAMEWORK Foundation)
    mark_as_advanced(FOUNDATION_FRAMEWORK)
    set(EXTRA_LIBS ${EXTRA_LIBS} ${FOUNDATION_FRAMEWORK})
  endif(BUILD_SHARED)
elseif(WIN32)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    add_definitions( -DWIN64 )
    set( LIBNAME "openvr_api64" )
  endif()
endif()

# Add include folders.
include_directories(${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/headers ${CMAKE_SOURCE_DIR}/src/vrcommon)

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
	add_library(${LIBNAME} SHARED ${SOURCE_FILES})
else()
	add_library(${LIBNAME} STATIC ${SOURCE_FILES})
endif()

if(USE_CUSTOM_LIBCXX)
	set(EXTRA_LIBS ${EXTRA_LIBS} c++ c++abi)
endif()

target_link_libraries(${LIBNAME} ${EXTRA_LIBS})

install(TARGETS ${LIBNAME} DESTINATION lib)

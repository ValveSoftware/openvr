# Set the minimum required version of CMake for this project.
cmake_minimum_required(VERSION 2.8)

# Set project name.
project(OpenVRSDK)

# Setup some options.
option(BUILD_SHARED "Builds the library as shared library" OFF)
option(BUILD_FRAMEWORK "Builds the library as an apple Framework" OFF)
option(BUILD_UNIVERSAL "Builds the shared or framework as a universal (fat, 32- & 64-bit) binary" ON)
option(BUILD_OSX_I386 "Builds the shared or framework as a 32-bit binary, even on a 64-bit platform" OFF)
option(USE_LIBCXX "Uses libc++ instead of libstdc++" ON)
option(USE_CUSTOM_LIBCXX "Uses a custom libc++" OFF)

add_definitions( -DVR_API_PUBLIC )

# Check if 32 or 64 bit system.
set(SIZEOF_VOIDP ${CMAKE_SIZEOF_VOID_P})
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(PROCESSOR_ARCH "64")
else()
  set(PROCESSOR_ARCH "32")
endif()

# Get platform.
if(WIN32)
	set(PLATFORM_NAME "win")
elseif(UNIX AND NOT APPLE)
  if(CMAKE_SYSTEM_NAME MATCHES ".*Linux")
    set(PLATFORM_NAME "linux")
    add_definitions(-DLINUX -DPOSIX)
    if(PROCESSOR_ARCH MATCHES "64")
        add_definitions(-DLINUX64)
    endif()
  endif()
elseif(APPLE)
  if(CMAKE_SYSTEM_NAME MATCHES ".*Darwin.*" OR CMAKE_SYSTEM_NAME MATCHES ".*MacOS.*")
    set(PLATFORM_NAME "osx")
    add_definitions(-DOSX -DPOSIX)
    if(BUILD_UNIVERSAL)
      set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")
    endif()
    if(BUILD_OSX_I386)
      set(PROCESSOR_ARCH "32")
      set(CMAKE_OSX_ARCHITECTURES "i386")
    endif()
  endif()
endif()

# Set output folder for static and shared libraries
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PLATFORM_NAME}${PROCESSOR_ARCH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PLATFORM_NAME}${PROCESSOR_ARCH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PLATFORM_NAME}${PROCESSOR_ARCH})

# Enable some properties.
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
	# Enable c++11 and hide symbols which shouldn't be visible
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -fvisibility=hidden")

	# Set custom libc++ usage here
	if(CMAKE_C_COMPILER_ID MATCHES "Clang" AND USE_LIBCXX)
		if(USE_CUSTOM_LIBCXX)
			if(BUILD_SHARED)
				set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
			endif()
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdinc++")
			include_directories( ${LIBCXX_INCLUDE} ${LIBCXX_ABI_INCLUDE})
			message(STATUS "Using custom libc++")
		else()
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
			message(STATUS "Using libc++")
		endif()
	endif()
endif()

add_subdirectory(src)

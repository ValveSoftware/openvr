cmake_minimum_required(VERSION 3.7.1)

project(openvr_samples)

# For your project, this might look something like:
# set(OPENVR_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/openvr)

# But we have the binaries we need in this repo, so link those:
get_filename_component(OPENVR_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}../../.. ABSOLUTE)

# If not set, determine the platform architecture
if(NOT PLATFORM)
  if(CMAKE_SIZEOF_VOID_P MATCHES 8)
    set(PLATFORM 64)
  else()
    set(PLATFORM 32)
  endif()
endif()
message(STATUS "Compilation set for ${PLATFORM}bits architectures.")

# OpenVR compatibility checking
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  add_definitions(-DLINUX -DPOSIX)
  set(ARCH_TARGET linux64)

  if(${PLATFORM} MATCHES 32)
    message(WARNING "OpenVR x86 binaries not provided on GNU/Linux.")
  endif()
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_MACOSX_RPATH 0)
  add_definitions(-DOSX -DPOSIX)
  set(ARCH_TARGET osx32)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  add_definitions(-D_WIN32)
  set(ARCH_TARGET win${PLATFORM})

  # Binaries path for thirdparties are not generics so we try to guess their suffixes.
  set(WINDOWS_PATH_SUFFIXES win${PLATFORM} Win${PLATFORM} x${PLATFORM})
endif()

find_library(OPENVR_LIBRARIES
  NAMES
    openvr_api
  PATHS
    ${OPENVR_LIB_DIR}/bin
    ${OPENVR_LIB_DIR}/lib
  PATH_SUFFIXES
    osx${PLATFORM}
    linux${PLATFORM}
    win${PLATFORM}
  NO_DEFAULT_PATH
  NO_CMAKE_FIND_ROOT_PATH
)

set(OPENVR_INCLUDE_DIR ${OPENVR_LIB_DIR}/headers)

add_subdirectory(utils)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/output/drivers")
add_subdirectory(drivers)
set(TARGET_NAME hellovr_vulkan)

if(APPLE)
  if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set_source_files_properties(../shared/pathtools.cpp hellovr_vulkan_main.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
    find_library(FOUNDATION_FRAMEWORK Foundation)
    mark_as_advanced(FOUNDATION_FRAMEWORK)
    set(EXTRA_LIBS ${EXTRA_LIBS} ${FOUNDATION_FRAMEWORK})

    find_library(APPKIT_FRAMEWORK AppKit)
    mark_as_advanced(APPKIT_FRAMEWORK)
    set(EXTRA_LIBS ${EXTRA_LIBS} ${APPKIT_FRAMEWORK})

	# Create a Metal compatible view
	file(GLOB COCOA_METAL_VIEW_FILES
	  osx/SDL_cocoametalview.h
	  osx/SDL_cocoametalview.m
	)

  endif()
endif()

add_executable(${TARGET_NAME}
  ${SHARED_SRC_FILES}
  ${COCOA_METAL_VIEW_FILES}
  hellovr_vulkan_main.cpp
)

target_link_libraries(${TARGET_NAME}
  ${VULKAN_LIBRARY}
  ${SDL2_LIBRARIES}
  ${OPENVR_LIBRARIES}
  ${CMAKE_DL_LIBS}
  ${EXTRA_LIBS}
)

setTargetOutputDirectory(${TARGET_NAME})

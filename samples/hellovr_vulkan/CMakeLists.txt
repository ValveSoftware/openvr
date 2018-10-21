set(TARGET_NAME hellovr_vulkan)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  add_compile_options(-x objective-c++)

  find_library(FOUNDATION_FRAMEWORK Foundation)
  mark_as_advanced(FOUNDATION_FRAMEWORK)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${FOUNDATION_FRAMEWORK})

  find_library(APPKIT_FRAMEWORK AppKit)
  mark_as_advanced(APPKIT_FRAMEWORK)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${APPKIT_FRAMEWORK})

  find_library(METAL_FRAMEWORK Metal)
  mark_as_advanced(METAL_FRAMEWORK)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${METAL_FRAMEWORK})

  find_library(IOSURFACE_FRAMEWORK IOSurface)
  mark_as_advanced(IOSURFACE_FRAMEWORK)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${IOSURFACE_FRAMEWORK})

  find_library(QUARTZCORE_FRAMEWORK QuartzCore)
  mark_as_advanced(QUARTZCORE_FRAMEWORK)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${QUARTZCORE_FRAMEWORK})

  add_executable(${TARGET_NAME}
    ${SHARED_SRC_FILES}
    osx/SDL_cocoametalview.h
    osx/SDL_cocoametalview.m
    hellovr_vulkan_main.cpp
  )

else()
  add_executable(${TARGET_NAME}
    ${SHARED_SRC_FILES}
    hellovr_vulkan_main.cpp
  )
endif()


target_link_libraries(${TARGET_NAME}
  ${VULKAN_LIBRARY}
  ${SDL2_LIBRARIES}
  ${OPENVR_LIBRARIES}
  ${CMAKE_DL_LIBS}
  ${EXTRA_LIBS}
)

setTargetOutputDirectory(${TARGET_NAME})

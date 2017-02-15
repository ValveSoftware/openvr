set(TARGET_NAME tracked_camera_openvr_sample)

if(APPLE)
  if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set_source_files_properties(../shared/pathtools.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
    find_library(FOUNDATION_FRAMEWORK Foundation)
    mark_as_advanced(FOUNDATION_FRAMEWORK)
    set(EXTRA_LIBS ${EXTRA_LIBS} ${FOUNDATION_FRAMEWORK})
  endif()
endif()

add_executable(${TARGET_NAME}
  ${SHARED_SRC_FILES}
  main.cpp
  tracked_camera_openvr_sample.cpp
  tracked_camera_openvr_sample.h
)

add_definitions(${QT_DEFINITIONS})
if(NOT "${CMAKE_BUILD_TYPE}" EQUAL "Debug")
    add_definitions(-DQT_NO_DEBUG)
endif()

target_link_libraries(${TARGET_NAME}
  ${QT_LIBRARIES}
  ${OPENVR_LIBRARIES}
  ${CMAKE_DL_LIBS}
  ${EXTRA_LIBS}
)

set_target_properties(${TARGET_NAME}
  PROPERTIES
  COMPILE_FLAGS "${QT_FLAGS}"
)

setTargetOutputDirectory(${TARGET_NAME})
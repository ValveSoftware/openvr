set(TARGET_NAME helloworldoverlay)

add_executable(${TARGET_NAME}
  ${SHARED_SRC_FILES}
  main.cpp
  openvroverlaycontroller.cpp
  openvroverlaycontroller.h
  overlaywidget.cpp
  overlaywidget.h
)

add_definitions(${QT_DEFINITIONS})
if(NOT "${CMAKE_BUILD_TYPE}" EQUAL "Debug")
    add_definitions(-DQT_NO_DEBUG)
endif()

target_link_libraries(${TARGET_NAME}
  ${QT_LIBRARIES}
  ${OPENVR_LIBRARIES}
  ${CMAKE_DL_LIBS}
)

set_target_properties(${TARGET_NAME}
  PROPERTIES
  COMPILE_FLAGS "${QT_FLAGS}"
)

setTargetOutputDirectory(${TARGET_NAME})
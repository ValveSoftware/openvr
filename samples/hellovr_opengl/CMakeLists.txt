set(TARGET_NAME hellovr_opengl)

add_executable(${TARGET_NAME}
  ${SHARED_SRC_FILES}
  hellovr_opengl_main.cpp
)

target_link_libraries(${TARGET_NAME}
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${SDL2_LIBRARIES}
  ${OPENVR_LIBRARIES}
  ${CMAKE_DL_LIBS}
)

setTargetOutputDirectory(${TARGET_NAME})

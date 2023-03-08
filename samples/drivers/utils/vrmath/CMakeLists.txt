add_library(util_vrmath INTERFACE vrmath.h)
target_include_directories(util_vrmath INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(util_vrmath INTERFACE ${OPENVR_LIBRARIES})
target_include_directories(util_vrmath INTERFACE ${OPENVR_INCLUDE_DIR})
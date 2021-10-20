cmake_minimum_required(VERSION 3.9.6)

project(qpointsview CXX)

find_package(Qt5 COMPONENTS Widgets)

find_program(GLSLANG_VALIDATOR glslangValidator)

if(GLSLANG_VALIDATOR)
  add_custom_target(qpointsview_shader_validation ALL
    COMMAND ${GLSLANG_VALIDATOR} "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/render_points.vert"
    COMMAND ${GLSLANG_VALIDATOR} "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/render_points.frag")
endif(GLSLANG_VALIDATOR)

add_library(qpointsview
  include/qpointsview/QPointsView
  include/qpointsview/qpointsview.h
  include/qpointsview/QAbstractCamera
  include/qpointsview/qabstractcamera.h
  include/qpointsview/QFlyCamera
  include/qpointsview/qflycamera.h
  src/qabstractcamera.cpp
  src/qpointsview.cpp
  src/qflycamera.cpp
  src/shaders/shaders.qrc)

set_target_properties(qpointsview
  PROPERTIES
    AUTOMOC ON
    AUTORCC ON)

target_include_directories(qpointsview
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_compile_features(qpointsview PRIVATE cxx_std_17)

target_link_libraries(qpointsview
  PUBLIC
    Qt5::Widgets)

add_executable(qpointsview_bench
  bench.cpp)

target_link_libraries(qpointsview_bench PUBLIC qpointsview)

set_target_properties(qpointsview_bench
  PROPERTIES
    OUTPUT_NAME bench)

cmake_minimum_required(VERSION 3.9.6)

project(qpointsview CXX)

option(QPOINTSVIEW_EXAMPLE "Whether or not to build the example program." OFF)

##############################################
# Optionally validate shaders at build time. #
##############################################

find_program(GLSLANG_VALIDATOR glslangValidator)

if(GLSLANG_VALIDATOR)
  add_custom_target(qpointsview_shader_validation ALL
    COMMAND ${GLSLANG_VALIDATOR} "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/render_points.vert"
    COMMAND ${GLSLANG_VALIDATOR} "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/render_points.frag"
    COMMAND ${GLSLANG_VALIDATOR} "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/render_widget.vert"
    COMMAND ${GLSLANG_VALIDATOR} "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/render_widget.frag")
endif(GLSLANG_VALIDATOR)

######################
# Build the library. #
######################

find_package(Qt5 COMPONENTS Widgets)

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
  src/qwidgetcontainer.h
  src/qwidgetcontainer.cpp
  src/qwidgetcontainerprogram.h
  src/qwidgetcontainerprogram.cpp
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

############################
# Add the example program. #
############################

if(QPOINTSVIEW_EXAMPLE)

  add_executable(qpointsview_example
    example/main.cpp)

  target_link_libraries(qpointsview_example PUBLIC qpointsview)

  set_target_properties(qpointsview_example
    PROPERTIES
      OUTPUT_NAME example)

endif(QPOINTSVIEW_EXAMPLE)

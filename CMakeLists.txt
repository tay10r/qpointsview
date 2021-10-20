cmake_minimum_required(VERSION 3.9.6)

project(qpointsview CXX)

find_package(Qt5 COMPONENTS Widgets)

add_library(qpointsview
  include/qpointsview/QPointsView
  include/qpointsview/qpointsview.h
  src/qpointsview.cpp)

target_include_directories(qpointsview
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(qpointsview
  PUBLIC
    Qt5::Widgets)

add_executable(qpointsview_bench
  bench.cpp)

target_link_libraries(qpointsview_bench PUBLIC qpointsview)

set_target_properties(qpointsview_bench
  PROPERTIES
    OUTPUT_NAME bench)

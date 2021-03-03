cmake_minimum_required(VERSION 3.17)

project(Lab2)

set(CMAKE_CXX_STANDARD 14)

set(VTK_DIR C:/Program Files (x86)/VTK/lib/cmake/vtk-9.0)

find_package(VTK REQUIRED)

include(${VTK_USE_FILE})

add_executable(Lab2 main.cpp)

target_link_libraries(Lab2 gmsh ${VTK_LIBRARIES})
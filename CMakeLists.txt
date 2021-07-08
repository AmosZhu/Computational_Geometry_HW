cmake_minimum_required(VERSION 3.10)
project(ComputationalGemoetry)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(PA1_2 PA1_2_Crossroad/unit_test.cpp
        PA1_2_Crossroad/SweepLine.cpp)

target_link_libraries(PA1_2 ${OpenCV_LIBS})
target_include_directories(PA1_2 PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )

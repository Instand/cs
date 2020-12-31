cmake_minimum_required(VERSION 3.10)

project(test_benchmark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC "test_benchmark_instance.hpp" "test_benchmark_instance.cpp")
target_link_libraries(${PROJECT_NAME} cs)

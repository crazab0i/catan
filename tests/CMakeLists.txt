cmake_minimum_required(VERSION 3.22)

project(catan_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    -Wall
    -Werror
    -Wextra
    -Wpedantic
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/.. ${CMAKE_BINARY_DIR}/catan)

add_executable(catan_tests test_board.cpp)

target_link_libraries(catan_tests PRIVATE catan::catan)
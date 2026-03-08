cmake_minimum_required(VERSION 3.22)

project(game_catan_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    -Wall
    -Werror
    -Wextra
    -Wpedantic
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../.. ${CMAKE_BINARY_DIR}/catan)

add_executable(test_setup test_setup.cpp)

target_link_libraries(test_setup PRIVATE catan::catan)
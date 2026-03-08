cmake_minimum_required(VERSION 3.22)

project(print_catan_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    -Wall
    -Werror
    -Wextra
    -Wpedantic
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../.. ${CMAKE_BINARY_DIR}/catan)

add_executable(print_board basic_print_board.cpp)
add_executable(print_bank basic_print_bank.cpp)
add_executable(print_player basic_print_player.cpp)

target_link_libraries(print_board PRIVATE catan::catan)
target_link_libraries(print_bank PRIVATE catan::catan)
target_link_libraries(print_player PRIVATE catan::catan)
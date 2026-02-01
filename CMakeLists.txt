cmake_minimum_required(VERSION 3.22)

project(catan LANGUAGES CXX)

add_library(catan STATIC
    src/board.cpp
)

add_library(catan::catan ALIAS catan)

add_compile_options(
    -Wall
    -Werror
    -Wextra
    -Wpedantic
)

target_compile_features(catan PUBLIC cxx_std_23)

target_include_directories(catan
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)
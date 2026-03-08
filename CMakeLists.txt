cmake_minimum_required(VERSION 3.22)

project(catan LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(catan STATIC
    src/bank.cpp
    src/board.cpp
    src/game.cpp
    src/player.cpp
    src/catanConsts.cpp
)

add_library(catan::catan ALIAS catan)

add_compile_options(catan PRIVATE
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
cmake_minimum_required(VERSION 3.16)

project(LatticeTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    lattice_test
    tests/test_lattice.cpp
    src/lattice.cpp
)

target_include_directories(
    lattice_test
    PRIVATE
        include
)

target_compile_options(
    lattice_test
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

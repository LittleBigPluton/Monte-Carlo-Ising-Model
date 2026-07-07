cmake_minimum_required(VERSION 3.16)

project(IsingModelTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(
    ising_model_core
    src/lattice.cpp
    src/ising_model.cpp
    src/statistics.cpp
)

target_include_directories(
    ising_model_core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_compile_options(
    ising_model_core
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(
    ising_model_test
    tests/test_ising_model.cpp
)

target_link_libraries(
    ising_model_test
    PRIVATE
        ising_model_core
)

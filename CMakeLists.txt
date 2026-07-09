cmake_minimum_required(VERSION 3.16)

project(IsingModelTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use Release mode by default when no build type is specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_library(
    ising_model_core
    src/lattice.cpp
    src/ising_model.cpp
    src/metropolis.cpp
    src/measurements.cpp
    src/statistics.cpp
    src/error_analysis.cpp
    src/data_export.cpp
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
        $<$<CONFIG:Release>:-O3>
)

# OpenMP support
find_package(OpenMP)

if(OpenMP_CXX_FOUND)
    target_link_libraries(
        ising_model_core
        PUBLIC
            OpenMP::OpenMP_CXX
    )
endif()

# GSL support
find_package(GSL)

if(GSL_FOUND)
    target_link_libraries(
        ising_model_core
        PUBLIC
            GSL::gsl
            GSL::gslcblas
    )
endif()

add_executable(
    test_error_analysis
    tests/test_error_analysis.cpp
)

target_link_libraries(
    test_error_analysis
    PRIVATE
        ising_model_core
)

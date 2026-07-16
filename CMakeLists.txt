cmake_minimum_required(VERSION 3.20)

project(
    MonteCarloIsingModel
    VERSION 2.1.0
    DESCRIPTION "Monte Carlo simulations of 2D Ising and XY models using Metropolis, Wolff and Hybrid Monte Carlo algorithms"
    LANGUAGES CXX
)

# ---------------------------------------------------------------------------
# Project options
# ---------------------------------------------------------------------------

option(MC_BUILD_TESTS "Build test and simulation executables" ON)
option(MC_ENABLE_OPENMP "Enable OpenMP support" OFF)
option(MC_ENABLE_LTO "Enable link-time optimization in Release builds" ON)
option(MC_ENABLE_NATIVE_OPTIMIZATIONS
       "Optimize for the current CPU; reduces binary portability" OFF)
option(MC_ENABLE_SANITIZERS
       "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

# ---------------------------------------------------------------------------
# C++ configuration
# ---------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(
        CMAKE_BUILD_TYPE Release
        CACHE STRING "Build type: Debug, Release, RelWithDebInfo, or MinSizeRel"
        FORCE
    )
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------
# Reusable Monte Carlo library
# ---------------------------------------------------------------------------

add_library(
    ising_model_core
    src/lattice.cpp
    src/ising_model.cpp
    src/metropolis.cpp
    src/measurements.cpp
    src/statistics.cpp
    src/error_analysis.cpp
    src/data_export.cpp
    src/wolff_cluster.cpp
    src/hmc_xy_model.cpp
)

add_library(MonteCarlo::Core ALIAS ising_model_core)

target_include_directories(
    ising_model_core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_compile_features(ising_model_core PUBLIC cxx_std_17)

# ---------------------------------------------------------------------------
# Compiler warnings
# ---------------------------------------------------------------------------

if(MSVC)
    target_compile_options(
        ising_model_core
        PRIVATE
            /W4
            /permissive-
    )
else()
    target_compile_options(
        ising_model_core
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endif()

# ---------------------------------------------------------------------------
# Release optimizations
# ---------------------------------------------------------------------------

if(MSVC)
    target_compile_options(
        ising_model_core
        PRIVATE
            $<$<CONFIG:Release>:/O2>
            $<$<CONFIG:RelWithDebInfo>:/O2>
    )
else()
    target_compile_options(
        ising_model_core
        PRIVATE
            $<$<CONFIG:Release>:-O3>
            $<$<CONFIG:RelWithDebInfo>:-O2>
    )
endif()

if(MC_ENABLE_NATIVE_OPTIMIZATIONS)
    if(MSVC)
        target_compile_options(
            ising_model_core
            PRIVATE
                $<$<CONFIG:Release>:/arch:AVX2>
                $<$<CONFIG:RelWithDebInfo>:/arch:AVX2>
        )
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(
            ising_model_core
            PRIVATE
                $<$<CONFIG:Release>:-march=native>
                $<$<CONFIG:RelWithDebInfo>:-march=native>
        )
    endif()
endif()

if(MC_ENABLE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT MC_IPO_SUPPORTED OUTPUT MC_IPO_ERROR)

    if(MC_IPO_SUPPORTED)
        set_property(
            TARGET ising_model_core
            PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
        )
        set_property(
            TARGET ising_model_core
            PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO TRUE
        )
    else()
        message(STATUS "IPO/LTO is unavailable: ${MC_IPO_ERROR}")
    endif()
endif()

# ---------------------------------------------------------------------------
# Optional OpenMP
# ---------------------------------------------------------------------------

if(MC_ENABLE_OPENMP)
    find_package(OpenMP REQUIRED COMPONENTS CXX)
    target_link_libraries(ising_model_core PUBLIC OpenMP::OpenMP_CXX)
endif()

# ---------------------------------------------------------------------------
# Optional sanitizers
# ---------------------------------------------------------------------------

if(MC_ENABLE_SANITIZERS)
    if(MSVC)
        message(WARNING "Sanitizers are configured for GCC and Clang only.")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(
            ising_model_core
            PRIVATE
                -fsanitize=address,undefined
                -fno-omit-frame-pointer
        )
        target_link_options(
            ising_model_core
            PRIVATE
                -fsanitize=address,undefined
        )
    endif()
endif()

# ---------------------------------------------------------------------------
# Test and simulation executables
# ---------------------------------------------------------------------------

if(MC_BUILD_TESTS)
    add_executable(test_lattice tests/test_lattice.cpp)
    target_link_libraries(test_lattice PRIVATE MonteCarlo::Core)

    add_executable(test_metropolis tests/test_metropolis.cpp)
    target_link_libraries(test_metropolis PRIVATE MonteCarlo::Core)

    add_executable(test_error_analysis tests/test_error_analysis.cpp)
    target_link_libraries(test_error_analysis PRIVATE MonteCarlo::Core)

    add_executable(test_metropolis_wolff_comparison tests/test_metropolis_wolff_comparison.cpp)
    target_link_libraries(test_metropolis_wolff_comparison PRIVATE MonteCarlo::Core)

    add_executable(test_hmc_xy_parameter_scan tests/test_hmc_xy_parameter_scan.cpp)
    target_link_libraries(test_hmc_xy_parameter_scan PRIVATE MonteCarlo::Core)

    add_executable(test_hmc_xy_temperature_scan tests/test_hmc_xy_temperature_scan.cpp)
    target_link_libraries(test_hmc_xy_temperature_scan PRIVATE MonteCarlo::Core)
endif()

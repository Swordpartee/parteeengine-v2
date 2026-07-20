# Partee Engine CMakeLists.txt
# A hyper-customizable game engine build configuration

cmake_minimum_required(VERSION 3.21)

# Project declaration
project(
    parteeeengine
    VERSION 2.0.0
    DESCRIPTION "N/A"
    LANGUAGES CXX C
)

# ============================================================================
# CMake Configuration
# ============================================================================

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# ============================================================================
# Project Options
# ============================================================================

option(PARTEEE_BUILD_TESTS "Build unit tests" ON)
option(PARTEEE_BUILD_DOCS "Build documentation" OFF)
option(PARTEEE_ENABLE_LTO "Enable Link Time Optimization" ON)
option(PARTEEE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)

# ============================================================================
# Compiler Flags and Warnings
# ============================================================================

# MSVC-specific flags
if(MSVC)
    add_compile_options(/W4)
    if(PARTEEE_WARNINGS_AS_ERRORS)
        add_compile_options(/WX)
    endif()
else()
    # GCC and Clang flags
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(PARTEEE_WARNINGS_AS_ERRORS)
        add_compile_options(-Werror)
    endif()
endif()

# Link-time optimization
if(PARTEEE_ENABLE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT LTO_SUPPORTED OUTPUT LTO_NOT_SUPPORTED)
    if(LTO_SUPPORTED)
        message(STATUS "Link Time Optimization enabled")
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    else()
        message(WARNING "Link Time Optimization not supported: ${LTO_NOT_SUPPORTED}")
    endif()
endif()

# ============================================================================
# Source Files and Directories
# ============================================================================

set(PARTEEE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(PARTEEE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Collect source files
file(GLOB_RECURSE PARTEEE_SOURCES
    "${PARTEEE_SOURCE_DIR}/*.cpp"
    "${PARTEEE_SOURCE_DIR}/*.cc"
    "${PARTEEE_SOURCE_DIR}/*.cxx"
)

# Exclude main.cpp from library
list(FILTER PARTEEE_SOURCES EXCLUDE REGEX "main\\.cpp$")

file(GLOB_RECURSE PARTEEE_HEADERS
    "${PARTEEE_INCLUDE_DIR}/*.hpp"
    "${PARTEEE_INCLUDE_DIR}/*.h"
)

# ============================================================================
# Main Library Target
# ============================================================================

add_library(parteeeengine_lib STATIC ${PARTEEE_SOURCES} ${PARTEEE_HEADERS})

# Set target properties
set_target_properties(parteeeengine_lib PROPERTIES
    PUBLIC_HEADER "${PARTEEE_HEADERS}"
    SOVERSION 2
    VERSION 2.0.0
)

# Specify include directories
target_include_directories(parteeeengine_lib
    PUBLIC
        $<BUILD_INTERFACE:${PARTEEE_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${PARTEEE_SOURCE_DIR}
)

# Set compile features
target_compile_features(parteeeengine_lib PUBLIC cxx_std_17)

# ============================================================================
# Main Executable
# ============================================================================

add_executable(parteeeengine
    "${PARTEEE_SOURCE_DIR}/main.cpp"
)

target_link_libraries(parteeeengine PRIVATE parteeeengine_lib)
target_include_directories(parteeeengine PRIVATE ${PARTEEE_INCLUDE_DIR})

# ============================================================================
# Testing
# ============================================================================

if(PARTEEE_BUILD_TESTS)
    enable_testing()
    message(STATUS "Building tests enabled")
    
    # Fetch GTest from Google's repository
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP ON
    )
    
    # For Windows: prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    add_subdirectory(test)
endif()

# ============================================================================
# Documentation
# ============================================================================

if(PARTEEE_BUILD_DOCS)
    find_package(Doxygen)
    if(Doxygen_FOUND)
        message(STATUS "Building documentation enabled")
        # Add Doxygen configuration here
    else()
        message(WARNING "Doxygen not found - documentation build disabled")
    endif()
endif()

# ============================================================================
# Installation
# ============================================================================

include(GNUInstallDirs)

# Install targets
install(TARGETS parteeeengine parteeeengine_lib
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/parteeeengine
)

# Install header files
install(DIRECTORY "${PARTEEE_INCLUDE_DIR}/"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/parteeeengine
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

# ============================================================================
# Summary
# ============================================================================

message(STATUS "========== Partee Engine Configuration ==========")
message(STATUS "Build Type:            ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard:          ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler:              ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Build Tests:           ${PARTEEE_BUILD_TESTS}")
message(STATUS "Build Documentation:   ${PARTEEE_BUILD_DOCS}")
message(STATUS "Link Time Optimization: ${PARTEEE_ENABLE_LTO}")
message(STATUS "Warnings as Errors:    ${PARTEEE_WARNINGS_AS_ERRORS}")
message(STATUS "================================================")

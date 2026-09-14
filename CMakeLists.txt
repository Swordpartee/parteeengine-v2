cmake_minimum_required(VERSION 4.4)

# Project declaration
project(
    parteeengine
    VERSION 2.0.0
    DESCRIPTION "N/A"
    LANGUAGES CXX C
)

# ============================================================================
# CMake Compile Commands
# ============================================================================

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

# ============================================================================
# Project Options
# ============================================================================

option(PARTEEENGINE_BUILD_TESTS "Build unit tests" ON)
option(PARTEEENGINE_BUILD_DOCS "Build documentation" OFF)
option(PARTEEENGINE_ENABLE_LTO "Enable Link Time Optimization" OFF)
option(PARTEEENGINE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(PARTEEENGINE_ENABLE_CLANG_TIDY "Run clang-tidy during the build" ON)

# ============================================================================
# C++ Standard
# ============================================================================

set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# Compiler Flags and Warnings
# ============================================================================

# Link-time optimization
if(PARTEEENGINE_ENABLE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT LTO_SUPPORTED OUTPUT LTO_NOT_SUPPORTED)
    if(LTO_SUPPORTED)
        message(STATUS "Link Time Optimization enabled")
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    else()
        message(WARNING "Link Time Optimization not supported: ${LTO_NOT_SUPPORTED}")
    endif()
endif()

add_library(parteeengine_warnings INTERFACE)

if(MSVC)
    target_compile_options(parteeengine_warnings INTERFACE /W4)
    if(PARTEEENGINE_WARNINGS_AS_ERRORS)
        target_compile_options(parteeengine_warnings INTERFACE /WX)
    endif()
else()
    target_compile_options(parteeengine_warnings INTERFACE -Wall -Wextra -Wpedantic)
    if(PARTEEENGINE_WARNINGS_AS_ERRORS)
        target_compile_options(parteeengine_warnings INTERFACE -Werror)
    endif()
endif()

# ============================================================================
# Source Files and Directories
# ============================================================================

set(PARTEEENGINE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(PARTEEENGINE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Explicit list of all header files (Prevents stale CMake cache issues)
set(PARTEEENGINE_HEADERS
    "${PARTEEENGINE_INCLUDE_DIR}/core/Engine.hpp"
    "${PARTEEENGINE_INCLUDE_DIR}/core/entities/ComponentManager.hpp"
    "${PARTEEENGINE_INCLUDE_DIR}/core/entities/ComponentStorage.hpp"
    "${PARTEEENGINE_INCLUDE_DIR}/core/entities/Entity.hpp"
    "${PARTEEENGINE_INCLUDE_DIR}/core/entities/EntityManager.hpp"
    "${PARTEEENGINE_INCLUDE_DIR}/core/events/EventManager.hpp"
    "${PARTEEENGINE_INCLUDE_DIR}/core/events/QuitEvent.hpp"
    "${PARTEEENGINE_INCLUDE_DIR}/core/modules/ModuleBase.hpp"
    "${PARTEEENGINE_INCLUDE_DIR}/core/modules/ModuleManager.hpp"
    "${PARTEEENGINE_INCLUDE_DIR}/util/OSUtil.hpp"
    "${PARTEEENGINE_INCLUDE_DIR}/rendering/window/ModuleWindow.hpp"
    "${PARTEEENGINE_INCLUDE_DIR}/rendering/window/NativeWindow.hpp"
    "${PARTEEENGINE_INCLUDE_DIR}/rendering/window/WindowConfig.hpp"
    "${PARTEEENGINE_INCLUDE_DIR}/rendering/window/WindowEvent.hpp"
    "${PARTEEENGINE_INCLUDE_DIR}/rendering/window/WindowEventHandler.hpp"
    "${PARTEEENGINE_INCLUDE_DIR}/rendering/window/WindowManagerModule.hpp"
)

# Explicit list of all library source files
set(PARTEEENGINE_SOURCES
    "${PARTEEENGINE_SOURCE_DIR}/core/Engine.cpp"
    "${PARTEEENGINE_SOURCE_DIR}/core/entities/EntityManager.cpp"
    "${PARTEEENGINE_SOURCE_DIR}/core/modules/ModuleManager.cpp"
    "${PARTEEENGINE_SOURCE_DIR}/rendering/window/WindowManagerModule.cpp"
    "${PARTEEENGINE_SOURCE_DIR}/rendering/window/ModuleWindow.cpp"
)

if(WIN32)
    list(APPEND PARTEEENGINE_SOURCES
        "${PARTEEENGINE_SOURCE_DIR}/rendering/window/osWindow/W32NativeWindow.cpp"
    )
elseif(APPLE)
    list(APPEND PARTEEENGINE_SOURCES)
elseif(UNIX AND NOT APPLE)
    list(APPEND PARTEEENGINE_SOURCES
        "${PARTEEENGINE_SOURCE_DIR}/rendering/window/osWindow/UnixNativeWindow.cpp"
    )
endif()

# ============================================================================
# Main Library Target
# ============================================================================

add_library(parteeengine_lib STATIC)

target_sources(parteeengine_lib PRIVATE ${PARTEEENGINE_SOURCES})

target_compile_features(parteeengine_lib PUBLIC cxx_std_23)

target_link_libraries(parteeengine_lib PRIVATE parteeengine_warnings)

set_target_properties(parteeengine_lib PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# Specify include directories
target_include_directories(parteeengine_lib
    PUBLIC
        $<BUILD_INTERFACE:${PARTEEENGINE_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# ============================================================================
# Main Executable
# ============================================================================

add_executable(parteeengine "${PARTEEENGINE_SOURCE_DIR}/main.cpp")

target_link_libraries(parteeengine PRIVATE parteeengine_lib parteeengine_warnings)
target_include_directories(parteeengine PRIVATE ${PARTEEENGINE_INCLUDE_DIR})

# ============================================================================
# Testing
# ============================================================================

if(PARTEEENGINE_BUILD_TESTS)
    enable_testing()
    message(STATUS "Building tests enabled")

    # Fetch GTest from Google's repository
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP ON
        SYSTEM
    )

    # For Windows: prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_subdirectory(test)
endif()

# ============================================================================
# Clang Tidy
# ============================================================================

if(PARTEEENGINE_ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    if(CLANG_TIDY_EXE)
        set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}")

        set_target_properties(parteeengine_lib PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
        set_target_properties(parteeengine PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

        if(PARTEEENGINE_BUILD_TESTS AND TARGET parteeengine_tests)
            set_target_properties(parteeengine_tests PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
        endif()
    else()
        message(WARNING "clang-tidy not found - static analysis disabled")
    endif()
endif()

# ============================================================================
# Documentation
# ============================================================================

if(PARTEEENGINE_BUILD_DOCS)
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
install(TARGETS parteeengine parteeengine_lib
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install header files
install(DIRECTORY "${PARTEEENGINE_INCLUDE_DIR}/"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/parteeengine
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

# ============================================================================
# Summary
# ============================================================================

message(STATUS "========== Partee Engine Configuration ==========")
message(STATUS "Build Type:             ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Requirement:        cxx_std_23")
message(STATUS "Compiler:               ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Build Tests:            ${PARTEEENGINE_BUILD_TESTS}")
message(STATUS "Build Documentation:    ${PARTEEENGINE_BUILD_DOCS}")
message(STATUS "Link Time Optimization: ${PARTEEENGINE_ENABLE_LTO}")
message(STATUS "Warnings as Errors:     ${PARTEEENGINE_WARNINGS_AS_ERRORS}")
message(STATUS "Clang-Tidy Enabled:     ${PARTEEENGINE_ENABLE_CLANG_TIDY}")
message(STATUS "================================================")
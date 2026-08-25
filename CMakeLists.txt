cmake_minimum_required(VERSION 4.4.1)

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

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

# Explicit list of all header files (Prevents stale CMake cache issues)
set(PARTEEE_HEADERS
    "${PARTEEE_INCLUDE_DIR}/core/Engine.hpp"
    "${PARTEEE_INCLUDE_DIR}/core/entities/ComponentManager.hpp"
    "${PARTEEE_INCLUDE_DIR}/core/entities/ComponentStorage.hpp"
    "${PARTEEE_INCLUDE_DIR}/core/entities/Entity.hpp"
    "${PARTEEE_INCLUDE_DIR}/core/entities/EntityManager.hpp"
    "${PARTEEE_INCLUDE_DIR}/core/events/EventManager.hpp"
    "${PARTEEE_INCLUDE_DIR}/core/events/QuitEvent.hpp"
    "${PARTEEE_INCLUDE_DIR}/core/modules/ModuleBase.hpp"
    "${PARTEEE_INCLUDE_DIR}/core/modules/ModuleManager.hpp"
    "${PARTEEE_INCLUDE_DIR}/util/OSUtil.hpp"
    "${PARTEEE_INCLUDE_DIR}/rendering/windows/ModuleWindow.hpp"
    "${PARTEEE_INCLUDE_DIR}/rendering/windows/NativeWindow.hpp"
    "${PARTEEE_INCLUDE_DIR}/rendering/windows/WindowConfig.hpp"
    "${PARTEEE_INCLUDE_DIR}/rendering/windows/WindowEvent.hpp"
    "${PARTEEE_INCLUDE_DIR}/rendering/windows/WindowEventHandler.hpp"
    "${PARTEEE_INCLUDE_DIR}/rendering/windows/WindowManagerModule.hpp"

)

# Explicit list of all library source files
set(PARTEEE_SOURCES
    "${PARTEEE_SOURCE_DIR}/core/Engine.cpp"
    "${PARTEEE_SOURCE_DIR}/core/entities/EntityManager.cpp"
    "${PARTEEE_SOURCE_DIR}/core/modules/ModuleManager.cpp"
    "${PARTEEE_SOURCE_DIR}/rendering/windows/WindowManagerModule.cpp"
    "${PARTEEE_SOURCE_DIR}/rendering/windows/Window.cpp"




)

if(WIN32)
    list(APPEND PARTEEE_SOURCES 
        "${PARTEEE_SOURCE_DIR}/rendering/windows/W32NativeWindow.cpp"
        )
elseif(APPLE)
    set(OS_SOURCES )
elseif(UNIX AND NOT APPLE)
    set(OS_SOURCES)
endif()

# ============================================================================
# Main Library Target
# ============================================================================

add_library(parteeeengine_lib STATIC ${PARTEEE_SOURCES} ${PARTEEE_HEADERS})

# Specify include directories
target_include_directories(parteeeengine_lib
    PUBLIC
        $<BUILD_INTERFACE:${PARTEEE_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# ============================================================================
# Main Executable
# ============================================================================

add_executable( parteeeengine "${PARTEEE_SOURCE_DIR}/main.cpp" )

target_link_libraries(parteeeengine PRIVATE parteeeengine_lib)
target_include_directories(parteeeengine PRIVATE ${PARTEEE_INCLUDE_DIR})

# Warning Flags
if(MSVC)
    target_compile_options(parteeeengine_lib PRIVATE /W4)

    if(PARTEEE_WARNINGS_AS_ERRORS)
        target_compile_options(parteeeengine_lib PRIVATE /WX)
    endif()

else()
    target_compile_options(parteeeengine_lib PRIVATE -Wall -Wextra -Wpedantic)

    if(PARTEEE_WARNINGS_AS_ERRORS)
        target_compile_options(parteeeengine_lib PRIVATE -Werror)
    endif()

endif()

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
        URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip
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

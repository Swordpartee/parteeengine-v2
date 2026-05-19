cmake_minimum_required(VERSION 3.23)
project(parteeeengine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

# Recursively find all CPP files
file(GLOB_RECURSE PARTEE_SOURCES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.cc"
    "${PROJECT_SOURCE_DIR}/src/*.cxx"
)

message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

# Add parteeengine executable
add_executable(parteeeengine ${PARTEE_SOURCES})

# Require C++23 for this target
target_compile_features(parteeeengine PUBLIC cxx_std_23)

# Include directories
target_include_directories(parteeeengine PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/libs
)

# Compiler warnings and flags — use generator expressions for portability
target_compile_options(parteeeengine PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4;/permissive-;/WX>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall;-Wextra;-Werror;-pedantic>
)
# Include GoogleTest module for gtest_discover_tests
include(GoogleTest)

# Collect test source files
file(GLOB_RECURSE TEST_SOURCES "*.cpp")

# Create test executable
add_executable(parteeeengine_tests ${TEST_SOURCES})

# Link against the library and GTest
target_link_libraries(parteeeengine_tests 
    PRIVATE 
        parteeeengine_lib
        GTest::gtest_main
        GTest::gtest
)

# Include directories
target_include_directories(parteeeengine_tests 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PARTEEE_INCLUDE_DIR}
)

# Set C++ standard
target_compile_features(parteeeengine_tests PRIVATE cxx_std_23)

# Discover and register individual tests
gtest_discover_tests(parteeeengine_tests)
# Include GoogleTest module for gtest_discover_tests
include(GoogleTest)

# Explicit test source list (no file globbing).
set(TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/core/EngineTest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/entities/ComponentManagerTest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/entities/EntityManagerTest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/events/EventManagerTest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/modules/ModuleManagerTest.cpp"
)

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
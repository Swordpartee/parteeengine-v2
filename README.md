# Partee Engine v2

A customizable, high-performance C++ game engine built from scratch with a modern entity-component-system (ECS) architecture.

## Features

- **Entity-Component-System Architecture**: Flexible and scalable design pattern for game logic
- **Component Manager**: Efficient component storage and retrieval
- **Event Bus**: Decoupled event handling for game systems
- **Entity Manager**: Centralized entity lifecycle management
- **Modern C++17**: Clean, type-safe code with minimal dependencies
- **CMake Build System**: Cross-platform compilation support
- **Comprehensive Tests**: Unit tests using Google Test framework

## Project Structure

```
parteeengine-v2/
├── include/              # Public header files
│   └── core/
│       ├── entities/     # ECS core components
│       └── events/       # Event system
├── src/                  # Implementation files
│   ├── main.cpp         # Entry point
│   └── core/
│       └── entities/    # ECS implementations
├── test/                # Unit tests
│   └── core/
│       ├── entities/    # Entity/Component tests
│       └── events/      # Event system tests
├── build/               # Build output (generated)
└── CMakeLists.txt       # Build configuration
```

## Building

### Prerequisites

- CMake 3.21 or higher
- C++17 compatible compiler (MSVC, GCC, or Clang)
- Google Test (automatically downloaded via CMake)

### Build Steps

```bash
# Create and enter build directory
mkdir build && cd build

# Configure the project
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build . --config Release

# Run tests
ctest -C Release
```

## Running Tests

All tests are automatically built with the project. To run them:

```bash
cd build
ctest -C Release -V  # -V for verbose output
```

Or run specific test suites:

```bash
# Run entity manager tests
ctest -C Release -R EntityManager -V

# Run component manager tests
ctest -C Release -R ComponentManager -V
```

## Architecture Overview

### Entity-Component-System (ECS)

The engine uses an ECS pattern for maximum flexibility:

- **Entities**: Objects in your game world
- **Components**: Data containers that define entity properties
- **Systems**: Logic that operates on entities with specific components

### Core Systems

- **EntityManager**: Manages entity lifecycle and queries
- **ComponentManager**: Stores and retrieves components efficiently
- **EventBus**: Distributes events throughout the engine

## CMake Options

Configure build behavior with these options:

```cmake
-DPARTEEE_BUILD_TESTS=ON      # Build unit tests (default: ON)
-DPARTEEE_BUILD_DOCS=OFF      # Build documentation (default: OFF)
-DPARTEEE_ENABLE_LTO=ON       # Enable Link Time Optimization (default: ON)
-DPARTEEE_WARNINGS_AS_ERRORS=ON  # Treat warnings as errors (default: ON)
```

Example:
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -DPARTEEE_BUILD_TESTS=ON
```

## Compiler Support

- **MSVC**: Visual Studio 2017 or later
- **GCC**: 7.0 or later
- **Clang**: 5.0 or later

C++ Standard: **C++17** (required)

## CI/CD

This project uses GitHub Actions for continuous integration. The build workflow:
1. Configures CMake
2. Builds the project in Release mode
3. Runs all unit tests

See [.github/workflows/cppbuild.yml](.github/workflows/cppbuild.yml) for details.

## License

See LICENSE file for details.

## Contributing

Contributions are welcome! Please ensure:
- All tests pass locally before submitting
- Code follows the existing style conventions
- New features include corresponding unit tests
- Compiler warnings are treated as errors (enabled by default)
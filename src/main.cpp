#include <iostream>

#include "core/Engine.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;

    parteeengine::Engine engine;

    engine.run();

    return 0;
}
#include "core/Engine.hpp"

#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;

    parteeengine::Engine engine;

    engine.run();

    return 0;
}
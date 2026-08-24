#include "core/Engine.hpp"
#include "rendering/window/WindowManagerModule.hpp"

#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;

    parteeengine::Engine engine;

    engine.addModule<parteeengine::rendering::WindowManagerModule>();

    engine.getModule<parteeengine::rendering::WindowManagerModule>().generateWindow({});

    engine.run();

    return 0;
}
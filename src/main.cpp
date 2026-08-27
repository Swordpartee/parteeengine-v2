#include "core/Engine.hpp"
#include "rendering/window/ModuleWindow.hpp"
#include "rendering/window/WindowEvent.hpp"
#include "rendering/window/WindowManagerModule.hpp"

#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;

    parteeengine::Engine engine;

    engine.addModule<parteeengine::rendering::WindowManagerModule>();

    auto& windowManager = engine.getModule<parteeengine::rendering::WindowManagerModule>();
    auto& window = windowManager.createWindow({.dimensions = {800, 600}, .visible = true});

    window.subscribe<parteeengine::rendering::WindowCloseEvent>([](auto event, auto& window) {
        std::cout << "boop" << std::endl;
        // window.close();
    });

    engine.run();

    return 0;
}
#include "core/Engine.hpp"
#include "rendering/window/WindowEvent.hpp"
#include "rendering/window/WindowManagerModule.hpp"

#include <iostream>

int main() {
    std::cout << "Hello, World!" << '\n';

    parteeengine::Engine engine;

    engine.addModule<parteeengine::rendering::WindowManagerModule>();

    auto* windowManager = engine.getModule<parteeengine::rendering::WindowManagerModule>();
    if (windowManager == nullptr) {
        return 1;
    }
    auto* window = windowManager->createWindow({.dimensions = {800, 600}, .visible = true});
    if (window == nullptr) {
        return 1;
    }

    window->subscribe<parteeengine::rendering::WindowCloseEvent>([](auto& /*event*/, auto& window) {
        std::cout << "boop" << '\n';
        window.close();
    });

    engine.run();

    return 0;
}
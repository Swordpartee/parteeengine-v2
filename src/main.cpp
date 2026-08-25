#include "core/Engine.hpp"
#include "rendering/windows/ModuleWindow.hpp"
#include "rendering/windows/WindowEvent.hpp"
#include "rendering/windows/WindowManagerModule.hpp"

#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;

    parteeengine::Engine engine;

    engine.addModule<parteeengine::rendering::WindowManagerModule>();

    auto windowManager = engine.getModule<parteeengine::rendering::WindowManagerModule>();
    auto window = windowManager.createWindow({.visible = true});

    window.subscribe<parteeengine::rendering::WindowCloseEvent>([](parteeengine::rendering::WindowCloseEvent event, parteeengine::rendering::ModuleWindow* window){
        window->close();
    });

    engine.run();

    return 0;
}
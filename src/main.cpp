#include "core/Engine.hpp"
#include "rendering/window/WindowManagerModule.hpp"

int main() {

    parteeengine::Engine engine;

    engine.addModule<parteeengine::rendering::WindowManagerModule>();

    engine.getModule<parteeengine::rendering::WindowManagerModule>().createWindow({800, 600, "sample window"});

    engine.run();

    return 0;
}
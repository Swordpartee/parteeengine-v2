
#include "rendering/window/WindowManagerModule.hpp"

#include "rendering/window/NativeWindow.hpp"

namespace parteeengine::rendering {

void WindowManagerModule::update(const ModuleInput&) {
    for (auto window : windows) {
        window.poll();
    }
};

ModuleWindow& WindowManagerModule::createWindow(const WindowDesc& config) {

    auto native = NativeWindow::Create(config);

    auto window = ModuleWindow{native};

    windows.emplace_back(window);

    native->setEventHandler(&windows.back());

    return windows.back();
};

} // namespace parteeengine::rendering
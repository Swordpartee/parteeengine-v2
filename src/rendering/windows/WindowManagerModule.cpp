
#include "rendering/windows/WindowManagerModule.hpp"

#include "rendering/windows/NativeWindow.hpp"

namespace parteeengine::rendering {

void WindowManagerModule::update(const ModuleInput&) {
  for (auto window : windows) {
    window.poll();
  }
};

ModuleWindow& WindowManagerModule::createWindow(const WindowConfig& config) {

    auto native = NativeWindow::Create();

    auto window = ModuleWindow{native};

    window.configure(config);

    windows.emplace_back(window);

    native->setEventHandler(&windows.back());

    return windows.back();
};

} // namespace parteeengine::rendering
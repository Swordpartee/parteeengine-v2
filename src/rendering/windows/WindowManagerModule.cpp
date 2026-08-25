
#include "rendering/windows/WindowManagerModule.hpp"

#include "rendering/windows/NativeWindow.hpp"

namespace parteeengine::rendering {

void WindowManagerModule::update(const ModuleInput&) {
  for (auto window : windows) {
    window.poll();
  }
};

ModuleWindow& WindowManagerModule::createWindow(const WindowDesc& config) {

    auto native = NativeWindow::Create(config);

    auto window = ModuleWindow{native};

    auto it = windows.emplace(window);

    native->setEventHandler(&*it);

    return *it;
};

} // namespace parteeengine::rendering
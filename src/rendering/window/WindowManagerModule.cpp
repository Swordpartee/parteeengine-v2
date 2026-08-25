#include "rendering/window/WindowManagerModule.hpp"

namespace parteeengine::rendering {
void WindowManagerModule::update(const ModuleInput&) {
    std::erase_if(windows, [](NativeWindowHandle handle) { return Window::Get(handle).poll(); });
};

Window& WindowManagerModule::generateWindow(const WindowConfig& config) {
    windows.emplace_back(Window::CreateNativeWindow(config));

    return Window::Get(windows.back());
}
} // namespace parteeengine::rendering

#include "rendering/window/WindowManagerModule.hpp"

#include "core/modules/ModuleBase.hpp"
#include "rendering/window/NativeWindow.hpp"
#include "rendering/window/WindowConfig.hpp"

#include <utility>

namespace parteeengine::rendering {

void WindowManagerModule::update(const ModuleInput& /*input*/) {
    std::erase_if(windows, [](const auto& window) { return window.isDestroyed(); });

    for (auto& window : windows) {
        if (!window.isDestroyed()) {
            window.poll();
        }
    }
}

ModuleWindow* WindowManagerModule::createWindow(const WindowDesc& config) {
    auto native = NativeWindow::Create(config);

    if (native == nullptr) {
        return nullptr;
    }

    windows.emplace_back();
    ModuleWindow& window = windows.back();

    native->setEventHandler(&window);
    window.setNative(std::move(native));

    return &window;
};

} // namespace parteeengine::rendering
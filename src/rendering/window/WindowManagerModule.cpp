
#include "rendering/window/WindowManagerModule.hpp"

#include "core/modules/ModuleBase.hpp"
#include "rendering/window/NativeWindow.hpp"
#include "rendering/window/WindowConfig.hpp"

namespace parteeengine::rendering {

void WindowManagerModule::update(const ModuleInput& /*input*/) {
    for (auto window : windows) {
        window.poll();
    }
};

ModuleWindow* WindowManagerModule::createWindow(const WindowDesc& config) {

    auto* native = NativeWindow::Create(config);

    if (native == nullptr) {
        return nullptr;
    }

    auto window = ModuleWindow{native};

    windows.emplace_back(window);

    native->setEventHandler(&windows.back());

    return &windows.back();
};

} // namespace parteeengine::rendering
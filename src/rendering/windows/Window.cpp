#include "rendering/windows/ModuleWindow.hpp"

namespace parteeengine::rendering {
void ModuleWindow::emit(WindowEvent* event) {
    auto it = subscribers.find(typeid(*event));
    if (it != subscribers.end()) {
        it->second(std::any(event), this);
    }
}

void ModuleWindow::configure(const WindowConfig& config) { nativeWindow->config(config); }

void ModuleWindow::poll() { nativeWindow->poll(); }
} // namespace parteeengine::rendering
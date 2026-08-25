#include "rendering/windows/ModuleWindow.hpp"

namespace parteeengine::rendering {

void ModuleWindow::emit(std::unique_ptr<WindowEvent> event) {
    auto it = subscribers.find(typeid(*event));
    if (it != subscribers.end()) {
        for (auto& subscriber : it->second) {
            subscriber(*event, this);
        }
    }
    // event destructs automatically here
}

void ModuleWindow::configure(const WindowConfig& config) { nativeWindow->config(config); }

void ModuleWindow::close() { nativeWindow->close(); }

void ModuleWindow::poll() { nativeWindow->poll(); }
} // namespace parteeengine::rendering
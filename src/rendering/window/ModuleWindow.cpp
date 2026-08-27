#include "rendering/window/ModuleWindow.hpp"

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

bool ModuleWindow::shouldForward(const WindowEvent& event) const {
    // auto it = subscribers.find(typeid(event));
    // if (it == subscribers.end()) {
    //     return true;
    // }

    // return it->second.empty();

    (void)event;

    return false;
};

void ModuleWindow::configure(const WindowConfig& config) { nativeWindow->config(config); }

void ModuleWindow::close() { nativeWindow->close(); }

void ModuleWindow::poll() { nativeWindow->poll(); }
} // namespace parteeengine::rendering
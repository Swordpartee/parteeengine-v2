#include "rendering/window/ModuleWindow.hpp"

#include "rendering/window/WindowConfig.hpp"
#include "rendering/window/WindowEvent.hpp"

namespace parteeengine::rendering {

void ModuleWindow::emit(std::unique_ptr<WindowEvent> event) {
    WindowEvent& eventRef = *event;
    auto it = subscriberMap.find(typeid(eventRef));
    if (it != subscriberMap.end()) {
        for (auto& subscriber : it->second) {
            subscriber(*event, *this);
        }
    }
    // event destructs automatically here
}

bool ModuleWindow::shouldForward(const WindowEvent& event) const {
    auto it = subscriberMap.find(typeid(event));
    if (it == subscriberMap.end()) {
        return true;
    }

    return it->second.empty();
};

void ModuleWindow::configure(const WindowConfig& config) { nativeWindow->config(config); }

void ModuleWindow::close() { nativeWindow->close(); }

void ModuleWindow::poll() { nativeWindow->poll(); }

void ModuleWindow::config(const WindowConfig& config) { nativeWindow->config(config); }

} // namespace parteeengine::rendering
#include "rendering/window/ModuleWindow.hpp"

#include "rendering/window/WindowConfig.hpp"
#include "rendering/window/WindowEvent.hpp"

#include <memory>

namespace parteeengine::rendering {

void ModuleWindow::emit(std::unique_ptr<WindowEvent> event) {
    auto& eventRef = *event;
    auto iter = subscriberMap.find(typeid(eventRef));
    if (iter != subscriberMap.end()) {
        for (auto& subscriber : iter->second) {
            subscriber(*event, *this);
        }
    }
}

bool ModuleWindow::shouldForward(const WindowEvent& event) const {
    auto iter = subscriberMap.find(typeid(event));
    if (iter == subscriberMap.end()) {
        return true;
    }

    return iter->second.empty();
};

void ModuleWindow::configure(const WindowConfig& config) { nativeWindow->config(config); }

void ModuleWindow::close() { nativeWindow->close(); }

void ModuleWindow::poll() { nativeWindow->poll(); }

void ModuleWindow::config(const WindowConfig& config) { nativeWindow->config(config); }

} // namespace parteeengine::rendering
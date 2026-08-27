// ModuleWindow.hpp
#pragma once

#include "rendering/windows/WindowEvent.hpp"
#include "rendering/windows/NativeWindow.hpp"
#include "rendering/windows/WindowEventHandler.hpp"

#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace parteeengine::rendering {

struct WindowConfig;
class ModuleWindow;

using WindowSubscriber = std::function<void(WindowEvent&, ModuleWindow*)>;
template <is_window_event EventType>
using TypedWindowSubscriber = std::function<void(const EventType&, ModuleWindow*)>;

class ModuleWindow : public WindowEventHandler {
  private:
    std::unordered_map<std::type_index, std::vector<WindowSubscriber>> subscribers;
    NativeWindow* nativeWindow;

  public:
    ModuleWindow(NativeWindow* native) : nativeWindow(native) {};
    ModuleWindow() = delete;

    template <is_window_event EventType>
    void subscribe(TypedWindowSubscriber<EventType> subscriber);

    void emit(std::unique_ptr<WindowEvent> event) override;

    bool shouldForward(const WindowEvent& event) const override;

    void configure(const WindowConfig& config);
    void close();
    void poll();
};

template <is_window_event EventType>
void ModuleWindow::subscribe(TypedWindowSubscriber<EventType> subscriber) {
    subscribers[std::type_index(typeid(EventType))].push_back(
        [subscriber = std::move(subscriber)](WindowEvent& event, ModuleWindow* window) {
            subscriber(static_cast<const EventType&>(event), window);
        }
    );
}

} // namespace parteeengine::rendering
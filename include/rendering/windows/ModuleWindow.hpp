#pragma once

#include "WindowEvent.hpp"
#include "rendering/windows/NativeWindow.hpp"
#include "rendering/windows/WindowEventHandler.hpp"

#include <any>
#include <functional>
#include <typeindex>

#include <unordered_map>

namespace parteeengine::rendering {

struct WindowConfig;

class ModuleWindow;

using WindowSubscriber = std::function<void(const std::any, ModuleWindow*)>;
template <is_window_event EventType>
using TypedWindowSubscriber = std::function<void(const EventType, ModuleWindow*)>;

class ModuleWindow : public WindowEventHandler {
  private:
    std::unordered_map<std::type_index, WindowSubscriber> subscribers;

    NativeWindow* nativeWindow;

  public:
    ModuleWindow(NativeWindow* native) : nativeWindow(native) {};
    ModuleWindow() = delete;

    template <is_window_event EventType>
    void Subscribe(TypedWindowSubscriber<EventType> subscriber);

    void emit(WindowEvent* event) override;

    void configure(const WindowConfig& config);

    void poll();
};

template <is_window_event EventType>
void ModuleWindow::Subscribe(TypedWindowSubscriber<EventType> subscriber) {
    subscribers.emplace(std::type_index(typeid(EventType)), subscriber);
}

} // namespace parteeengine::rendering
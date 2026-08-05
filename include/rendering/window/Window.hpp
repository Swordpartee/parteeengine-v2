#pragma once

#include "WindowEvent.hpp"
#include "rendering/window/NativeWindowHandle.hpp"
#include "rendering/window/WindowConfig.hpp"
#include <any>
#include <functional>
#include <typeindex>

#include <unordered_map>

namespace parteeengine::rendering {

template <typename EventType>
concept is_window_event = std::derived_from<EventType, WindowEvent>;

using WindowSubscriber = std::function<void(const std::any)>;
template <is_window_event EventType>
using TypedWindowSubscriber = std::function<void(const EventType)>;

class Window {
private:
    NativeWindowHandle handle;
    
    std::unordered_map<std::type_index, WindowSubscriber> subscribers;

public:

    static Window& Get(NativeWindowHandle handle);

    static NativeWindowHandle CreateNativeWindow(WindowConfig& config);

    void poll();

    void setHandle(NativeWindowHandle& nativeHandle);

    template<is_window_event EventType>
    void Subscribe(TypedWindowSubscriber<EventType> subscriber);

    template<is_window_event EventType>
    void emit(EventType event);

};

template<is_window_event EventType>
void Window::Subscribe(TypedWindowSubscriber<EventType> subscriber) {
    subscribers.emplace(std::type_index(typeid(EventType)), subscriber);
}

template<is_window_event EventType>
void Window::emit(EventType event) {
    auto it = subscribers.find(std::type_index(typeid(EventType)));
    if (it == subscribers.end()) {
        return;
    }

    it->second(event);
}

} // namespace parteeengine::rendering
#pragma once

#include <concepts>

namespace parteeengine::rendering {

struct WindowEvent;

template <typename EventType>
concept is_window_event = std::derived_from<EventType, WindowEvent>;

class WindowEventHandler {
  public:
    virtual ~WindowEventHandler() = default;
    virtual void emit(WindowEvent* event) = 0;
};

} // namespace parteeengine::rendering
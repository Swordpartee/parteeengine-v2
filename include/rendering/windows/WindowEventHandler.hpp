#pragma once

#include <concepts>
#include <memory>

namespace parteeengine::rendering {

struct WindowEvent;

template <typename EventType>
concept is_window_event = std::derived_from<EventType, WindowEvent>;

class WindowEventHandler {
  public:
    virtual ~WindowEventHandler() = default;
    virtual void emit(std::unique_ptr<WindowEvent> event) = 0;

    
};

} // namespace parteeengine::rendering
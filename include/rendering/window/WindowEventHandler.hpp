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

    [[nodiscard]] virtual bool shouldForward(const WindowEvent& event) const = 0;

    virtual void windowDestroyed() = 0;
};

} // namespace parteeengine::rendering
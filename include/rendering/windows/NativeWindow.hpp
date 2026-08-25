#pragma once

#include "WindowEventHandler.hpp"
#include "rendering/windows/WindowConfig.hpp"

namespace parteeengine::rendering {

class NativeWindow {
  private:
    WindowEventHandler* eventHandler;

  public:
    void setEventHandler(WindowEventHandler* handler) { eventHandler = handler; }

    void poll();

    void config(const WindowConfig& config);

    static NativeWindow* Create();
};

} // namespace parteeengine::rendering
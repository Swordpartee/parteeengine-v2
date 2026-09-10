#pragma once

#include "WindowEventHandler.hpp"
#include "rendering/window/WindowConfig.hpp"

#include <memory>

namespace parteeengine::rendering {

class NativeWindow {
  private:
    struct Impl;

    WindowEventHandler* eventHandler = nullptr;
    std::unique_ptr<Impl> impl;

  public:
    NativeWindow();
    ~NativeWindow();

    void setEventHandler(WindowEventHandler* handler) { eventHandler = handler; }
    WindowEventHandler* getEventHandler() { return eventHandler; }

    Impl& getImpl() { return *impl; }

    void poll();

    void config(const WindowConfig& config);
    void close();

    static std::unique_ptr<NativeWindow> Create(const WindowDesc& config);
};

} // namespace parteeengine::rendering
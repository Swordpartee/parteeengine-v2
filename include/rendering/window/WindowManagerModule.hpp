#pragma once

#include "core/modules/ModuleBase.hpp"
#include "rendering/window/NativeWindowHandle.hpp"
#include "rendering/window/Window.hpp"

#include <vector>

namespace parteeengine::rendering {

class WindowManagerModule : public ModuleBase {
  private:
    std::vector<NativeWindowHandle> windows;

  public:
    void update(const ModuleInput&) {
      std::erase_if(windows, [](NativeWindowHandle handle) {
        return Window::Get(handle).poll();
      });
    };

    Window& generateWindow(const WindowConfig& config) {
        windows.emplace_back(Window::CreateNativeWindow(config));

        return Window::Get(windows.back());
    }
};

} // namespace parteeengine::rendering
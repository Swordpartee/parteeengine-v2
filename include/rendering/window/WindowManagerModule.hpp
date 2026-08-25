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
    void update(const ModuleInput&);

    Window& generateWindow(const WindowConfig& config);
};

} // namespace parteeengine::rendering
#pragma once

#include "core/modules/ModuleBase.hpp"
#include "rendering/window/NativeWindowUtil.hpp"
#include "rendering/window/WindowConfig.hpp"
#include "rendering/window/Window.hpp"

#include <vector>

namespace parteeengine::rendering {

class WindowManagerModule : public ModuleBase {
  private:
    std::vector<NativeWindowHandle> handles;

  public:
    Window& createWindow(WindowConfig config);

    void update();
};

} // namespace parteeengine::rendering
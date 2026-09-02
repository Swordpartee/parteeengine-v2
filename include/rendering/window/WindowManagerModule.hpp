#pragma once

#include "core/modules/ModuleBase.hpp"
#include "rendering/window/ModuleWindow.hpp"

namespace parteeengine::rendering {

class WindowManagerModule : public ModuleBase {
  private:
    std::vector<ModuleWindow> windows;

  public:
    void update(const ModuleInput& input) override;

    ModuleWindow* createWindow(const WindowDesc& config);
};

} // namespace parteeengine::rendering
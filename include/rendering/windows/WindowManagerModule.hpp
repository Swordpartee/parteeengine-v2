#pragma once

#include "core/modules/ModuleBase.hpp"
#include "rendering/windows/ModuleWindow.hpp"

#include <vector>

namespace parteeengine::rendering {

class WindowManagerModule : public ModuleBase {
  private:
    std::vector<ModuleWindow> windows;

  public:
    void update(const ModuleInput&);

    ModuleWindow& createWindow(const WindowConfig& config);
};

} // namespace parteeengine::rendering
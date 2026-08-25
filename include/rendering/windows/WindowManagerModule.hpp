#pragma once

#include "core/modules/ModuleBase.hpp"
#include "rendering/windows/ModuleWindow.hpp"

#include "util/plf_hive.h"

namespace parteeengine::rendering {

class WindowManagerModule : public ModuleBase {
  private:
    plf::hive<ModuleWindow> windows;

  public:
    void update(const ModuleInput&);

    ModuleWindow& createWindow(const WindowDesc& config);
};

} // namespace parteeengine::rendering
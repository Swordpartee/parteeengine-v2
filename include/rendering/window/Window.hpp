#pragma once

#include "rendering/window/NativeWindowUtil.hpp"

namespace parteeengine::rendering {

class Window {
  private:
    NativeWindowHandle handle;

  public:
    void setHandle(NativeWindowHandle);
};

} // namespace parteeengine::rendering
#pragma once

#include "rendering/window/NativeWindowUtil.hpp"
#include "rendering/window/WindowConfig.hpp"

namespace parteeengine::rendering {

const NativeWindowHandle createWindow(WindowConfig config);

} // namespace parteeengine::rendering

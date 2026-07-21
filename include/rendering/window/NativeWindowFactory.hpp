#pragma once

#include "rendering/window/NativeWindowUtil.hpp"
#include "rendering/window/Window.hpp"
#include "rendering/window/WindowConfig.hpp"

namespace parteeengine::rendering {

struct NativeWindowFactory {
    static NativeWindowHandle createNativeWindow(WindowConfig config);

    static Window* GetWindowInstance(NativeWindowHandle handle);
};

} // namespace parteeengine::rendering

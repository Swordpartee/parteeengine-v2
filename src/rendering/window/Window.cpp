#include "rendering/window/Window.hpp"
#include "rendering/window/NativeWindowUtil.hpp"

namespace parteeengine::rendering {

    void Window::setHandle(NativeWindowHandle windowHandle) {
        handle = windowHandle;
    }

} // namespace parteeengine::rendering
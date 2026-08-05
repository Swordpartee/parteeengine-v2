#include "rendering/window/Window.hpp"

namespace parteeengine::rendering {
    
void Window::setHandle(NativeWindowHandle& nativeHandle) {
    handle = nativeHandle;
}

}
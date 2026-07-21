#include "rendering/window/WindowManagerModule.hpp"

#include "rendering/window/WindowConfig.hpp"
#include "rendering/window/NativeWindowFactory.hpp"

namespace parteeengine::rendering {

    Window& WindowManagerModule::createWindow(WindowConfig config) {
        auto handle = NativeWindowFactory::createNativeWindow(config);

        handles.emplace_back(handle);

        return *NativeWindowFactory::GetWindowInstance(handle);
    }

    void WindowManagerModule::update() {
        for (auto handle : handles) {
            MSG msg = {};
            while (GetMessage(&msg, handle, 0, 0) > 0) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

} // namespace parteeengine::rendering
#include "rendering/window/NativeWindowFactory.hpp"
#include "rendering/window/NativeWindowUtil.hpp"

#include <stdexcept>
#include <winuser.h>


namespace parteeengine::rendering {

#if defined(PLATFORM_WINDOWS)

Window* NativeWindowFactory::GetWindowInstance(NativeWindowHandle windowHandle) {
    return reinterpret_cast<Window*>(GetWindowLongPtr(windowHandle, GWLP_USERDATA));
}

void SetWindowInstance(HWND windowHandle, Window* instance) {
    SetWindowLongPtr(windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(instance));
}

LRESULT CALLBACK WndProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Window* window;

    if (uMsg == WM_NCCREATE) {
        // Grab the void* tucked inside lParam and cast to the window
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        window = (Window*)pCreate->lpCreateParams;

        // Assign the native window's user data to it's window instance
        SetWindowInstance(windowHandle, window);
        window->setHandle(windowHandle);

    } else {
        window = NativeWindowFactory::GetWindowInstance(windowHandle);
    }

    if (!window) {
        return DefWindowProc(windowHandle, uMsg, wParam, lParam);
    }

    switch (uMsg) {
    case WM_CLOSE:
        // Destroys the window when the user clicks the close 'X' button
        DestroyWindow(windowHandle);
        // window->emit();
        return 0;

    case WM_DESTROY:
        // Posts a quit message to the application's message loop
        PostQuitMessage(0);
        return 0;

    default:
        // Passes all other messages to Windows for default processing
        return DefWindowProc(windowHandle, uMsg, wParam, lParam);
    }


}

NativeWindowHandle NativeWindowFactory::createNativeWindow(WindowConfig config) {
    const char* CLASS_NAME = "GlobalWindowClass";

    static auto instanceHandle = GetModuleHandle(nullptr);

    // This block executes exactly once across all method calls
    static bool isClassRegistered = [&]() {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WndProc;
        wc.hInstance = instanceHandle;
        wc.lpszClassName = CLASS_NAME;

        return RegisterClassEx(&wc) != 0;
    }();

    if (!isClassRegistered) {
        // throw std::runtime_error("Failed to register window class.");
    }

    Window* window = new Window();

    HWND windowHandle = CreateWindowEx(0, CLASS_NAME, config.title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                                       CW_USEDEFAULT, config.width, config.height, NULL, NULL, instanceHandle, window);

    if (!windowHandle) {
        // throw std::runtime_error("Failed to generate window.");
    }

    ShowWindow(windowHandle, SW_SHOW);

    return windowHandle;
}

#elif defined(PLATFORM_MACOS)

#elif defined(PLATFORM_LINUX)

#endif

} // namespace parteeengine::rendering
#include "rendering/window/WindowFactory.hpp"

namespace parteeengine::rendering {

#if defined(PLATFORM_WINDOWS)

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        // Destroys the window when the user clicks the close 'X' button
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        // Posts a quit message to the application's message loop
        PostQuitMessage(0);
        return 0;

    default:
        // Passes all other messages to Windows for default processing
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

const NativeWindowHandle createWindow(WindowConfig config) {
    const char* CLASS_NAME = "GlobalWindowClass";

    static auto instanceHandle = GetModuleHandle(nullptr);

    // This block executes exactly once across all method calls
    static bool isClassRegistered = [&]() {
        WNDCLASSEX wc = {sizeof(wc)};
        wc.lpfnWndProc = WndProc;
        wc.hInstance = instanceHandle;
        wc.lpszClassName = CLASS_NAME;

        return RegisterClassEx(&wc) != 0;
    }();

    if (!isClassRegistered) {
        return nullptr; // Registration failed
    }

    HWND windowHandle = CreateWindowEx(0,          // Optional window styles.
                                       CLASS_NAME, // Window class
                                       config.title.c_str(), // Window text
                                       WS_OVERLAPPEDWINDOW,  // Window style

                                       // Size and position
                                       CW_USEDEFAULT, CW_USEDEFAULT,
                                       config.width, config.height,

                                       NULL, // Parent window
                                       NULL, // Menu
                                       instanceHandle,
                                       NULL // Additional application data
    );

    if (!windowHandle) {
        return nullptr;
    }

    return windowHandle;
}

#elif defined(PLATFORM_MACOS)

#elif defined(PLATFORM_LINUX)

#endif

} // namespace parteeengine::rendering
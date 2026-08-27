#include "rendering/windows/WindowConfig.hpp"
#include "rendering/windows/NativeWindow.hpp"
#include "rendering/windows/WindowEvent.hpp"

#include <memory>
#include <stdexcept>
#include <windows.h>
#include <winuser.h>


namespace parteeengine::rendering {
// SetMessageHook(HWND, std::function<bool(UINT,WPARAM,LPARAM,LRESULT&)>)

struct NativeWindow::Impl {
    HWND hwnd = nullptr;
};

NativeWindow::NativeWindow() : eventHandler(nullptr), impl(std::make_unique<Impl>()) {}

NativeWindow::~NativeWindow() = default;

void NativeWindow::poll() {

    MSG msg = {};

    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
};

void NativeWindow::config(const WindowConfig& config) {
    if (config.dimensions) {
    }

    if (config.title) {
    }

    if (config.visible) {
    }
};

void NativeWindow::close() {
    DestroyWindow(impl->hwnd);
}

LRESULT CALLBACK WndProc(HWND handle, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    NativeWindow* window;

    if (uMsg == WM_NCCREATE) {
        // Grab the void* tucked inside lParam and cast to the window
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        window = (NativeWindow*)pCreate->lpCreateParams;

        // Assign the native window's user data to it's window instance
        SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        window->getImpl().hwnd = handle;

    } else {
        window = reinterpret_cast<NativeWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
    }

    if (!window) {
        return DefWindowProc(handle, uMsg, wParam, lParam);
    }

    auto handler = window->getEventHandler();

    if (!handler) {
        return DefWindowProc(handle, uMsg, wParam, lParam);
    }

    switch (uMsg) {
    case WM_CLOSE:
        handler->emit(std::make_unique<WindowCloseEvent>());
        if (handler->shouldForward(WindowCloseEvent{})) {
            break;
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        handler->emit(std::make_unique<WindowDestroyEvent>());
        delete window;
        return 0;
    
    }

    // Passes all other messages to Windows for default processing
    return DefWindowProc(handle, uMsg, wParam, lParam);

}

NativeWindow* NativeWindow::Create(const WindowDesc& config) {
    const char* CLASS_NAME = "GlobalWindowClass";

    static auto instanceHandle = GetModuleHandle("Parteeengine");

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
        throw std::runtime_error("Failed to register window class.");
    }

    auto window = new NativeWindow();

    HWND handle = CreateWindowEx(0, CLASS_NAME, config.title.c_str(), WS_OVERLAPPEDWINDOW, config.location.first,
                                       config.location.second, config.dimensions.first, config.dimensions.second, NULL, NULL, instanceHandle, window);

    if (!handle) {
        delete window;
        throw std::runtime_error("Failed to generate window.");
    }

    window->impl->hwnd = handle;

    if (config.visible) {
        ShowWindow(handle, SW_SHOW);
    }

    return window;

};

} // namespace parteeengine::rendering
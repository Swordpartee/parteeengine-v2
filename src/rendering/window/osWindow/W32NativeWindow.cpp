#include "rendering/window/NativeWindow.hpp"
#include "rendering/window/WindowConfig.hpp"
#include "rendering/window/WindowEvent.hpp"

#include <memory>

// NOLINTBEGIN(misc-include-cleaner, cppcoreguidelines-owning-memory)

#include <windows.h>

namespace parteeengine::rendering {
// SetMessageHook(HWND, std::function<bool(UINT,WPARAM,LPARAM,LRESULT&)>)

struct NativeWindow::Impl {
    HWND hwnd = nullptr;
};

NativeWindow::NativeWindow() : eventHandler(nullptr), impl(std::make_unique<Impl>()) {}

NativeWindow::~NativeWindow() = default;

void NativeWindow::poll() {

    MSG msg = {};

    while (GetMessage(&msg, this->impl->hwnd, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
};

void NativeWindow::config(const WindowConfig& config) {
    if (config.dimensions.has_value()) {
        SetWindowPos(impl->hwnd, HWND_TOP, 0, 0, config.dimensions->first, config.dimensions->first,
                     SWP_NOMOVE + SWP_NOOWNERZORDER);
    }

    if (config.location.has_value()) {
    }

    if (config.title.has_value()) {
        SetWindowTextA(impl->hwnd, config.title->data());
    }

    if (config.visible.has_value()) {
        if (config.visible) {
            ShowWindow(impl->hwnd, SW_HIDE);

        } else {
            ShowWindow(impl->hwnd, SW_SHOW);
        }
    }
};

void NativeWindow::close() { DestroyWindow(impl->hwnd); }

namespace {
LRESULT CALLBACK WndProc(HWND handle, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    NativeWindow* window = nullptr;

    if (uMsg == WM_NCCREATE) {
        // Grab the void* tucked inside lParam and cast to the window
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast, performance-no-int-to-ptr)
        auto* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = static_cast<NativeWindow*>(pCreate->lpCreateParams);
        // Assign the native window's user data to iter's window instance
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        window->getImpl().hwnd = handle;

    } else {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast, performance-no-int-to-ptr)
        window = reinterpret_cast<NativeWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
    }

    if (window == nullptr) {
        return DefWindowProc(handle, uMsg, wParam, lParam);
    }

    auto* handler = window->getEventHandler();

    if (handler == nullptr) {
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
    default:
        break;
    }

    // Passes all other messages to Windows for default processing
    return DefWindowProc(handle, uMsg, wParam, lParam);
}
} // namespace

NativeWindow* NativeWindow::Create(const WindowDesc& config) {
    const char* CLASS_NAME = "GlobalWindowClass";

    static auto* instanceHandle = GetModuleHandle("Parteeengine");

    // This block executes exactly once across all method calls
    const static bool isClassRegistered = [&]() {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WndProc;
        wc.hInstance = instanceHandle;
        wc.lpszClassName = CLASS_NAME;

        return RegisterClassEx(&wc) != 0;
    }();

    if (!isClassRegistered) {
        return nullptr;
    }

    auto* window = new NativeWindow();

    HWND handle = CreateWindowEx(0, CLASS_NAME, config.title.c_str(), WS_OVERLAPPEDWINDOW, config.location.first,
                                 config.location.second, config.dimensions.first, config.dimensions.second, nullptr,
                                 nullptr, instanceHandle, window);

    if (handle == nullptr) {
        delete window;
        return nullptr;
    }

    window->impl->hwnd = handle;

    if (config.visible) {
        ShowWindow(handle, SW_SHOW);
    }

    return window;
};

// NOLINTEND(misc-include-cleaner, cppcoreguidelines-owning-memory)

} // namespace parteeengine::rendering
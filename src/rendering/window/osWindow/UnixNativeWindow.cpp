#include "rendering/window/NativeWindow.hpp"
#include "rendering/window/WindowConfig.hpp"

namespace parteeengine::rendering {
// SetMessageHook(HWND, std::function<bool(UINT,WPARAM,LPARAM,LRESULT&)>)

struct NativeWindow::Impl {
};

NativeWindow::NativeWindow() : eventHandler(nullptr) {}

NativeWindow::~NativeWindow() = default;

void NativeWindow::poll() {

};

void NativeWindow::config(const WindowConfig& config) {
    (void)config;
};

void NativeWindow::close() { }

NativeWindow* NativeWindow::Create(const WindowDesc& config) {
    (void)config;

    return new NativeWindow;
};

} // namespace parteeengine::rendering
#include "rendering/window/NativeWindow.hpp"
#include "rendering/window/WindowConfig.hpp"

#include <memory>

// NOLINTBEGIN

namespace parteeengine::rendering {
// SetMessageHook(HWND, std::function<bool(UINT,WPARAM,LPARAM,LRESULT&)>)

struct NativeWindow::Impl {};

NativeWindow::NativeWindow() : eventHandler(nullptr) {}

NativeWindow::~NativeWindow() = default;

void NativeWindow::poll() {

};

void NativeWindow::config(const WindowConfig& /*config*/) {};

void NativeWindow::close() {}

std::unique_ptr<NativeWindow> NativeWindow::Create(const WindowDesc& /*config*/) {

    return std::make_unique<NativeWindow>();
};

} // namespace parteeengine::rendering

// NOLINTEND

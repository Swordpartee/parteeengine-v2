#include "rendering/window/WindowFactory.hpp"
#include "util/OSUtil.hpp"

#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;

    auto window = parteeengine::rendering::createWindow({800, 600, "sample window"});

    ShowWindow(window, SW_SHOW);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
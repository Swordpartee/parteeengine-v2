#include <iostream>

#include "rendering/window/WindowFactory.hpp"


int main() {
    std::cout << "Hello, World!" << std::endl;

    auto window = parteeengine::rendering::createWindow({600, 800, "sample window"});

    ShowWindow(window, SW_SHOW);

    while(true) {}

    return 0;
}
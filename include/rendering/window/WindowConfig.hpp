#pragma once

#include <string>

namespace parteeengine::rendering {

struct WindowConfig {
    int width, height;
    const std::string title;
};

} // namespace parteeengine::rendering
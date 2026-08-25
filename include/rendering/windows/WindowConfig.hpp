#pragma once

#include <optional>
#include <string>

namespace parteeengine::rendering {

struct WindowConfig {
    std::optional<int> width;
    std::optional<int> height;
    std::optional<std::string> title;
    std::optional<bool> visible;
};

} // namespace parteeengine::rendering
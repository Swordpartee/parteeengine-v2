#pragma once

#include <optional>
#include <string>

namespace parteeengine::rendering {

using Width = int;
using Height = int;
using X = int;
using Y = int;

struct WindowConfig {
    std::optional<std::pair<Width, Height>> dimensions;
    std::optional<std::pair<X, Y>> location;
    std::optional<std::string> title;
    std::optional<bool> visible;
};

struct WindowDesc {
    std::pair<Width, Height> dimensions;
    std::pair<X, Y> location;
    std::string title;
    bool visible;
};

} // namespace parteeengine::rendering
#pragma once

#include "util/Vector2.hpp"

namespace util {

struct Transform {
  public:
    Vector2 position;
    double rotation;
    Vector2 scale;
};

} // namespace util

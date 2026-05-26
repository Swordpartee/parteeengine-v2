#pragma once

namespace util
{
    class Vector4 {
    private:
       float x, y, z, w;

    public:

    Vector4(float x, float y, float z = 0.f, float w = 1.f) : x(x), y(y), z(z), w(w) {}

    }; 
} // namespace util

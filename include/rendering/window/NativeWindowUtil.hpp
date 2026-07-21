#pragma once

#include "rendering/window/NativeWindowUtil.hpp"
#include "util/OSUtil.hpp"

namespace parteeengine::rendering {

#if defined(PLATFORM_WINDOWS)

using NativeWindowHandle = HWND;

#elif defined(PLATFORM_MACOS)

using NativeWindowHandle = void*;

#elif defined(PLATFORM_LINUX)

using NativeWindowHandle = void*;

#endif

} // namespace parteeengine::rendering
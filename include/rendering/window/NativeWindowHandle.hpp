#pragma once 

#include "util/OSUtil.hpp"

namespace parteeengine::rendering {

#if defined (PLATFORM_WINDOWS) 

    using NativeWindowHandle = HWMD;

#elif defined (PLATFORM_LINUX)

#elif defined (PLATFORM_MAC)

#endif

}
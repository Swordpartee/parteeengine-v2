#pragma once

#if defined(_WIN32)

#define PLATFORM_WINDOWS 1

#include <windows.h>
#elif defined(__APPLE__)

#define PLATFORM_MACOS 1

#elif defined(__linux__)

#define PLATFORM_LINUX 1
#include <X11/Xlib.h>

#else

#error "Unknown Operating System"

#endif
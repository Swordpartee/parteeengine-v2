#pragma once

#if defined(_WIN32)

#define PLATFORM_WINDOWS 1
#define NOMINMAX
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#elif defined(__linux__)

#define PLATFORM_LINUX 1

#elif defined(__APPLE__)

#define PLATFORM_MACOS 1

#else

#error "Unknown Operating System"

#endif
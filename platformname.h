#pragma once

#if defined(__CYGWIN__) && !defined(_WIN32)
#define PLATFORM_NAME "Cygwin"
#elif defined(__MINGW32__)
#define PLATFORM_NAME "Windows (MinGW)"
#elif defined(_MSC_VER) && defined(__cplusplus_cli)
#define PLATFORM_NAME "Windows (C++/CLI)"
#elif defined(_MSC_VER) && defined(__cplusplus)
#define PLATFORM_NAME "Windows (Visual C++)"
#elif defined (_WIN32)
#define PLATFORM_NAME "Windows"
#elif defined(__linux__)
#define PLATFORM_NAME "Linux"
#elif defined(__APPLE__) && defined(__MACH__)
#define PLATFORM_NAME "Mac OS X / iOS"
#endif

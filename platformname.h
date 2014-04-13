#pragma once

// A header file to detect some common platforms and architectures.
// Helps tell if an executable is 32-bit or 64-bit, and whether it's compiled on Cygwin or not.

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

#if defined(__amd64) || defined(_M_AMD64)
#define ARCH_NAME "x86_64"
#elif defined(__i386) || defined(_M_IX86) || defined(_X86_)
#define ARCH_NAME "x86"
#endif

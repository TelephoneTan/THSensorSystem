#ifndef MACROS
#define MACROS

#ifdef __GNUC__
#define LOGME_GCC
#define V_GCC
#ifdef _WIN32
#define LOGME_WINDOWS
#define V_WINDOWS
#endif // _WIN32
#ifdef __linux__
#define LOGME_LINUX
#define V_LINUX
#endif // __linux__
#endif // __GNUC__
#ifdef _MSC_VER
#define LOGME_MSVC
#define V_MSVC
#ifdef _WIN32
#define LOGME_WINDOWS
#define V_WINDOWS
#endif // _WIN32
#ifdef __linux__
#define LOGME_LINUX
#define V_LINUX
#endif // __linux__
#endif // _MSC_VER
#endif // MACROS

#if !defined(LOGME_WINDOWS) && !defined(LOGME_LINUX)
#define LOGME_BARE_METAL
#define V_BARE_METAL
#endif // !defined(LOGME_WINDOWS) && !defined(LOGME_LINUX)

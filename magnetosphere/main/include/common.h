#if !defined(MAGNET_COMMON_H_)
#define MAGNET_COMMON_H_

// Detect and set platform
#define MAG_PLATFORM_WIN32 0
#define MAG_PLATFORM_UNIX  1

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#   define MAGNET_PLATFORM MAG_PLATFORM_WIN32
#else
#   pragma message("WARNING: Unknown platform!")
#   define MAGNET_PLATFORM MAG_PLATFORM_UNIX
#endif

// Detect and set compiler
#define MAG_COMPILER_MSVC    0
#define MAG_COMPILER_GNUC    1
//#define MAG_COMPILER_BORLAND 2

#if defined(_MSC_VER)
#   define MAGNET_COMPILER  MAG_COMPILER_MSVC
#   define COMPILER_VERSION _MSC_VER
/*#elif defined(__GNUC__)
#   define MAGNET_COMPILER  MAG_COMPILER_GNUC
#   define COMPILER_VERSION __VERSION__
#elif defined(__BORLANDC__)
#   define MAGNET_COMPILER  MAG_COMPILER_BORLAND
#   define COMPILER_VERSION __BCPLUSPLUS__*/
#else
#   pragma message("WARNING: Unknown compiler!")
#   define MAGNET_COMPILER  MAG_COMPILER_GNUC
#   define COMPILER_VERSION 0
#endif

// Set shared library specs
#if MAGNET_COMPILER == COMPILER_MSVC
#   define MAGNET_IMPORT _declspec(dllimport)
#   define MAGNET_EXPORT _declspec(dllexport)
#else
#   define MAGNET_IMPORT
#   define MAGNET_EXPORT
#endif
#ifdef MAIN_EXPORTS
#   define MAG_MAIN_EXPORT MAGNET_EXPORT
#else
#   define MAG_MAIN_EXPORT MAGNET_IMPORT
#endif

// Check for MSVC w/out stlport
/*#if MAGNET_COMPILER == MAG_COMPILER_MSVC
#   pragma once
#   include <map>
#   if !defined(_STLPORT_VERSION)
#       pragma message("WARNING: Building with Microsoft's STL implementation!\n  Please download, build, and install STLport from http://www.stlport.org/ as it is faster and more compatible.")
#   endif
#endif*/

// Set stlport for debugging if we are debugging
#if defined(DEBUG) || defined(_DEBUG)
#   define _STLP_DEBUG 1
#endif

#endif // !defined(MAGNET_COMMON_H_)
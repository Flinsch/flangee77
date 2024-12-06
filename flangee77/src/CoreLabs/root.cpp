#include "root.h"



#if defined(_MSC_VER)
#pragma message( "Compiled with MSVC" )
#elif defined(__clang__)
// Be mindful that __GNUC__ is also defined for Clang to ensure compatibility with GCC,
// so always check for __clang__ first if detecting Clang specifically.
#pragma message( "Compiled with Clang" )
#elif defined(__GNUC__)
#pragma message( "Compiled with GCC" )
#else
static_assert( false, "Unknown compiler" );
#endif

#if defined(_WIN32)
#pragma message( "Running on Windows" )
#elif defined(__linux__)
#pragma message( "Running on Linux" )
#elif defined(__APPLE__) && defined(__MACH__)
#pragma message( "Running on macOS" )
#else
static_assert( false, "Unknown operating system" );
#endif

#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
#pragma message( "64-bit system" )
#elif defined(_WIN32) || defined(__i386__) || defined(__ppc__)
#pragma message( "32-bit system" )
#else
static_assert( false, "Unknown bitness" )
#endif

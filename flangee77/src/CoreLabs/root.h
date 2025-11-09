#ifndef CL7_ROOT_H
#define CL7_ROOT_H



// NOLINTBEGIN(*-reserved-identifier)
#if !defined(NDEBUG) && !defined(_DEBUG)
// Neither set: assume debug build. GCC/Clang don't set _DEBUG, for example, while
// MSVC does; but all three set NDEBUG in the case of a non-debug build.
#define _DEBUG
#endif
#if (defined(NDEBUG) && defined(_DEBUG)) || (!defined(NDEBUG) && !defined(_DEBUG))
#error Inconsistent NDEBUG and _DEBUG definition.
#endif
// NOLINTEND(*-reserved-identifier)



#ifndef __BYTE_ORDER__
#if defined(_MSC_VER)
#define __ORDER_LITTLE_ENDIAN__ 1234
#define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
#endif
#endif



#define F77_IS_MSVC     0
#define F77_IS_CLANG    0
#define F77_IS_GCC      0

#define F77_IS_WINDOWS  0
#define F77_IS_LINUX    0
#define F77_IS_MACOS    0

#define F77_IS_64BIT    0
#define F77_IS_32BIT    0

#define F77_IS_LITTLE_ENDIAN    0
#define F77_IS_BIG_ENDIAN       0

#if defined(_MSC_VER)
#undef F77_IS_MSVC
#define F77_IS_MSVC 1
#define F77_COMPILER_MSVC 1
#elif defined(__clang__)
// Be mindful that __GNUC__ is also defined for Clang to ensure compatibility with GCC,
// so always check for __clang__ first if detecting Clang specifically.
#undef F77_IS_CLANG
#define F77_IS_CLANG 1
#define F77_COMPILER_CLANG 1
#elif defined(__GNUC__)
#undef F77_IS_GCC
#define F77_IS_GCC 1
#define F77_COMPILER_GCC 1
#endif

#if defined(_WIN32)
#undef F77_IS_WINDOWS
#define F77_IS_WINDOWS 1
#define F77_PLATFORM_WINDOWS 1
#elif defined(__linux__)
#undef F77_IS_LINUX
#define F77_IS_LINUX 1
#define F77_PLATFORM_LINUX 1
#elif defined(__APPLE__) && defined(__MACH__)
#undef F77_IS_MACOS
#define F77_IS_MACOS 1
#define F77_PLATFORM_MACOS 1
#endif

#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
#undef F77_IS_64BIT
#define F77_IS_64BIT 1
#define F77_ARCH_64BIT 1
#elif defined(_WIN32) || defined(__i386__) || defined(__ppc__)
#undef F77_IS_32BIT
#define F77_IS_32BIT 1
#define F77_ARCH_32BIT 1
#endif

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#undef F77_IS_LITTLE_ENDIAN
#define F77_IS_LITTLE_ENDIAN 1
#define F77_ENDIAN_LITTLE 1
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#undef F77_IS_BIG_ENDIAN
#define F77_IS_BIG_ENDIAN 1
#define F77_ENDIAN_BIG 1
#endif



// NOLINTBEGIN(*-reserved-identifier)
#ifndef _WIN32_WINNT
//#define _WIN32_WINNT _WIN32_WINNT_WIN7
//#define _WIN32_WINNT _WIN32_WINNT_WIN8
#define _WIN32_WINNT _WIN32_WINNT_WIN10
#endif // !_WIN32_WINNT
// NOLINTEND(*-reserved-identifier)
#include <sdkddkver.h>

#include <cassert>
#include <concepts>
#include <cstring>
#include <memory>



// "unknown pragma 'GCC'" etc.
#pragma warning(disable: 4068)

// "This object file does not define any previously undefined public symbols,
// so it will not be used by any link operation that consumes this library."
#pragma warning(disable: 4221)



// Stupid and unnecessary, I know.
// And yet: let me calm my paranoia!
static_assert(sizeof(signed) == 4);
static_assert(sizeof(unsigned) == 4);
static_assert(sizeof(int) == 4);
static_assert(sizeof(char) == 1);
static_assert(sizeof(short) == 2);
static_assert(sizeof(long) == 4);
static_assert(sizeof(long long) == 8);
static_assert(sizeof(float) == 4);
static_assert(sizeof(double) == 8);
static_assert(sizeof(long double) >= 8);



// For the following preprocessor definitions, see
// https://stackoverflow.com/a/5256500/480304
// Thank you for this, Jim Balter!

/**
 * Concatenate preprocessor tokens A and B without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define PPCAT_NX(A, B) A##B

/**
 * Concatenate preprocessor tokens A and B after macro-expanding them.
 */
#define PPCAT(A, B) PPCAT_NX(A, B)

/**
 * Turn A into a string literal without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define STRINGIZE_NX(A) #A

/**
 * Turn A into a string literal after macro-expanding it.
 */
#define STRINGIZE(A) STRINGIZE_NX(A)



#endif // CL7_ROOT_H

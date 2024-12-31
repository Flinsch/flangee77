#ifndef CL7_ROOT_H
#define CL7_ROOT_H



// NOLINTBEGIN(*-reserved-identifier)
#ifndef _WIN32_WINNT
//#define _WIN32_WINNT _WIN32_WINNT_WIN7
//#define _WIN32_WINNT _WIN32_WINNT_WIN8
#define _WIN32_WINNT _WIN32_WINNT_WIN10
#endif // !_WIN32_WINNT
// NOLINTEND(*-reserved-identifier)
#include <sdkddkver.h>

#include <cassert>

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

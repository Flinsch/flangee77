#pragma once
#ifndef CL7_ROOT_H
#define CL7_ROOT_H

#ifndef _WIN32_WINNT
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#endif // !_WIN32_WINNT
#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN

#include <assert.h>

#include <memory>



// "This object file does not define any previously undefined public symbols,
// so it will not be used by any link operation that consumes this library."
#pragma warning( disable: 4221 )



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

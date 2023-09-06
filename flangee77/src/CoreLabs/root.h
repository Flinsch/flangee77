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



#endif // CL7_ROOT_H

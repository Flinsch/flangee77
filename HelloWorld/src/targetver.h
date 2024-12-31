#ifndef HELLOWORLD_TARGETVER_H
#define HELLOWORLD_TARGETVER_H

// Including "SDKDDKVer.h" defines the highest available Windows platform.
// If you want to build your application for an earlier Windows platform, include "WinSDKVer.h", and
// before including "SDKDDKVer.h", set the _WIN32_WINNT macro to the platform you want to support.
#include <WinSDKVer.h>
// NOLINTBEGIN(*-reserved-identifier)
#ifndef _WIN32_WINNT
//#define _WIN32_WINNT _WIN32_WINNT_WIN7
//#define _WIN32_WINNT _WIN32_WINNT_WIN8
#define _WIN32_WINNT _WIN32_WINNT_WIN10
#endif // !_WIN32_WINNT
// NOLINTEND(*-reserved-identifier)
#include <SDKDDKVer.h>

#endif // HELLOWORLD_TARGETVER_H

#pragma once
#ifndef CL7_SSTREAM_H
#define CL7_SSTREAM_H

#include <CoreLabs/string.h>

#include <sstream>



namespace cl7 {



#ifdef UNICODE
typedef std::wstringstream sstream;
typedef std::wistringstream isstream;
typedef std::wostringstream osstream;
#else // => !UNICODE
typedef std::stringstream sstream;
typedef std::istringstream isstream;
typedef std::ostringstream osstream;
#endif // #else => !UNICODE

typedef std::stringstream csstream;
typedef std::wstringstream wsstream;

typedef std::istringstream cisstream;
typedef std::wistringstream wisstream;

typedef std::ostringstream cosstream;
typedef std::wostringstream wosstream;



} // namespace cl7

#endif // CL7_SSTREAM_H

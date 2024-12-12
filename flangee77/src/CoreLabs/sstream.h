#pragma once
#ifndef CL7_SSTREAM_H
#define CL7_SSTREAM_H

#include <CoreLabs/string.h>

#include <sstream>



namespace cl7 {



#ifdef UNICODE
    using sstream = std::wstringstream;
    using isstream = std::wistringstream;
    using osstream = std::wostringstream;
#else // => !UNICODE
    using sstream = std::stringstream;
    using isstream = std::istringstream;
    using osstream = std::ostringstream;
#endif // #else => !UNICODE

    using asstream = std::stringstream;
    using wsstream = std::wstringstream;

    using aisstream = std::istringstream;
    using wisstream = std::wistringstream;

    using aosstream = std::ostringstream;
    using wosstream = std::wostringstream;



} // namespace cl7

#endif // CL7_SSTREAM_H

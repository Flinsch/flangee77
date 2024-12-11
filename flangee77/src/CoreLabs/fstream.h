#pragma once
#ifndef CL7_FSTREAM_H
#define CL7_FSTREAM_H

#include <CoreLabs/string.h>

#include <fstream>



namespace cl7 {



#ifdef UNICODE
    using fstream = std::wfstream;
    using ifstream = std::wifstream;
    using ofstream = std::wofstream;
#else // => !UNICODE
    using fstream = std::fstream;
    using ifstream = std::ifstream;
    using ofstream = std::ofstream;
#endif // #else => !UNICODE

    using afstream = std::fstream;
    using wfstream = std::wfstream;

    using aifstream = std::ifstream;
    using wifstream = std::wifstream;

    using aofstream = std::ofstream;
    using wofstream = std::wofstream;



} // namespace cl7

#endif // CL7_FSTREAM_H

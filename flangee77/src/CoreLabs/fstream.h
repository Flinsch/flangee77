#pragma once
#ifndef CL7_FSTREAM_H
#define CL7_FSTREAM_H

#include <CoreLabs/string.h>

#include <fstream>



namespace cl7 {



#ifdef UNICODE
typedef std::wfstream fstream;
typedef std::wifstream ifstream;
typedef std::wofstream ofstream;
#else // => !UNICODE
typedef std::fstream fstream;
typedef std::ifstream ifstream;
typedef std::ofstream ofstream;
#endif // #else => !UNICODE

typedef std::fstream cfstream;
typedef std::wfstream wfstream;

typedef std::ifstream cifstream;
typedef std::wifstream wifstream;

typedef std::ofstream cofstream;
typedef std::wofstream wofstream;



} // namespace cl7

#endif // CL7_FSTREAM_H

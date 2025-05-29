#ifndef CL7_IO_OPENMODE_H
#define CL7_IO_OPENMODE_H

#include <CoreLabs/root.h>



namespace cl7::io {



enum struct OpenMode
{
    Read        = 0x1,
    Write       = 0x2,
    Append      = 0x2 | 0x4,
    Truncate    = 0x2 | 0x8,
};



inline OpenMode operator|(OpenMode a, OpenMode b)
{
    return static_cast<OpenMode>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
}

inline OpenMode operator&(OpenMode a, OpenMode b)
{
    return static_cast<OpenMode>(static_cast<unsigned>(a) & static_cast<unsigned>(b));
}



} // namespace cl7::io

#endif // CL7_IO_OPENMODE_H

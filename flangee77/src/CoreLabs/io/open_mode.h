#pragma once
#ifndef CL7_IO_OPENMODE_H
#define CL7_IO_OPENMODE_H

#include <CoreLabs/root.h>



namespace cl7 {
namespace io {



enum class open_mode
{
    read        = 0x1,
    write       = 0x2,
    append      = 0x2 | 0x4,
    truncate    = 0x2 | 0x8,
};



inline open_mode operator | (open_mode a, open_mode b)
{
    return static_cast<open_mode>( static_cast<unsigned>( a ) | static_cast<unsigned>( b ) );
}

inline open_mode operator & (open_mode a, open_mode b)
{
    return static_cast<open_mode>( static_cast<unsigned>( a ) & static_cast<unsigned>( b ) );
}



} // namespace io
} // namespace cl7

#endif // CL7_IO_OPENMODE_H

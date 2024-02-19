#pragma once
#ifndef CL7_IO_SEEKMODE_H
#define CL7_IO_SEEKMODE_H

#include <CoreLabs/root.h>



namespace cl7 {
namespace io {



enum class seek_mode
{
    begin,
    current,
    end,
};



} // namespace io
} // namespace cl7

#endif // CL7_IO_SEEKMODE_H

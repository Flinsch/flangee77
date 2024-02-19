#pragma once
#ifndef CL7_IO_IROM_H
#define CL7_IO_IROM_H

#include "./seek_mode.h"

#include <CoreLabs/byte_span.h>



namespace cl7 {
namespace io {



class irom
{
public:
    virtual ~irom() = default;



    // #############################################################################
    // Prototypes
    // #############################################################################
public:
    /**
     * Checks whether the file is "open" and can be used.
     */
    virtual bool is_good() const = 0;

    /**
     * Returns the size of the file, in bytes.
     */
    virtual size_t get_size() const = 0;

    /**
     * Returns the current byte position within the file (relative to the beginning).
     */
    virtual size_t get_position() const = 0;

    /**
     * (Re)sets the current byte position within the file (relative to the beginning).
     * Returns the new/current byte position.
     */
    virtual size_t set_position(size_t position) = 0;

    /**
     * (Re)sets the current byte position within the file by moving it by the
     * specified bytes relative to the file's beginning, end, or current position,
     * as specified.
     * Returns the new/current byte position.
     */
    virtual size_t seek(ptrdiff_t relative, const seek_mode seek_mode = seek_mode::current) = 0;

    /**
     * Reads data from the file (at the current position) into the specified buffer
     * and returns the number of bytes transferred.
     */
    virtual size_t read(cl7::byte_span buffer) = 0;

}; // class irom



} // namespace io
} // namespace cl7

#endif // CL7_IO_IROM_H

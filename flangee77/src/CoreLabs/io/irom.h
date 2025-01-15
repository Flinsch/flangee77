#ifndef CL7_IO_IROM_H
#define CL7_IO_IROM_H

#include "./seek_mode.h"

#include <CoreLabs/byte_span.h>



namespace cl7::io {



class irom
{

public:
    virtual ~irom() = default;


    /**
     * Checks whether the file is "open" and can be used.
     */
    virtual bool is_good() const = 0;

    /**
     * Checks whether the current byte position exceeds the size of the file.
     */
    virtual bool is_eof() const = 0;

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
    virtual size_t seek(ptrdiff_t relative, seek_mode seek_mode = seek_mode::current) = 0;

    /**
     * Reads data from the file (at the current position) into the specified buffer
     * and returns the number of bytes transferred.
     */
    virtual size_t read(cl7::byte_span buffer) = 0;

    /**
     * Reads a single byte from the file (at the current position) and returns the
     * number of bytes transferred (i.e. 0 or 1).
     */
    virtual size_t read(std::byte& byte) = 0;

    /**
     * "Peeks" a single byte from the file (at the current position) without
     * extracting it. Returns the number of bytes that would have been extracted if
     * actually read (i.e. 0 or 1).
     */
    virtual size_t peek(std::byte& byte) = 0;

}; // class irom



} // namespace cl7::io

#endif // CL7_IO_IROM_H

#ifndef CL7_IO_IREADABLE_H
#define CL7_IO_IREADABLE_H

#include "./SeekMode.h"

#include <CoreLabs/byte_span.h>



namespace cl7::io {



class IReadable
{

public:
    virtual ~IReadable() = default;


    /**
     * Checks whether the source is readable, i.e. whether the device is open or
     * whatever.
     */
    virtual bool is_readable() const = 0;

    /**
     * Checks whether EOF has been reached, so that there is no more data to read at
     * the current byte position.
     */
    virtual bool is_eof() const = 0;

    /**
     * Returns the total readable size of the source, in bytes.
     */
    virtual size_t get_total_readable_bytes() const = 0;

    /**
     * Returns the number of bytes that can be read from the source at the current
     * read position.
     */
    virtual size_t get_readable_bytes_remaining() const = 0;

    /**
     * Returns the current read position within the source (in bytes, relative to
     * the beginning).
     */
    virtual size_t get_read_position() const = 0;

    /**
     * (Re)sets the current read position within the source (in bytes, relative to
     * the beginning). Returns the new/current byte position.
     */
    virtual size_t set_read_position(size_t position) = 0;

    /**
     * (Re)sets the current read position within the source by moving it by the
     * specified bytes relative to the beginning, end, or current position, as
     * specified. Returns the new/current byte position.
     */
    virtual size_t seek_read(ptrdiff_t relative, SeekMode seek_mode = SeekMode::Current) = 0;

    /**
     * Reads data from the source (at the current position) into the specified
     * buffer and returns the number of bytes actually read.
     */
    virtual size_t read(cl7::byte_span buffer) = 0;

    /**
     * Reads a single byte from the source (at the current position) and returns the
     * number of bytes actually read (i.e. 0 or 1).
     */
    virtual size_t read(std::byte& byte) = 0;

    /**
     * "Peeks" a single byte from the source (at the current position) without
     * extracting it. Returns the number of bytes that would have been extracted if
     * actually read (i.e. 0 or 1).
     */
    virtual size_t peek(std::byte& byte) = 0;

}; // class IReadable



} // namespace cl7::io

#endif // CL7_IO_IREADABLE_H

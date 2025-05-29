#ifndef CL7_IO_IWRITABLE_H
#define CL7_IO_IWRITABLE_H

#include "./SeekMode.h"

#include <CoreLabs/byte_view.h>



namespace cl7::io {



class IWritable
{

public:
    virtual ~IWritable() = default;


    /**
     * Checks whether the target is writable, i.e. whether the device is open or
     * whatever.
     */
    virtual bool is_writable() const = 0;

    /**
     * Returns the total writable size of the target, in bytes.
     */
    virtual size_t get_total_writable_bytes() const = 0;

    /**
     * Returns the number of bytes that can be written to the target at the current
     * write position.
     */
    virtual size_t get_writable_bytes_remaining() const = 0;

    /**
     * Returns the current write position within the target (in bytes, relative to
     * the beginning).
     */
    virtual size_t get_write_position() const = 0;

    /**
     * (Re)sets the current write position within the target (in bytes, relative to
     * the beginning). Returns the new/current byte position.
     */
    virtual size_t set_write_position(size_t position) = 0;

    /**
     * (Re)sets the current write position within the target by moving it by the
     * specified bytes relative to the beginning, end, or current position, as
     * specified. Returns the new/current byte position.
     */
    virtual size_t seek_write(ptrdiff_t relative, SeekMode seek_mode = SeekMode::Current) = 0;

    /**
     * Writes data to the target (at the current position) from the specified buffer
     * and returns the number of bytes actually written.
     */
    virtual size_t write(cl7::byte_view buffer) = 0;

    /**
     * Writes a single byte to the target (at the current position) and returns the
     * number of bytes actually written (i.e. 0 or 1).
     */
    virtual size_t write(std::byte byte) = 0;

}; // class IWritable



} // namespace cl7::io

#endif // CL7_IO_IWRITABLE_H

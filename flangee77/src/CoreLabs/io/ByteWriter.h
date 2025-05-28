#ifndef CL7_IO_BYTEWRITER_H
#define CL7_IO_BYTEWRITER_H

#include "./IWritable.h"

#include <CoreLabs/byte_view.h>
#include <CoreLabs/bits.h>



namespace cl7::io {



/**
 * A utility class for writing single or multiple bytes to a file (or similar).
 * Also supports writing various integral and floating-point types with optional
 * endian-awareness.
 */
class ByteWriter
{

public:
    /**
     * Prepares a byte writer for writing to the specified writable object.
     */
    explicit ByteWriter(IWritable* writable) noexcept;

  /**
     * Attempts to write a single byte. Returns 1 on success, 0 on failure.
     */
    size_t write_byte(std::byte byte) const;

    /**
     * Attempts to write multiple bytes from a buffer. Returns the number of bytes
     * actually written.
     */
    size_t write_bytes(cl7::byte_view data) const;

    /**
     * Attempts to write a trivially copyable scalar value (e.g., integer, float)
     * with optional endian conversion. Returns the number of bytes successfully
     * written.
     */
    template <std::endian target_endian = std::endian::native, typename T>
    size_t write_scalar(T value) const
    {
        static_assert(std::is_trivially_copyable_v<T>);
        value = cl7::bits::swap_bytes_unless_endian<target_endian>(value);
        return _writable->write(cl7::make_byte_view(&value));
    }

private:
    /** The writable target object. */
    IWritable* _writable;

}; // class ByteWriter



} // namespace cl7::io

#endif //CL7_IO_BYTEWRITER_H

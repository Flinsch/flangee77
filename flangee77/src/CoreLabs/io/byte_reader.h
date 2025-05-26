#ifndef CL7_IO_BYTEREADER_H
#define CL7_IO_BYTEREADER_H

#include "./irom.h"

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_span.h>
#include <CoreLabs/bits.h>



namespace cl7::io {



/**
 * A utility class for reading single or multiple bytes from a read-only memory.
 * Also supports reading various integral and floating-point types with optional
 * endian-awareness.
 */
class byte_reader
{

public:
    /**
     * Prepares a byte reader for reading from the specified read-only memory.
     */
    explicit byte_reader(irom* rom) noexcept;

    /**
     * Reads and returns a single byte. There is no guarantee that the operation was
     * successful. In case of an error or out-of-bounds, 0 is returned. However, a
     * value other than 0 is not a reliable indicator of success.
     */
    std::byte read_byte() const;

    /**
     * Attempts to fill the given buffer with bytes. Returns the number of bytes
     * actually read.
     */
    size_t read_bytes(cl7::byte_span buffer) const;

    /**
     * Reads and returns all available/remaining bytes until EOF or failure.
     */
    cl7::byte_vector read_all() const;

    /**
     * Reads and returns a trivially copyable scalar value (e.g., integer, float)
     * with optional endian conversion. There is no guarantee that the operation was
     * successful. In case of an error or out-of-bounds, 0 is returned. However, a
     * value other than 0 is not a reliable indicator of success.
     */
    template <typename T, std::endian source_endian = std::endian::native>
    T read_scalar() const
    {
        static_assert(std::is_trivially_copyable_v<T>);
        T value{0};
        _rom->read(cl7::make_byte_span(&value));
        return cl7::bits::swap_bytes_unless_endian<source_endian>(value);
    }

    /**
     * Attempts to read a trivially copyable scalar value (e.g., integer, float)
     * into the given reference and performs optional endian conversion. Returns the
     * number of bytes actually read.
     */
    template <std::endian source_endian = std::endian::native, typename T>
    size_t read_scalar(T& value) const
    {
        static_assert(std::is_trivially_copyable_v<T>);
        const auto read = _rom->read(cl7::make_byte_span(&value));
        value = cl7::bits::swap_bytes_unless_endian<source_endian>(value);
        return read;
    }

private:
    /** The read-only memory to read from. */
    irom* _rom;

}; // class byte_reader



} // namespace cl7::io

#endif //CL7_IO_BYTEREADER_H

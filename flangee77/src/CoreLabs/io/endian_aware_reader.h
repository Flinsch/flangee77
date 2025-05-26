#ifndef CL7_IO_ENDIANAWAREREADER_H
#define CL7_IO_ENDIANAWAREREADER_H

#include "./byte_reader.h"



namespace cl7::io {



/**
 * Wraps a `byte_reader` and reads scalar types using the specified fixed endian
 * order. Simplifies repeated reads from a known-endian source.
 */
template <std::endian source_endian>
class endian_aware_reader
{

public:
    /**
     * Prepares an endian-aware reader for reading from the specified read-only memory.
     */
    explicit endian_aware_reader(irom* rom) noexcept : _byte_reader(rom) {}

    /**
     * Reads and returns a single byte. There is no guarantee that the operation was
     * successful. In case of an error or out-of-bounds, 0 is returned. However, a
     * value other than 0 is not a reliable indicator of success.
     */
    std::byte read_byte() const { return _byte_reader.read_byte(); }

    /**
     * Attempts to fill the given buffer with bytes. Returns the number of bytes
     * actually read.
     */
    size_t read_bytes(cl7::byte_span buffer) const { return _byte_reader.read_bytes(buffer); }

    /**
     * Reads and returns all available/remaining bytes until EOF or failure.
     */
    cl7::byte_vector read_all() const { return _byte_reader.read_all(); }

    /**
     * Reads and returns a trivially copyable scalar value (e.g., integer, float)
     * with optional endian conversion. There is no guarantee that the operation was
     * successful. In case of an error or out-of-bounds, 0 is returned. However, a
     * value other than 0 is not a reliable indicator of success.
     */
    template <typename T>
    T read_scalar() const { return _byte_reader.read_scalar<T, source_endian>(); }

    /**
     * Attempts to read a trivially copyable scalar value (e.g., integer, float)
     * into the given reference and performs optional endian conversion. Returns the
     * number of bytes actually read.
     */
    template <typename T>
    size_t read_scalar(T& value) const { return _byte_reader.read_scalar<source_endian>(value); }

private:
    /** The wrapped byte reader. */
    byte_reader _byte_reader;

}; // class endian_aware_reader



} // namespace cl7::io

#endif // CL7_IO_ENDIANAWAREREADER_H

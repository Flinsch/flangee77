#ifndef CL7_IO_ENDIANAWAREWRITER_H
#define CL7_IO_ENDIANAWAREWRITER_H

#include "./ByteWriter.h"



namespace cl7::io {



/**
 * Wraps a `ByteWriter` and writes scalar types using the specified fixed endian
 * order. Useful when writing to a format with a predefined byte order.
 */
template <std::endian target_endian>
class EndianAwareWriter
{

public:
    /**
     * Prepares an endian-aware writer for writing to the specified writable object.
     */
    explicit EndianAwareWriter(IWritable* writable) noexcept : _byte_writer(writable) {}

    /**
     * Attempts to write a single byte. Returns 1 on success, 0 on failure.
     */
    size_t write_byte(std::byte byte) { return _byte_writer.write_byte(byte); }

    /**
     * Attempts to write multiple bytes from a buffer. Returns the number of bytes
     * actually written.
     */
    size_t write_bytes(cl7::byte_view data) { return _byte_writer.write_bytes(data); }

    /**
     * Attempts to write a trivially copyable scalar value (e.g., integer, float)
     * with optional endian conversion. Returns the number of bytes successfully
     * written.
     */
    template <typename T>
    size_t write_scalar(T value) { return _byte_writer.write_scalar<target_endian>(value); }

private:
    /** The wrapped byte writer. */
    ByteWriter _byte_writer;

}; // class EndianAwareWriter



} // namespace cl7::io

#endif // CL7_IO_ENDIANAWAREWRITER_H

#include "ByteWriter.h"



namespace cl7::io {



    /**
     * Prepares a byte writer for writing to the specified file.
     */
    ByteWriter::ByteWriter(IWritable* writable) noexcept
        : _writable(writable)
    {
    }

    /**
     * Attempts to write multiple bytes from a buffer. Returns the number of bytes
     * actually written.
     */
    size_t ByteWriter::write_bytes(cl7::byte_view data) const
    {
        return _writable->write(data);
    }

    /**
     * Attempts to write a single byte. Returns 1 on success, 0 on failure.
     */
    size_t ByteWriter::write_byte(std::byte byte) const
    {
        return _writable->write(cl7::make_byte_view(&byte));
    }



} // namespace cl7::io

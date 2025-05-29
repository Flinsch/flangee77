#include "ByteReader.h"



namespace cl7::io {



    /**
     * Prepares a byte reader for reading from the specified readable object.
     */
    ByteReader::ByteReader(IReadable* readable) noexcept
        : _readable(readable)
    {
    }

    /**
     * Reads and returns a single byte. There is no guarantee that the operation was
     * successful. In case of an error or out-of-bounds, 0 is returned. However, a
     * value other than 0 is not a reliable indicator of success.
     */
    std::byte ByteReader::read_byte() const
    {
        std::byte byte{0};
        _readable->read(cl7::make_byte_span(&byte));
        return byte;
    }

    /**
     * Attempts to fill the given buffer with bytes. Returns the number of bytes
     * actually read.
     */
    size_t ByteReader::read_bytes(cl7::byte_span buffer) const
    {
        const auto remaining = _readable->get_readable_bytes_remaining();
        const auto possible = (std::min)(remaining, buffer.size());
        const auto read = _readable->read(buffer);
        assert(read == possible);
        return read;
    }

    /**
     * Reads and returns all available/remaining bytes until EOF or failure.
     */
    cl7::byte_vector ByteReader::read_all() const
    {
        const auto remaining = _readable->get_readable_bytes_remaining();
        cl7::byte_vector data{remaining};
        const auto read = _readable->read(data);
        assert(read == remaining);
        return data;
    }



} // namespace cl7::io

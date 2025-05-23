#include "byte_reader.h"



namespace cl7::io {



    /**
     * Prepares a byte reader for reading from the specified read-only memory.
     */
    byte_reader::byte_reader(irom* rom) noexcept
        : _rom(rom)
    {
    }

    /**
     * Reads and returns a single byte. There is no guarantee that the operation was
     * successful. In case of an error or out-of-bounds, 0 is returned. However, a
     * value other than 0 is not a reliable indicator of success.
     */
    std::byte byte_reader::read_byte() const
    {
        std::byte byte{0};
        _rom->read(cl7::make_byte_span(&byte));
        return byte;
    }

    /**
     * Attempts to fill the given buffer with bytes. Returns the number of bytes
     * actually read.
     */
    size_t byte_reader::read_bytes(cl7::byte_span buffer) const
    {
        const auto remaining = _rom->get_size() - _rom->get_position();
        assert(remaining <= _rom->get_size());
        const auto possible = (std::min)(remaining, buffer.size());
        const auto read = _rom->read(buffer);
        assert(read == possible);
        return read;
    }

    /**
     * Reads and returns all available/remaining bytes until EOF or failure.
     */
    cl7::byte_vector byte_reader::read_all() const
    {
        const auto remaining = _rom->get_size() - _rom->get_position();
        assert(remaining <= _rom->get_size());
        cl7::byte_vector data{remaining};
        const auto read = _rom->read(data);
        assert(read == remaining);
        return data;
    }



} // namespace cl7::io

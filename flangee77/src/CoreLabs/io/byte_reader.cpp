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
     * Reads and returns all available/remaining data.
     */
    cl7::byte_vector byte_reader::read() const
    {
        const auto remaining = _rom->get_size() - _rom->get_position();
        assert(remaining <= _rom->get_size());
        cl7::byte_vector data{remaining};
        const auto read = _rom->read(data);
        assert(read == remaining);
        return data;
    }

    /**
     * Attempts to fill the given data buffer and returns the number of bytes
     * actually transferred.
     */
    size_t byte_reader::read(cl7::byte_span data) const
    {
        const auto remaining = _rom->get_size() - _rom->get_position();
        assert(remaining <= _rom->get_size());
        const auto possible = (std::min)(remaining, data.size());
        const auto read = _rom->read(data);
        assert(read == possible);
        return read;
    }



} // namespace cl7::io

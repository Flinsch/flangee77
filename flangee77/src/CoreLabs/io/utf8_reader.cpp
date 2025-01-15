#include "utf8_reader.h"

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/strings.h>



namespace cl7::io {



    /**
     * Prepares a UTF-8 reader for reading from the specified read-only memory.
     */
    utf8_reader::utf8_reader(irom* rom) noexcept
        : _rom(rom)
    {
    }

    /**
     * Reads and returns all available/remaining data.
     */
    cl7::u8string utf8_reader::read() const
    {
        const auto remaining = _rom->get_size() - _rom->get_position();
        assert(remaining <= _rom->get_size());
        cl7::byte_vector data{remaining};
        const auto read = _rom->read(data);
        assert(read == remaining);
        return cl7::strings::to_utf8_unchecked(data);
    }

    /**
     * Attempts to read the next line.
     */
    cl7::u8string utf8_reader::read_line() const
    {
        cl7::byte_vector data;
        std::byte byte;
        while (_rom->read(byte) && !cl7::strings::is_line_break(static_cast<cl7::u8char_type>(byte)))
        {
            if (data.size() == data.capacity())
                data.reserve(data.capacity() + 128);
            data.push_back(byte);
        }

        if (cl7::strings::is_line_break(static_cast<cl7::u8char_type>(byte)))
        {
            std::byte byte1;
            if (_rom->peek(byte1) && cl7::strings::is_line_break_strict(static_cast<cl7::u8char_type>(byte), static_cast<cl7::u8char_type>(byte1)))
                _rom->read(byte1);
        }

        return cl7::strings::to_utf8_unchecked(data);
    }



} // namespace cl7::io

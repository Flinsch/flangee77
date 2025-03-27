#include "utf8_reader.h"

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/strings.h>

#include <CoreLabs/strings/Analyzer.h>
#include <CoreLabs/strings/Transcoder.h>



namespace cl7::io {



    /**
     * Prepares a UTF-8 reader for reading from the specified read-only memory.
     */
    utf8_reader::utf8_reader(irom* rom) noexcept
        : _rom(rom)
    {
    }

    /**
     * Reads all available/remaining data without checking for valid UTF-8.
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
     * Attempts to read the next line without checking for valid UTF-8.
     */
    cl7::u8string utf8_reader::read_line() const
    {
        cl7::byte_vector data;
        std::byte byte;
        while (_rom->read(byte) && !cl7::strings::is_line_break(static_cast<cl7::u8char_t>(byte)))
        {
            if (data.size() == data.capacity())
                data.reserve(data.capacity() + 128);
            data.push_back(byte);
        }

        if (cl7::strings::is_line_break(static_cast<cl7::u8char_t>(byte)))
        {
            std::byte byte1;
            if (_rom->peek(byte1) && cl7::strings::is_line_break_strict(static_cast<cl7::u8char_t>(byte), static_cast<cl7::u8char_t>(byte1)))
                _rom->read(byte1);
        }

        return cl7::strings::to_utf8_unchecked(data);
    }

    static cl7::u8string _validate_utf8(cl7::u8string&& utf8_unchecked)
    {
        if (cl7::strings::Analyzer<cl7::u8char_t, cl7::strings::ErrorHandler>().validate(utf8_unchecked))
            return std::move(utf8_unchecked);

        return cl7::strings::Transcoder<cl7::u8char_t, cl7::u8char_t>().transcode(utf8_unchecked);
    }

    /**
     * Reads all available/remaining data with UTF-8 validation/"correction".
     */
    cl7::u8string utf8_reader::read_validated() const
    {
        return _validate_utf8(read());
    }

    /**
     * Attempts to read the next line with UTF-8 validation/"correction".
     */
    cl7::u8string utf8_reader::read_line_validated() const
    {
        return _validate_utf8(read_line());
    }



} // namespace cl7::io

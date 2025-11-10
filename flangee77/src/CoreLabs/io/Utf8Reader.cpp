#include "Utf8Reader.h"

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/text/codec.h>
#include <CoreLabs/text/codec/Analyzer.h>
#include <CoreLabs/text/codec/PlainErrorHandler.h>
#include <CoreLabs/text/codec/Transcoder.h>
#include <CoreLabs/text/inspect.h>



namespace cl7::io {



    /**
     * Prepares a UTF-8 reader for reading from the specified readable object.
     */
    Utf8Reader::Utf8Reader(IReadable* readable) noexcept
        : _readable(readable)
    {
    }

    /**
     * Reads all available/remaining data without checking for valid UTF-8.
     */
    cl7::u8string Utf8Reader::read_all() const
    {
        const auto remaining = _readable->get_readable_bytes_remaining();
        cl7::byte_vector data{remaining};
        const auto read = _readable->read(data);
        assert(read == remaining);
        return cl7::text::codec::to_utf8_unchecked(data);
    }

    /**
     * Attempts to read the next line without checking for valid UTF-8.
     */
    cl7::u8string Utf8Reader::read_line() const
    {
        cl7::byte_vector data;
        std::byte byte;
        while (_readable->read(byte) && !cl7::text::inspect::is_line_break(static_cast<cl7::u8char_t>(byte)))
        {
            if (data.size() == data.capacity())
                data.reserve(data.capacity() + 128);
            data.push_back(byte);
        }

        if (cl7::text::inspect::is_line_break(static_cast<cl7::u8char_t>(byte)))
        {
            std::byte byte1;
            if (_readable->peek(byte1) && cl7::text::inspect::is_line_break_strict(static_cast<cl7::u8char_t>(byte), static_cast<cl7::u8char_t>(byte1)))
                _readable->read(byte1);
        }

        return cl7::text::codec::to_utf8_unchecked(data);
    }

    /**
     * Attempts to read a single non-whitespace token/word (potentially skipping
     * preceding whitespace) without checking for valid UTF-8.
     * However, due to the logic used to check for whitespace, it is practically
     * guaranteed that only valid UTF-8 code points will be processed.
     */
    cl7::u8string Utf8Reader::read_token() const
    {
        cl7::u8string tmp;
        while (!(tmp = peek_codepoint()).empty() && cl7::text::inspect::is_whitespace_prefix(tmp) == tmp.length())
            _readable->seek_read(static_cast<ptrdiff_t>(tmp.length()));

        cl7::u8string result;
        while (!(tmp = peek_codepoint()).empty() && cl7::text::inspect::is_whitespace_prefix(tmp) == 0)
        {
            _readable->seek_read(static_cast<ptrdiff_t>(tmp.length()));
            if (result.size() + tmp.length() > result.capacity())
                result.reserve(result.capacity() + 16);
            result.append(tmp);
        }

        return result;
    }

    static cl7::u8string _validate_utf8(cl7::u8string&& utf8_unchecked)
    {
        if (cl7::text::codec::Analyzer<cl7::u8char_t, cl7::text::codec::PlainErrorHandler>().validate(utf8_unchecked))
            return std::move(utf8_unchecked);

        return cl7::text::codec::Transcoder<cl7::u8char_t, cl7::u8char_t>().transcode(utf8_unchecked);
    }

    /**
     * Reads all available/remaining data with UTF-8 validation/"correction".
     */
    cl7::u8string Utf8Reader::read_all_validated() const
    {
        return _validate_utf8(read_all());
    }

    /**
     * Attempts to read the next line with UTF-8 validation/"correction".
     */
    cl7::u8string Utf8Reader::read_line_validated() const
    {
        return _validate_utf8(read_line());
    }

    /**
     * Attempts to read a single non-whitespace token/word (potentially skipping
     * preceding whitespace) with UTF-8 validation/"correction".
     */
    cl7::u8string Utf8Reader::read_token_validated() const
    {
        return _validate_utf8(read_token());
    }

    /**
     * Attempts to "peek" a single Unicode codepoint without extracting it.
     */
    cl7::u8string_view Utf8Reader::peek_codepoint() const
    {
        static cl7::u8char_t buffer[6];

        const auto read_position = _readable->get_read_position();

        cl7::byte_vector data(6);
        const size_t max_count = _readable->read(data);

        size_t count = 0;
        for (size_t i = 0; i < max_count; ++i)
        {
            cl7::u8string_view sv{reinterpret_cast<const cl7::u8char_t*>(data.data()), i + 1};
            if (cl7::text::codec::Analyzer<cl7::u8char_t, cl7::text::codec::PlainErrorHandler>().validate(sv))
            {
                buffer[i] = *reinterpret_cast<const cl7::u8char_t*>(&data[i]);
                break;
            }
        }

        _readable->set_read_position(read_position);

        return {buffer, count};
    }

    /**
     * "Peeks" a single character without extracting it. Returns 0 if not possible.
     */
    cl7::u8char_t Utf8Reader::peek_char() const
    {
        std::byte byte;
        if (_readable->peek(byte) != 1)
            return 0;

        return static_cast<cl7::u8char_t>(byte);
    }



} // namespace cl7::io

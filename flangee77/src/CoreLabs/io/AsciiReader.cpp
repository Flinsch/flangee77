#include "AsciiReader.h"

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/text/codec.h>
#include <CoreLabs/text/codec/Analyzer.h>
#include <CoreLabs/text/codec/PlainErrorHandler.h>
#include <CoreLabs/text/codec/Transcoder.h>
#include <CoreLabs/text/inspect.h>



namespace cl7::io {



    /**
     * Prepares an ASCII reader for reading from the specified readable object.
     */
    AsciiReader::AsciiReader(IReadable* readable) noexcept
        : _readable(readable)
    {
    }

    /**
     * Reads all available/remaining data without checking for valid ASCII.
     */
    cl7::astring AsciiReader::read_all()
    {
        const auto remaining = _readable->get_readable_bytes_remaining();
        cl7::byte_vector data{remaining};
        const auto read = _readable->read(data);
        assert(read == remaining);
        return cl7::text::codec::to_ascii_unchecked(data);
    }

    /**
     * Attempts to read the next line without checking for valid ASCII.
     */
    cl7::astring AsciiReader::read_line()
    {
        cl7::byte_vector data;
        std::byte byte;
        while (_readable->read(byte) && !cl7::text::inspect::is_line_break(static_cast<cl7::achar_t>(byte)))
        {
            if (data.size() == data.capacity())
                data.reserve(data.capacity() + 128);
            data.push_back(byte);
        }

        if (cl7::text::inspect::is_line_break(static_cast<cl7::achar_t>(byte)))
        {
            std::byte byte1;
            if (_readable->peek(byte1) && cl7::text::inspect::is_line_break_strict(static_cast<cl7::achar_t>(byte), static_cast<cl7::achar_t>(byte1)))
                _readable->read(byte1);
        }

        return cl7::text::codec::to_ascii_unchecked(data);
    }

    /**
     * Attempts to read a single non-whitespace token/word (potentially skipping
     * preceding whitespace) without checking for valid ASCII.
     */
    cl7::astring AsciiReader::read_token()
    {
        std::byte byte;
        while (_readable->peek(byte) && cl7::text::inspect::is_whitespace(static_cast<cl7::achar_t>(byte)))
            _readable->read(byte);

        cl7::byte_vector data;
        while (_readable->peek(byte) && !cl7::text::inspect::is_whitespace(static_cast<cl7::achar_t>(byte)))
        {
            _readable->read(byte);
            if (data.size() == data.capacity())
                data.reserve(data.capacity() + 16);
            data.push_back(byte);
        }

        return cl7::text::codec::to_ascii_unchecked(data);
    }

    /**
     * Attempts to read a single character. Returns 0 if not possible.
     */
    cl7::achar_t AsciiReader::read_char()
    {
        std::byte byte;
        if (_readable->read(byte) != 1)
            return 0;

        return static_cast<cl7::achar_t>(byte);
    }

    static cl7::astring _validate_ascii(cl7::astring&& ascii_unchecked)
    {
        if (cl7::text::codec::Analyzer<cl7::achar_t, cl7::text::codec::PlainErrorHandler>().validate(ascii_unchecked))
            return std::move(ascii_unchecked);

        return cl7::text::codec::Transcoder<cl7::achar_t, cl7::achar_t>().transcode(ascii_unchecked);
    }

    /**
     * Reads all available/remaining data with ASCII validation/"correction".
     */
    cl7::astring AsciiReader::read_all_validated()
    {
        return _validate_ascii(read_all());
    }

    /**
     * Attempts to read the next line with ASCII validation/"correction".
     */
    cl7::astring AsciiReader::read_line_validated()
    {
        return _validate_ascii(read_line());
    }

    /**
     * Attempts to read a single non-whitespace token/word (potentially skipping
     * preceding whitespace) with ASCII validation/"correction".
     */
    cl7::astring AsciiReader::read_token_validated()
    {
        return _validate_ascii(read_token());
    }

    /**
     * "Peeks" a single character without extracting it. Returns 0 if not possible.
     */
    cl7::achar_t AsciiReader::peek_char()
    {
        std::byte byte;
        if (_readable->peek(byte) != 1)
            return 0;

        return static_cast<cl7::achar_t>(byte);
    }



} // namespace cl7::io

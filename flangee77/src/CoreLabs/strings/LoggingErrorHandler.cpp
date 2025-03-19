#include "LoggingErrorHandler.h"

#include <CoreLabs/logging.h>
#include <CoreLabs/strings.h>



namespace cl7::strings {



    LoggingErrorHandler::LoggingErrorHandler(cl7::logging::LogContext log_context)
        : _log_context(log_context)
    {
    }



    CodepointResult LoggingErrorHandler::_check_adjust_ascii(codepoint codepoint) const
    {
        if (codepoint.value > codepoint::max_ascii)
            _try_log_warning(u8"Invalid ASCII character: " + to_0xhex(codepoint.value) + u8" (" + to_string(codepoint.value) + u8")");

        return ErrorHandler::_check_adjust_ascii(codepoint);
    }

    CodepointResult LoggingErrorHandler::_check_adjust_unicode(codepoint codepoint) const
    {
        if (codepoint.value > codepoint::max_unicode)
            _try_log_warning(u8"Invalid Unicode code point: " + to_0xhex(codepoint.value));

        if (codepoint.is_surrogate())
            _try_log_warning(u8"Invalid Unicode surrogate: " + to_0xhex(codepoint.value));

        return ErrorHandler::_check_adjust_unicode(codepoint);
    }



    CodepointResult LoggingErrorHandler::_on_exhausted_output_space(Encoding encoding, CodepointResult codepoint_result) const
    {
        _try_log_warning(u8"Exhausted output space.");

        return ErrorHandler::_on_exhausted_output_space(encoding, codepoint_result);
    }

    CodepointResult LoggingErrorHandler::_on_insufficient_output_space(Encoding encoding, CodepointResult codepoint_result, size_t required, size_t available) const
    {
        _try_log_warning(u8"Insufficient output space (required: " + to_string(required) + u8", available: " + to_string(available) + u8").");

        return ErrorHandler::_on_insufficient_output_space(encoding, codepoint_result, required, available);
    }



    DecodeResult<cl7::achar_type> LoggingErrorHandler::_on_invalid_code_unit(cl7::astring_view input_read) const
    {
        assert(input_read.size() == 1);
        const auto value = static_cast<uint8_t>(input_read[0]);
        _try_log_warning(u8"Invalid ASCII character: " + to_0xhex(value) + u8" (" + to_string(value) + u8")");

        return ErrorHandler::_on_invalid_code_unit(input_read);
    }

    DecodeResult<cl7::u8char_type> LoggingErrorHandler::_on_invalid_code_unit(cl7::u8string_view input_read) const
    {
        assert(input_read.size() == 1);
        const auto value = static_cast<uint8_t>(input_read[0]);
        if (value >= 0x80 && value <= 0xbf)
            _try_log_warning(u8"Unexpected UTF-8 continuation byte: " + to_0xhex(value));
        else
            _try_log_warning(u8"Invalid UTF-8 code unit: " + to_0xhex(value));

        return ErrorHandler::_on_invalid_code_unit(input_read);
    }

    DecodeResult<cl7::u16char_type> LoggingErrorHandler::_on_invalid_code_unit(cl7::u16string_view input_read) const
    {
        assert(input_read.size() == 1);
        const auto value = static_cast<uint16_t>(input_read[0]);
        _try_log_warning(u8"Invalid UTF-16 code unit: " + to_0xhex(value));

        return ErrorHandler::_on_invalid_code_unit(input_read);
    }

    DecodeResult<cl7::u32char_type> LoggingErrorHandler::_on_invalid_code_unit(cl7::u32string_view input_read) const
    {
        assert(input_read.size() == 1);
        const auto value = static_cast<uint32_t>(input_read[0]);
        _try_log_warning(u8"Invalid UTF-32 code unit: " + to_0xhex(value));

        return ErrorHandler::_on_invalid_code_unit(input_read);
    }



    DecodeResult<cl7::u8char_type> LoggingErrorHandler::_on_invalid_sequence(cl7::u8string_view input_read) const
    {
        assert(!input_read.empty());
        const auto first = static_cast<uint8_t>(input_read[0]);
        cl7::u8string message = u8"Invalid UTF-8 byte sequence:";

        if (first == 0xed && input_read.size() > 1 && input_read[1] >= 0xa0)
            message = u8"Invalid 3-byte UTF-8 sequence representing the surrogate range 0xd800 through 0xdfff:";

        if (first == 0xf4 && input_read.size() > 1 && input_read[1] >= 0x90)
            message = u8"Invalid 4-byte UTF-8 sequence representing a code range above 0x10ffff:";

        if (first >= 0xf5 && first <= 0xf7)
            message = u8"Invalid 4-byte UTF-8 sequence representing a code range starting at 0x140000:";

        if (first >= 0xf8 && first <= 0xfb)
            message = u8"Invalid 5-byte UTF-8 sequence:";

        if (first >= 0xfc && first <= 0xfd)
            message = u8"Invalid 6-byte UTF-8 sequence:";

        for (auto u8c : input_read)
            message += u8" " + to_0xhex(static_cast<uint8_t>(u8c));
        _try_log_warning(message);

        return ErrorHandler::_on_invalid_sequence(input_read);
    }



    DecodeResult<cl7::u8char_type> LoggingErrorHandler::_on_incomplete_sequence(cl7::u8string_view input_read, size_t expected_length) const
    {
        assert(!input_read.empty());
        assert(expected_length > input_read.size());
        cl7::u8string message = u8"Incomplete UTF-8 sequence, " + to_string(expected_length - input_read.size()) + u8" byte(s) missing:";
        for (auto u8c : input_read)
            message += u8" " + to_0xhex(static_cast<uint8_t>(u8c));
        _try_log_warning(message);

        return ErrorHandler::_on_incomplete_sequence(input_read, expected_length);
    }

    DecodeResult<cl7::u16char_type> LoggingErrorHandler::_on_incomplete_sequence(cl7::u16string_view input_read) const
    {
        assert(input_read.size() == 1);
        const auto value = static_cast<uint16_t>(input_read[0]);
        _try_log_warning(u8"Incomplete UTF-16 surrogate pair (unpaired leading/high UTF-16 surrogate): " + to_0xhex(value));

        return ErrorHandler::_on_incomplete_sequence(input_read);
    }



    DecodeResult<cl7::u8char_type> LoggingErrorHandler::_on_overlong_encoding(cl7::u8string_view input_read) const
    {
        assert(!input_read.empty());
        const auto first = static_cast<uint8_t>(input_read[0]);
        cl7::u8string message = u8"Overlong UTF-8 encoding:";

        if (first == 0xc0 || first == 0xc1)
            message = u8"Overlong 2-byte UTF-8 encoding representing the code range from 0 to 0x7f:";

        if (first == 0xe0 && input_read.size() > 1 && input_read[1] < 0xa0)
            message = u8"Overlong 3-byte UTF-8 encoding representing the code range from 0 to 0x7ff:";

        if (first == 0xf0 && input_read.size() > 1 && input_read[1] < 0x90)
            message = u8"Overlong 4-byte UTF-8 encoding representing the code range from 0 to 0xffff:";

        for (auto u8c : input_read)
            message += u8" " + to_0xhex(static_cast<uint8_t>(u8c));
        _try_log_warning(message);

        return ErrorHandler::_on_overlong_encoding(input_read);
    }



    DecodeResult<cl7::u16char_type> LoggingErrorHandler::_on_unpaired_surrogate(cl7::u16string_view input_read) const
    {
        assert(input_read.size() == 1);
        const auto value = static_cast<uint16_t>(input_read[0]);
        _try_log_warning(u8"Unpaired trailing/low UTF-16 surrogate: " + to_0xhex(value));

        return ErrorHandler::_on_unpaired_surrogate(input_read);
    }



    void LoggingErrorHandler::_try_log_warning(cl7::u8string_view message) const
    {
        TRY_LOG_WARNING(_log_context, message);
    }



} // namespace cl7::strings

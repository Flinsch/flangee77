#include "LoggingErrorHandler.h"

#include <CoreLabs/logging.h>
#include <CoreLabs/strings.h>

#include <iostream>



namespace cl7::strings {



    LoggingErrorHandler::LoggingErrorHandler(cl7::logging::LogContext log_context)
        : _log_context(log_context)
    {
    }



    CodepointResult LoggingErrorHandler::_check_adjust_ascii(ErrorStatus& error_status, codepoint codepoint) const
    {
        if (codepoint.value > codepoint::max_ascii)
        {
            if (codepoint.value <= 0xff)
                _try_log_warning(u8"Invalid code point outside ASCII range: " + to_0xhex(codepoint.value) + u8" (" + to_string(codepoint.value) + u8")");
            else
                _try_log_warning(u8"Invalid code point outside ASCII range: " + to_0xhex(codepoint.value));
        }

        return ErrorHandler::_check_adjust_ascii(error_status, codepoint);
    }

    CodepointResult LoggingErrorHandler::_check_adjust_unicode(ErrorStatus& error_status, codepoint codepoint) const
    {
        if (codepoint.value > codepoint::max_unicode)
            _try_log_warning(u8"Invalid Unicode code point (out of range): " + to_0xhex(codepoint.value));

        if (codepoint.is_surrogate())
            _try_log_warning(u8"Invalid Unicode code point (surrogate): " + to_0xhex(codepoint.value));

        return ErrorHandler::_check_adjust_unicode(error_status, codepoint);
    }



    CodepointResult LoggingErrorHandler::_on_exhausted_output_space(ErrorStatus& error_status, Encoding encoding, CodepointResult codepoint_result) const
    {
        _try_log_warning(u8"Exhausted output space.");

        return ErrorHandler::_on_exhausted_output_space(error_status, encoding, codepoint_result);
    }

    CodepointResult LoggingErrorHandler::_on_insufficient_output_space(ErrorStatus& error_status, Encoding encoding, CodepointResult codepoint_result, size_t required, size_t available) const
    {
        _try_log_warning(u8"Insufficient output space (required: " + to_string(required) + u8", available: " + to_string(available) + u8").");

        return ErrorHandler::_on_insufficient_output_space(error_status, encoding, codepoint_result, required, available);
    }



    DecodeResult<cl7::achar_t> LoggingErrorHandler::_on_invalid_code_unit(ErrorStatus& error_status, cl7::astring_view input_read) const
    {
        assert(input_read.size() == 1);
        const auto value = static_cast<uint8_t>(input_read[0]);
        _try_log_warning(u8"Invalid ASCII character: " + to_0xhex(value) + u8" (" + to_string(value) + u8")");

        return ErrorHandler::_on_invalid_code_unit(error_status, input_read);
    }

    DecodeResult<cl7::u8char_t> LoggingErrorHandler::_on_invalid_code_unit(ErrorStatus& error_status, cl7::u8string_view input_read) const
    {
        assert(input_read.size() == 1);
        const auto value = static_cast<uint8_t>(input_read[0]);
        if (value >= 0x80 && value <= 0xbf)
            _try_log_warning(u8"Unexpected UTF-8 continuation byte: " + to_0xhex(value));
        else
            _try_log_warning(u8"Invalid UTF-8 code unit: " + to_0xhex(value));

        return ErrorHandler::_on_invalid_code_unit(error_status, input_read);
    }

    DecodeResult<cl7::u16char_t> LoggingErrorHandler::_on_invalid_code_unit(ErrorStatus& error_status, cl7::u16string_view input_read) const
    {
        assert(input_read.size() == 1);
        const auto value = static_cast<uint16_t>(input_read[0]);
        _try_log_warning(u8"Invalid UTF-16 code unit: " + to_0xhex(value));

        return ErrorHandler::_on_invalid_code_unit(error_status, input_read);
    }

    DecodeResult<cl7::u32char_t> LoggingErrorHandler::_on_invalid_code_unit(ErrorStatus& error_status, cl7::u32string_view input_read) const
    {
        assert(input_read.size() == 1);
        const auto value = static_cast<uint32_t>(input_read[0]);
        _try_log_warning(u8"Invalid UTF-32 code unit: " + to_0xhex(value));

        return ErrorHandler::_on_invalid_code_unit(error_status, input_read);
    }



    DecodeResult<cl7::u8char_t> LoggingErrorHandler::_on_invalid_sequence(ErrorStatus& error_status, cl7::u8string_view input_read) const
    {
        assert(!input_read.empty());
        const auto first = static_cast<uint8_t>(input_read[0]);
        cl7::u8string message = u8"Invalid UTF-8 byte sequence:";

        if (first == 0xed && input_read.size() > 1 && input_read[1] >= 0xa0)
            message = u8"Invalid 3-byte UTF-8 sequence representing the surrogate range 0xD800 through 0xDFFF:";

        if (first == 0xf4 && input_read.size() > 1 && input_read[1] >= 0x90)
            message = u8"Invalid 4-byte UTF-8 sequence representing a code range above 0x10FFFF:";

        if (first >= 0xf5 && first <= 0xf7)
            message = u8"Invalid 4-byte UTF-8 sequence (representing a code range starting at 0x140000):";

        if (first >= 0xf8 && first <= 0xfb)
            message = u8"Invalid 5-byte UTF-8 sequence:";

        if (first >= 0xfc && first <= 0xfd)
            message = u8"Invalid 6-byte UTF-8 sequence:";

        for (auto u8c : input_read)
            message += u8" " + to_0xhex(static_cast<uint8_t>(u8c));
        _try_log_warning(message);

        return ErrorHandler::_on_invalid_sequence(error_status, input_read);
    }



    DecodeResult<cl7::u8char_t> LoggingErrorHandler::_on_incomplete_sequence(ErrorStatus& error_status, cl7::u8string_view input_read, size_t expected_length) const
    {
        assert(!input_read.empty());
        assert(expected_length > input_read.size());
        const auto first = static_cast<uint8_t>(input_read[0]);
        cl7::u8string message = u8"Incomplete UTF-8 sequence, ";

        if (first >= 0xc0 && first <= 0xdf)
            message = u8"Incomplete 2-byte UTF-8 sequence, ";

        if (first >= 0xe0 && first <= 0xef)
            message = u8"Incomplete 3-byte UTF-8 sequence, ";

        if (first >= 0xf0 && first <= 0xf4)
            message = u8"Incomplete 4-byte UTF-8 sequence, ";

        message += to_string(expected_length - input_read.size()) + u8" byte(s) missing:";

        for (auto u8c : input_read)
            message += u8" " + to_0xhex(static_cast<uint8_t>(u8c));
        _try_log_warning(message);

        return ErrorHandler::_on_incomplete_sequence(error_status, input_read, expected_length);
    }

    DecodeResult<cl7::u16char_t> LoggingErrorHandler::_on_incomplete_sequence(ErrorStatus& error_status, cl7::u16string_view input_read) const
    {
        assert(input_read.size() == 1);
        const auto value = static_cast<uint16_t>(input_read[0]);
        _try_log_warning(u8"Incomplete UTF-16 surrogate pair (unpaired leading/high UTF-16 surrogate): " + to_0xhex(value));

        return ErrorHandler::_on_incomplete_sequence(error_status, input_read);
    }



    DecodeResult<cl7::u8char_t> LoggingErrorHandler::_on_overlong_encoding(ErrorStatus& error_status, cl7::u8string_view input_read) const
    {
        assert(!input_read.empty());
        const auto first = static_cast<uint8_t>(input_read[0]);
        cl7::u8string message = u8"Overlong UTF-8 encoding:";

        if (first == 0xc0 || first == 0xc1)
            message = u8"Overlong 2-byte UTF-8 encoding representing the code range from 0 to 0x7F:";

        if (first == 0xe0 && input_read.size() > 1 && input_read[1] < 0xa0)
            message = u8"Overlong 3-byte UTF-8 encoding representing the code range from 0 to 0x7FF:";

        if (first == 0xf0 && input_read.size() > 1 && input_read[1] < 0x90)
            message = u8"Overlong 4-byte UTF-8 encoding representing the code range from 0 to 0xFFFF:";

        for (auto u8c : input_read)
            message += u8" " + to_0xhex(static_cast<uint8_t>(u8c));
        _try_log_warning(message);

        return ErrorHandler::_on_overlong_encoding(error_status, input_read);
    }



    DecodeResult<cl7::u16char_t> LoggingErrorHandler::_on_unpaired_surrogate(ErrorStatus& error_status, cl7::u16string_view input_read) const
    {
        assert(input_read.size() == 1);
        const auto value = static_cast<uint16_t>(input_read[0]);
        _try_log_warning(u8"Unpaired trailing/low UTF-16 surrogate: " + to_0xhex(value));

        return ErrorHandler::_on_unpaired_surrogate(error_status, input_read);
    }



    void LoggingErrorHandler::_try_log_warning(cl7::u8string_view message) const
    {
        TRY_LOG_WARNING(_log_context, message);
        //std::cout << cl7::strings::reinterpret_utf8(message) << "\n";
    }



} // namespace cl7::strings

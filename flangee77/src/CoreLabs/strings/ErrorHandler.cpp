#include "ErrorHandler.h"



namespace cl7::strings {



    CodepointResult ErrorHandler::_check_adjust_ascii(ErrorStatus& error_status, codepoint codepoint) const
    {
        if (codepoint.value > codepoint::max_ascii)
        {
            error_status.merge_error(EncodingError::OutOfRangeAscii);
            return {replacement_ascii};
        }

        return CodepointResult{codepoint};
    }

    CodepointResult ErrorHandler::_check_adjust_unicode(ErrorStatus& error_status, codepoint codepoint) const
    {
        if (codepoint.value > codepoint::max_unicode)
        {
            error_status.merge_error(EncodingError::OutOfRangeUnicode);
            return {replacement_unicode};
        }

        if (codepoint.is_surrogate())
        {
            error_status.merge_error(EncodingError::DisallowedUnicode);
            return {replacement_unicode};
        }

        return CodepointResult{codepoint};
    }



    CodepointResult ErrorHandler::_on_exhausted_output_space(ErrorStatus& error_status, Encoding encoding, CodepointResult codepoint_result) const
    {
        error_status.merge_error(EncodingError::ExhaustedOutputSpace);
        return codepoint_result;
    }

    CodepointResult ErrorHandler::_on_insufficient_output_space(ErrorStatus& error_status, Encoding encoding, CodepointResult codepoint_result, size_t required, size_t available) const
    {
        error_status.merge_error(EncodingError::InsufficientOutputSpace);
        return codepoint_result;
    }



    DecodeResult<cl7::achar_t> ErrorHandler::_on_invalid_code_unit(ErrorStatus& error_status, cl7::astring_view input_read) const
    {
        assert(input_read.size() == 1);
        error_status.merge_error(EncodingError::InvalidCodeUnit);
        return {replacement_ascii, input_read};
    }

    DecodeResult<cl7::u8char_t> ErrorHandler::_on_invalid_code_unit(ErrorStatus& error_status, cl7::u8string_view input_read) const
    {
        assert(input_read.size() == 1);
        error_status.merge_error(EncodingError::InvalidCodeUnit);
        return {replacement_unicode, input_read};
    }

    DecodeResult<cl7::u16char_t> ErrorHandler::_on_invalid_code_unit(ErrorStatus& error_status, cl7::u16string_view input_read) const
    {
        assert(input_read.size() == 1);
        error_status.merge_error(EncodingError::InvalidCodeUnit);
        return {replacement_unicode, input_read};
    }

    DecodeResult<cl7::u32char_t> ErrorHandler::_on_invalid_code_unit(ErrorStatus& error_status, cl7::u32string_view input_read) const
    {
        assert(input_read.size() == 1);
        error_status.merge_error(EncodingError::InvalidCodeUnit);
        return {replacement_unicode, input_read};
    }



    DecodeResult<cl7::u8char_t> ErrorHandler::_on_invalid_sequence(ErrorStatus& error_status, cl7::u8string_view input_read) const
    {
        assert(!input_read.empty());
        error_status.merge_error(EncodingError::InvalidSequence);
        return {replacement_unicode, input_read};
    }



    DecodeResult<cl7::u8char_t> ErrorHandler::_on_incomplete_sequence(ErrorStatus& error_status, cl7::u8string_view input_read, size_t expected_length) const
    {
        assert(!input_read.empty());
        error_status.merge_error(EncodingError::IncompleteSequence);
        return {replacement_unicode, input_read};
    }

    DecodeResult<cl7::u16char_t> ErrorHandler::_on_incomplete_sequence(ErrorStatus& error_status, cl7::u16string_view input_read) const
    {
        assert(input_read.size() == 1);
        error_status.merge_error(EncodingError::IncompleteSequence);
        return {replacement_unicode, input_read};
    }



    DecodeResult<cl7::u8char_t> ErrorHandler::_on_overlong_encoding(ErrorStatus& error_status, cl7::u8string_view input_read) const
    {
        assert(!input_read.empty());
        error_status.merge_error(EncodingError::OverlongEncoding);
        return {replacement_unicode, input_read};
    }



    DecodeResult<cl7::u16char_t> ErrorHandler::_on_unpaired_surrogate(ErrorStatus& error_status, cl7::u16string_view input_read) const
    {
        assert(input_read.size() == 1);
        error_status.merge_error(EncodingError::UnpairedSurrogate);
        return {replacement_unicode, input_read};
    }



} // namespace cl7::strings

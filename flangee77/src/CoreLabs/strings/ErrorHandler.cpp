#include "ErrorHandler.h"



namespace cl7::strings {



    CodepointResult ErrorHandler::_check_adjust_ascii(codepoint codepoint) const
    {
        if (codepoint.value > codepoint::max_ascii)
            return {EncodingError::OutOfRangeAscii, replacement_ascii};

        return CodepointResult{codepoint};
    }

    CodepointResult ErrorHandler::_check_adjust_unicode(codepoint codepoint) const
    {
        if (codepoint.value > codepoint::max_unicode)
            return {EncodingError::OutOfRangeUnicode, replacement_unicode};

        if (codepoint.is_surrogate())
            return {EncodingError::DisallowedUnicode, replacement_unicode};

        return CodepointResult{codepoint};
    }



    CodepointResult ErrorHandler::_on_exhausted_output_space(Encoding encoding, CodepointResult codepoint_result) const
    {
        codepoint_result.merge_error(EncodingError::ExhaustedOutputSpace);
        return codepoint_result;
    }

    CodepointResult ErrorHandler::_on_insufficient_output_space(Encoding encoding, CodepointResult codepoint_result, size_t required, size_t available) const
    {
        codepoint_result.merge_error(EncodingError::InsufficientOutputSpace);
        return codepoint_result;
    }



    DecodeResult<cl7::achar_t> ErrorHandler::_on_invalid_code_unit(cl7::astring_view input_read) const
    {
        assert(input_read.size() == 1);
        return {EncodingError::InvalidCodeUnit, replacement_ascii, input_read};
    }

    DecodeResult<cl7::u8char_t> ErrorHandler::_on_invalid_code_unit(cl7::u8string_view input_read) const
    {
        assert(input_read.size() == 1);
        return {EncodingError::InvalidCodeUnit, replacement_unicode, input_read};
    }

    DecodeResult<cl7::u16char_t> ErrorHandler::_on_invalid_code_unit(cl7::u16string_view input_read) const
    {
        assert(input_read.size() == 1);
        return {EncodingError::InvalidCodeUnit, replacement_unicode, input_read};
    }

    DecodeResult<cl7::u32char_t> ErrorHandler::_on_invalid_code_unit(cl7::u32string_view input_read) const
    {
        assert(input_read.size() == 1);
        return {EncodingError::InvalidCodeUnit, replacement_unicode, input_read};
    }



    DecodeResult<cl7::u8char_t> ErrorHandler::_on_invalid_sequence(cl7::u8string_view input_read) const
    {
        assert(!input_read.empty());
        return {EncodingError::InvalidSequence, replacement_unicode, input_read};
    }



    DecodeResult<cl7::u8char_t> ErrorHandler::_on_incomplete_sequence(cl7::u8string_view input_read, size_t expected_length) const
    {
        assert(!input_read.empty());
        return {EncodingError::IncompleteSequence, replacement_unicode, input_read};
    }

    DecodeResult<cl7::u16char_t> ErrorHandler::_on_incomplete_sequence(cl7::u16string_view input_read) const
    {
        assert(input_read.size() == 1);
        return {EncodingError::IncompleteSequence, replacement_unicode, input_read};
    }



    DecodeResult<cl7::u8char_t> ErrorHandler::_on_overlong_encoding(cl7::u8string_view input_read) const
    {
        assert(!input_read.empty());
        return {EncodingError::OverlongEncoding, replacement_unicode, input_read};
    }



    DecodeResult<cl7::u16char_t> ErrorHandler::_on_unpaired_surrogate(cl7::u16string_view input_read) const
    {
        assert(input_read.size() == 1);
        return {EncodingError::UnpairedSurrogate, replacement_unicode, input_read};
    }



} // namespace cl7::strings

#include "ErrorHandler.h"



namespace cl7::strings::codec {



    /**
     * Checks whether the given code point represents a valid ASCII character with a
     * value of up to 0x7F and, if not, replaces it with the ASCII replacement
     * symbol. Adds an error if applicable.
     */
    CodepointResult ErrorHandler::check_adjust_ascii(ErrorStatus& error_status, codepoint codepoint) const
    {
        if (codepoint.value > codepoint::max_ascii)
        {
            _on_encode_error_helper(EncodingError::OutOfRangeAscii);
            _on_out_of_range_ascii(codepoint);
            return _handle_out_of_range_ascii(error_status, codepoint);
        }

        return CodepointResult{codepoint};
    }

    /**
     * Checks whether the given code point is valid Unicode with a maximum value of
     * U+10FFFF and not within the surrogate range U+D800 through U+DFFF, and if
     * not, replaces it with the Unicode replacement symbol. Adds an error if
     * applicable.
     */
    CodepointResult ErrorHandler::check_adjust_unicode(ErrorStatus& error_status, codepoint codepoint) const
    {
        if (codepoint.value > codepoint::max_unicode)
        {
            _on_encode_error_helper(EncodingError::OutOfRangeUnicode);
            _on_out_of_range_unicode(codepoint);
            return _handle_out_of_range_unicode(error_status, codepoint);
        }

        if (codepoint.is_surrogate())
        {
            _on_encode_error_helper(EncodingError::DisallowedUnicode);
            _on_disallowed_unicode(codepoint);
            return _handle_disallowed_unicode(error_status, codepoint);
        }

        return CodepointResult{codepoint};
    }

    /**
     * Returns a code point result on an "exhausted output space" error for the
     * specified encoding.
     */
    CodepointResult ErrorHandler::process_exhausted_output_space(ErrorStatus& error_status, Encoding encoding, const CodepointResult& codepoint_result) const
    {
        _on_encode_error_helper(EncodingError::ExhaustedOutputSpace, encoding);
        _on_exhausted_output_space(encoding, codepoint_result);
        return _handle_exhausted_output_space(error_status, encoding, codepoint_result);
    }

    /**
     * Returns a code point result on an "insufficient output space" error for the
     * specified encoding.
     */
    CodepointResult ErrorHandler::process_insufficient_output_space(ErrorStatus& error_status, Encoding encoding, const CodepointResult& codepoint_result, size_t required, size_t available) const
    {
        _on_encode_error_helper(EncodingError::InsufficientOutputSpace, encoding);
        _on_insufficient_output_space(encoding, codepoint_result, required, available);
        return _handle_insufficient_output_space(error_status, encoding, codepoint_result, required, available);
    }

    /**
     * Returns a decode result on an "invalid code unit" error based on the
     * specified ASCII input being processed.
     */
    DecodeResult<cl7::achar_t> ErrorHandler::process_invalid_code_unit(ErrorStatus& error_status, cl7::astring_view input_read) const
    {
        _on_decode_error_helper(EncodingError::InvalidCodeUnit, input_read);
        _on_invalid_code_unit(input_read);
        return _handle_invalid_code_unit(error_status, input_read);
    }

    /**
     * Returns a decode result on an "invalid code unit" error based on the
     * specified UTF-8 input being processed.
     */
    DecodeResult<cl7::u8char_t> ErrorHandler::process_invalid_code_unit(ErrorStatus& error_status, cl7::u8string_view input_read) const
    {
        _on_decode_error_helper(EncodingError::InvalidCodeUnit, input_read);
        _on_invalid_code_unit(input_read);
        return _handle_invalid_code_unit(error_status, input_read);
    }

    /**
     * Returns a decode result on an "invalid code unit" error based on the
     * specified UTF-16 input being processed.
     */
    DecodeResult<cl7::u16char_t> ErrorHandler::process_invalid_code_unit(ErrorStatus& error_status, cl7::u16string_view input_read) const
    {
        _on_decode_error_helper(EncodingError::InvalidCodeUnit, input_read);
        _on_invalid_code_unit(input_read);
        return _handle_invalid_code_unit(error_status, input_read);
    }

    /**
     * Returns a decode result on an "invalid code unit" error based on the
     * specified UTF-32 input being processed.
     */
    DecodeResult<cl7::u32char_t> ErrorHandler::process_invalid_code_unit(ErrorStatus& error_status, cl7::u32string_view input_read) const
    {
        _on_decode_error_helper(EncodingError::InvalidCodeUnit, input_read);
        _on_invalid_code_unit(input_read);
        return _handle_invalid_code_unit(error_status, input_read);
    }

    /**
     * Returns a decode result on an "invalid sequence" error based on the specified
     * UTF-8 input being processed.
     */
    DecodeResult<cl7::u8char_t> ErrorHandler::process_invalid_sequence(ErrorStatus& error_status, cl7::u8string_view input_read) const
    {
        _on_decode_error_helper(EncodingError::InvalidSequence, input_read);
        _on_invalid_sequence(input_read);
        return _handle_invalid_sequence(error_status, input_read);
    }

    /**
     * Returns a decode result on an "incomplete sequence" error based on the
     * specified UTF-8 input being processed.
     */
    DecodeResult<cl7::u8char_t> ErrorHandler::process_incomplete_sequence(ErrorStatus& error_status, cl7::u8string_view input_read, size_t expected_length) const
    {
        _on_decode_error_helper(EncodingError::IncompleteSequence, input_read);
        _on_incomplete_sequence(input_read, expected_length);
        return _handle_incomplete_sequence(error_status, input_read, expected_length);
    }

    /**
     * Returns a decode result on an "incomplete sequence" error based on the
     * specified UTF-16 input being processed.
     */
    DecodeResult<cl7::u16char_t> ErrorHandler::process_incomplete_sequence(ErrorStatus& error_status, cl7::u16string_view input_read) const
    {
        _on_decode_error_helper(EncodingError::IncompleteSequence, input_read);
        _on_incomplete_sequence(input_read);
        return _handle_incomplete_sequence(error_status, input_read);
    }

    /**
     * Returns a decode result on an "overlong encoding" error based on the
     * specified UTF-8 input being processed.
     */
    DecodeResult<cl7::u8char_t> ErrorHandler::process_overlong_encoding(ErrorStatus& error_status, cl7::u8string_view input_read) const
    {
        _on_decode_error_helper(EncodingError::OverlongEncoding, input_read);
        _on_overlong_encoding(input_read);
        return _handle_overlong_encoding(error_status, input_read);
    }

    /**
     * Returns a decode result on an "unpaired surrogate" error based on the
     * specified UTF-16 input being processed.
     */
    DecodeResult<cl7::u16char_t> ErrorHandler::process_unpaired_surrogate(ErrorStatus& error_status, cl7::u16string_view input_read) const
    {
        _on_decode_error_helper(EncodingError::UnpairedSurrogate, input_read);
        _on_unpaired_surrogate(input_read);
        return _handle_unpaired_surrogate(error_status, input_read);
    }



    CodepointResult ErrorHandler::_handle_out_of_range_ascii(ErrorStatus& error_status, codepoint codepoint) const
    {
        error_status.merge_error(EncodingError::OutOfRangeAscii);
        return {replacement_ascii};
    }

    CodepointResult ErrorHandler::_handle_out_of_range_unicode(ErrorStatus& error_status, codepoint codepoint) const
    {
        error_status.merge_error(EncodingError::OutOfRangeUnicode);
        return {replacement_unicode};
    }

    CodepointResult ErrorHandler::_handle_disallowed_unicode(ErrorStatus& error_status, codepoint codepoint) const
    {
        error_status.merge_error(EncodingError::DisallowedUnicode);
        return {replacement_unicode};
    }



    CodepointResult ErrorHandler::_handle_exhausted_output_space(ErrorStatus& error_status, Encoding encoding, CodepointResult codepoint_result) const
    {
        error_status.merge_error(EncodingError::ExhaustedOutputSpace);
        return codepoint_result;
    }

    CodepointResult ErrorHandler::_handle_insufficient_output_space(ErrorStatus& error_status, Encoding encoding, CodepointResult codepoint_result, size_t required, size_t available) const
    {
        error_status.merge_error(EncodingError::InsufficientOutputSpace);
        return codepoint_result;
    }



    DecodeResult<cl7::achar_t> ErrorHandler::_handle_invalid_code_unit(ErrorStatus& error_status, cl7::astring_view input_read) const
    {
        assert(input_read.size() == 1);
        error_status.merge_error(EncodingError::InvalidCodeUnit);
        return {replacement_ascii, input_read};
    }

    DecodeResult<cl7::u8char_t> ErrorHandler::_handle_invalid_code_unit(ErrorStatus& error_status, cl7::u8string_view input_read) const
    {
        assert(input_read.size() == 1);
        error_status.merge_error(EncodingError::InvalidCodeUnit);
        return {replacement_unicode, input_read};
    }

    DecodeResult<cl7::u16char_t> ErrorHandler::_handle_invalid_code_unit(ErrorStatus& error_status, cl7::u16string_view input_read) const
    {
        assert(input_read.size() == 1);
        error_status.merge_error(EncodingError::InvalidCodeUnit);
        return {replacement_unicode, input_read};
    }

    DecodeResult<cl7::u32char_t> ErrorHandler::_handle_invalid_code_unit(ErrorStatus& error_status, cl7::u32string_view input_read) const
    {
        assert(input_read.size() == 1);
        error_status.merge_error(EncodingError::InvalidCodeUnit);
        return {replacement_unicode, input_read};
    }



    DecodeResult<cl7::u8char_t> ErrorHandler::_handle_invalid_sequence(ErrorStatus& error_status, cl7::u8string_view input_read) const
    {
        assert(!input_read.empty());
        error_status.merge_error(EncodingError::InvalidSequence);
        return {replacement_unicode, input_read};
    }



    DecodeResult<cl7::u8char_t> ErrorHandler::_handle_incomplete_sequence(ErrorStatus& error_status, cl7::u8string_view input_read, size_t expected_length) const
    {
        assert(!input_read.empty());
        error_status.merge_error(EncodingError::IncompleteSequence);
        return {replacement_unicode, input_read};
    }

    DecodeResult<cl7::u16char_t> ErrorHandler::_handle_incomplete_sequence(ErrorStatus& error_status, cl7::u16string_view input_read) const
    {
        assert(input_read.size() == 1);
        error_status.merge_error(EncodingError::IncompleteSequence);
        return {replacement_unicode, input_read};
    }



    DecodeResult<cl7::u8char_t> ErrorHandler::_handle_overlong_encoding(ErrorStatus& error_status, cl7::u8string_view input_read) const
    {
        assert(!input_read.empty());
        error_status.merge_error(EncodingError::OverlongEncoding);
        return {replacement_unicode, input_read};
    }



    DecodeResult<cl7::u16char_t> ErrorHandler::_handle_unpaired_surrogate(ErrorStatus& error_status, cl7::u16string_view input_read) const
    {
        assert(input_read.size() == 1);
        error_status.merge_error(EncodingError::UnpairedSurrogate);
        return {replacement_unicode, input_read};
    }



    void ErrorHandler::_on_encode_error_helper(EncodingError error) const
    {
        _on_any_error(error);
        _on_encode_error(error);
    }

    void ErrorHandler::_on_encode_error_helper(EncodingError error, Encoding encoding) const
    {
        _on_any_error(error);
        _on_encode_error(error);
        _on_encode_error(error, encoding);
    }

    void ErrorHandler::_on_decode_error_helper(EncodingError error, cl7::astring_view input_read) const
    {
        _on_any_error(error);
        _on_decode_error(error);
        _on_decode_error(error, input_read);
    }

    void ErrorHandler::_on_decode_error_helper(EncodingError error, cl7::u8string_view input_read) const
    {
        _on_any_error(error);
        _on_decode_error(error);
        _on_decode_error(error, input_read);
    }

    void ErrorHandler::_on_decode_error_helper(EncodingError error, cl7::u16string_view input_read) const
    {
        _on_any_error(error);
        _on_decode_error(error);
        _on_decode_error(error, input_read);
    }

    void ErrorHandler::_on_decode_error_helper(EncodingError error, cl7::u32string_view input_read) const
    {
        _on_any_error(error);
        _on_decode_error(error);
        _on_decode_error(error, input_read);
    }



} // namespace cl7::strings::codec

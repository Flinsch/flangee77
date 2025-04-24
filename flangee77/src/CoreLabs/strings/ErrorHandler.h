#ifndef CL7_STRINGS_ERRORHANDLER_H
#define CL7_STRINGS_ERRORHANDLER_H

#include "./codepoint.h"
#include "./Encoding.h"
#include "./EncodeResult.h"
#include "./DecodeResult.h"
#include "./ErrorStatus.h"

#include <CoreLabs/string.h>



namespace cl7::strings {



class ErrorHandler
{
public:
    virtual ~ErrorHandler() = default;



    codepoint replacement_ascii = {codepoint::replacement_ascii};
    codepoint replacement_unicode = {codepoint::replacement_unicode};



    /**
     * Checks whether the given code point represents a valid ASCII character with a
     * value of up to 0x7F and, if not, replaces it with the ASCII replacement
     * symbol. Adds an error if applicable.
     */
    CodepointResult check_adjust_ascii(ErrorStatus& error_status, codepoint codepoint) const;

    /**
     * Checks whether the given code point is valid Unicode with a maximum value of
     * U+10FFFF and not within the surrogate range U+D800 through U+DFFF, and if
     * not, replaces it with the Unicode replacement symbol. Adds an error if
     * applicable.
     */
    CodepointResult check_adjust_unicode(ErrorStatus& error_status, codepoint codepoint) const;

    /**
     * Returns a code point result on an "exhausted output space" error for the
     * specified encoding.
     */
    CodepointResult process_exhausted_output_space(ErrorStatus& error_status, Encoding encoding, const CodepointResult& codepoint_result) const;

    /**
     * Returns a code point result on an "insufficient output space" error for the
     * specified encoding.
     */
    CodepointResult process_insufficient_output_space(ErrorStatus& error_status, Encoding encoding, const CodepointResult& codepoint_result, size_t required, size_t available) const;

    /**
     * Returns a decode result on an "invalid code unit" error based on the
     * specified ASCII input being processed.
     */
    DecodeResult<cl7::achar_t> process_invalid_code_unit(ErrorStatus& error_status, cl7::astring_view input_read) const;

    /**
     * Returns a decode result on an "invalid code unit" error based on the
     * specified UTF-8 input being processed.
     */
    DecodeResult<cl7::u8char_t> process_invalid_code_unit(ErrorStatus& error_status, cl7::u8string_view input_read) const;

    /**
     * Returns a decode result on an "invalid code unit" error based on the
     * specified UTF-16 input being processed.
     */
    DecodeResult<cl7::u16char_t> process_invalid_code_unit(ErrorStatus& error_status, cl7::u16string_view input_read) const;

    /**
     * Returns a decode result on an "invalid code unit" error based on the
     * specified UTF-32 input being processed.
     */
    DecodeResult<cl7::u32char_t> process_invalid_code_unit(ErrorStatus& error_status, cl7::u32string_view input_read) const;

    /**
     * Returns a decode result on an "invalid sequence" error based on the specified
     * UTF-8 input being processed.
     */
    DecodeResult<cl7::u8char_t> process_invalid_sequence(ErrorStatus& error_status, cl7::u8string_view input_read) const;

    /**
     * Returns a decode result on an "incomplete sequence" error based on the
     * specified UTF-8 input being processed.
     */
    DecodeResult<cl7::u8char_t> process_incomplete_sequence(ErrorStatus& error_status, cl7::u8string_view input_read, size_t expected_length) const;

    /**
     * Returns a decode result on an "incomplete sequence" error based on the
     * specified UTF-16 input being processed.
     */
    DecodeResult<cl7::u16char_t> process_incomplete_sequence(ErrorStatus& error_status, cl7::u16string_view input_read) const;

    /**
     * Returns a decode result on an "overlong encoding" error based on the
     * specified UTF-8 input being processed.
     */
    DecodeResult<cl7::u8char_t> process_overlong_encoding(ErrorStatus& error_status, cl7::u8string_view input_read) const;

    /**
     * Returns a decode result on an "unpaired surrogate" error based on the
     * specified UTF-16 input being processed.
     */
    DecodeResult<cl7::u16char_t> process_unpaired_surrogate(ErrorStatus& error_status, cl7::u16string_view input_read) const;



protected:

    virtual void _on_any_error(EncodingError error) const {}
    virtual void _on_encode_error(EncodingError error) const {}
    virtual void _on_encode_error(EncodingError error, Encoding encoding) const {}
    virtual void _on_decode_error(EncodingError error) const {}
    virtual void _on_decode_error(EncodingError error, cl7::astring_view input_read) const {}
    virtual void _on_decode_error(EncodingError error, cl7::u8string_view input_read) const {}
    virtual void _on_decode_error(EncodingError error, cl7::u16string_view input_read) const {}
    virtual void _on_decode_error(EncodingError error, cl7::u32string_view input_read) const {}


    virtual void _on_out_of_range_ascii(codepoint codepoint) const {}
    virtual void _on_out_of_range_unicode(codepoint codepoint) const {}
    virtual void _on_disallowed_unicode(codepoint codepoint) const {}

    virtual void _on_exhausted_output_space(Encoding encoding, CodepointResult codepoint_result) const {}
    virtual void _on_insufficient_output_space(Encoding encoding, CodepointResult codepoint_result, size_t required, size_t available) const {}

    virtual void _on_invalid_code_unit(cl7::astring_view input_read) const {}
    virtual void _on_invalid_code_unit(cl7::u8string_view input_read) const {}
    virtual void _on_invalid_code_unit(cl7::u16string_view input_read) const {}
    virtual void _on_invalid_code_unit(cl7::u32string_view input_read) const {}

    virtual void _on_invalid_sequence(cl7::u8string_view input_read) const {}

    virtual void _on_incomplete_sequence(cl7::u8string_view input_read, size_t expected_length) const {}
    virtual void _on_incomplete_sequence(cl7::u16string_view input_read) const {}

    virtual void _on_overlong_encoding(cl7::u8string_view input_read) const {}

    virtual void _on_unpaired_surrogate(cl7::u16string_view input_read) const {}


    virtual CodepointResult _handle_out_of_range_ascii(ErrorStatus& error_status, codepoint codepoint) const;
    virtual CodepointResult _handle_out_of_range_unicode(ErrorStatus& error_status, codepoint codepoint) const;
    virtual CodepointResult _handle_disallowed_unicode(ErrorStatus& error_status, codepoint codepoint) const;

    virtual CodepointResult _handle_exhausted_output_space(ErrorStatus& error_status, Encoding encoding, CodepointResult codepoint_result) const;
    virtual CodepointResult _handle_insufficient_output_space(ErrorStatus& error_status, Encoding encoding, CodepointResult codepoint_result, size_t required, size_t available) const;

    virtual DecodeResult<cl7::achar_t> _handle_invalid_code_unit(ErrorStatus& error_status, cl7::astring_view input_read) const;
    virtual DecodeResult<cl7::u8char_t> _handle_invalid_code_unit(ErrorStatus& error_status, cl7::u8string_view input_read) const;
    virtual DecodeResult<cl7::u16char_t> _handle_invalid_code_unit(ErrorStatus& error_status, cl7::u16string_view input_read) const;
    virtual DecodeResult<cl7::u32char_t> _handle_invalid_code_unit(ErrorStatus& error_status, cl7::u32string_view input_read) const;

    virtual DecodeResult<cl7::u8char_t> _handle_invalid_sequence(ErrorStatus& error_status, cl7::u8string_view input_read) const;

    virtual DecodeResult<cl7::u8char_t> _handle_incomplete_sequence(ErrorStatus& error_status, cl7::u8string_view input_read, size_t expected_length) const;
    virtual DecodeResult<cl7::u16char_t> _handle_incomplete_sequence(ErrorStatus& error_status, cl7::u16string_view input_read) const;

    virtual DecodeResult<cl7::u8char_t> _handle_overlong_encoding(ErrorStatus& error_status, cl7::u8string_view input_read) const;

    virtual DecodeResult<cl7::u16char_t> _handle_unpaired_surrogate(ErrorStatus& error_status, cl7::u16string_view input_read) const;



private:

    void _on_encode_error_helper(EncodingError error) const;
    void _on_encode_error_helper(EncodingError error, Encoding encoding) const;
    void _on_decode_error_helper(EncodingError error, cl7::astring_view input_read) const;
    void _on_decode_error_helper(EncodingError error, cl7::u8string_view input_read) const;
    void _on_decode_error_helper(EncodingError error, cl7::u16string_view input_read) const;
    void _on_decode_error_helper(EncodingError error, cl7::u32string_view input_read) const;

}; // class ErrorHandler



} // namespace cl7::strings

#endif // CL7_STRINGS_ERRORHANDLER_H

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
    CodepointResult check_adjust_ascii(ErrorStatus& error_status, codepoint codepoint) const { return _check_adjust_ascii(error_status, codepoint); }

    /**
     * Checks whether the given code point is valid Unicode with a maximum value of
     * U+10FFFF and not within the surrogate range U+D800 through U+DFFF, and if
     * not, replaces it with the Unicode replacement symbol. Adds an error if
     * applicable.
     */
    CodepointResult check_adjust_unicode(ErrorStatus& error_status, codepoint codepoint) const { return _check_adjust_unicode(error_status, codepoint); }

    /**
     * Returns a code point result on an "exhausted output space" error for the
     * specified encoding.
     */
    CodepointResult on_exhausted_output_space(ErrorStatus& error_status, Encoding encoding, const CodepointResult& codepoint_result) const { return _on_exhausted_output_space(error_status, encoding, codepoint_result); }

    /**
     * Returns a code point result on an "insufficient output space" error for the
     * specified encoding.
     */
    CodepointResult on_insufficient_output_space(ErrorStatus& error_status, Encoding encoding, const CodepointResult& codepoint_result, size_t required, size_t available) const { return _on_insufficient_output_space(error_status, encoding, codepoint_result, required, available); }

    /**
     * Returns a decode result on an "invalid code unit" error based on the
     * specified ASCII input being processed.
     */
    DecodeResult<cl7::achar_t> on_invalid_code_unit(ErrorStatus& error_status, cl7::astring_view input_read) const { return _on_invalid_code_unit(error_status, input_read); }

    /**
     * Returns a decode result on an "invalid code unit" error based on the
     * specified UTF-8 input being processed.
     */
    DecodeResult<cl7::u8char_t> on_invalid_code_unit(ErrorStatus& error_status, cl7::u8string_view input_read) const { return _on_invalid_code_unit(error_status, input_read); }

    /**
     * Returns a decode result on an "invalid code unit" error based on the
     * specified UTF-16 input being processed.
     */
    DecodeResult<cl7::u16char_t> on_invalid_code_unit(ErrorStatus& error_status, cl7::u16string_view input_read) const { return _on_invalid_code_unit(error_status, input_read); }

    /**
     * Returns a decode result on an "invalid code unit" error based on the
     * specified UTF-32 input being processed.
     */
    DecodeResult<cl7::u32char_t> on_invalid_code_unit(ErrorStatus& error_status, cl7::u32string_view input_read) const { return _on_invalid_code_unit(error_status, input_read); }

    /**
     * Returns a decode result on an "invalid sequence" error based on the specified
     * UTF-8 input being processed.
     */
    DecodeResult<cl7::u8char_t> on_invalid_sequence(ErrorStatus& error_status, cl7::u8string_view input_read) const { return _on_invalid_sequence(error_status, input_read); }

    /**
     * Returns a decode result on an "incomplete sequence" error based on the
     * specified UTF-8 input being processed.
     */
    DecodeResult<cl7::u8char_t> on_incomplete_sequence(ErrorStatus& error_status, cl7::u8string_view input_read, size_t expected_length) const { return _on_incomplete_sequence(error_status, input_read, expected_length); }

    /**
     * Returns a decode result on an "incomplete sequence" error based on the
     * specified UTF-16 input being processed.
     */
    DecodeResult<cl7::u16char_t> on_incomplete_sequence(ErrorStatus& error_status, cl7::u16string_view input_read) const { return _on_incomplete_sequence(error_status, input_read); }

    /**
     * Returns a decode result on an "overlong encoding" error based on the
     * specified UTF-8 input being processed.
     */
    DecodeResult<cl7::u8char_t> on_overlong_encoding(ErrorStatus& error_status, cl7::u8string_view input_read) const { return _on_overlong_encoding(error_status, input_read); }

    /**
     * Returns a decode result on an "unpaired surrogate" error based on the
     * specified UTF-16 input being processed.
     */
    DecodeResult<cl7::u16char_t> on_unpaired_surrogate(ErrorStatus& error_status, cl7::u16string_view input_read) const { return _on_unpaired_surrogate(error_status, input_read); }



protected:

    virtual CodepointResult _check_adjust_ascii(ErrorStatus& error_status, codepoint codepoint) const;
    virtual CodepointResult _check_adjust_unicode(ErrorStatus& error_status, codepoint codepoint) const;

    virtual CodepointResult _on_exhausted_output_space(ErrorStatus& error_status, Encoding encoding, CodepointResult codepoint_result) const;
    virtual CodepointResult _on_insufficient_output_space(ErrorStatus& error_status, Encoding encoding, CodepointResult codepoint_result, size_t required, size_t available) const;

    virtual DecodeResult<cl7::achar_t> _on_invalid_code_unit(ErrorStatus& error_status, cl7::astring_view input_read) const;
    virtual DecodeResult<cl7::u8char_t> _on_invalid_code_unit(ErrorStatus& error_status, cl7::u8string_view input_read) const;
    virtual DecodeResult<cl7::u16char_t> _on_invalid_code_unit(ErrorStatus& error_status, cl7::u16string_view input_read) const;
    virtual DecodeResult<cl7::u32char_t> _on_invalid_code_unit(ErrorStatus& error_status, cl7::u32string_view input_read) const;

    virtual DecodeResult<cl7::u8char_t> _on_invalid_sequence(ErrorStatus& error_status, cl7::u8string_view input_read) const;

    virtual DecodeResult<cl7::u8char_t> _on_incomplete_sequence(ErrorStatus& error_status, cl7::u8string_view input_read, size_t expected_length) const;
    virtual DecodeResult<cl7::u16char_t> _on_incomplete_sequence(ErrorStatus& error_status, cl7::u16string_view input_read) const;

    virtual DecodeResult<cl7::u8char_t> _on_overlong_encoding(ErrorStatus& error_status, cl7::u8string_view input_read) const;

    virtual DecodeResult<cl7::u16char_t> _on_unpaired_surrogate(ErrorStatus& error_status, cl7::u16string_view input_read) const;

}; // class ErrorHandler



} // namespace cl7::strings

#endif // CL7_STRINGS_ERRORHANDLER_H

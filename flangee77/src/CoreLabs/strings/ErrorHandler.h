#ifndef CL7_STRINGS_ERRORHANDLER_H
#define CL7_STRINGS_ERRORHANDLER_H

#include "./codepoint.h"
#include "./Encoding.h"
#include "./EncodeResult.h"
#include "./DecodeResult.h"

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
     * symbol. Returns an error code if applicable or none/zero if valid.
     */
    CodepointResult check_adjust_ascii(codepoint codepoint) const { return _check_adjust_ascii(codepoint); }

    /**
     * Checks whether the given code point is valid Unicode with a maximum value of
     * U+10FFFF and not within the surrogate range U+D800 through U+DFFF, and if
     * not, replaces it with the Unicode replacement symbol. Returns an error code
     * if applicable or none/zero if valid.
     */
    CodepointResult check_adjust_unicode(codepoint codepoint) const { return _check_adjust_unicode(codepoint); }

    /**
     * Returns a code point result that represents an "exhausted output space" error
     * merged with the given result for the specified encoding.
     */
    CodepointResult on_exhausted_output_space(Encoding encoding, const CodepointResult& codepoint_result) const { return _on_exhausted_output_space(encoding, codepoint_result); }

    /**
     * Returns a code point result that represents an "insufficient output space"
     * error merged with the given result for the specified encoding.
     */
    CodepointResult on_insufficient_output_space(Encoding encoding, const CodepointResult& codepoint_result, size_t required, size_t available) const { return _on_insufficient_output_space(encoding, codepoint_result, required, available); }

    /**
     * Returns a decode result that represents an "invalid code unit" error based on
     * the specified ASCII input being processed.
     */
    DecodeResult<cl7::achar_type> on_invalid_code_unit(cl7::astring_view input_read) const { return _on_invalid_code_unit(input_read); }

    /**
     * Returns a decode result that represents an "invalid code unit" error based on
     * the specified UTF-8 input being processed.
     */
    DecodeResult<cl7::u8char_type> on_invalid_code_unit(cl7::u8string_view input_read) const { return _on_invalid_code_unit(input_read); }

    /**
     * Returns a decode result that represents an "invalid code unit" error based on
     * the specified UTF-16 input being processed.
     */
    DecodeResult<cl7::u16char_type> on_invalid_code_unit(cl7::u16string_view input_read) const { return _on_invalid_code_unit(input_read); }

    /**
     * Returns a decode result that represents an "invalid code unit" error based on
     * the specified UTF-32 input being processed.
     */
    DecodeResult<cl7::u32char_type> on_invalid_code_unit(cl7::u32string_view input_read) const { return _on_invalid_code_unit(input_read); }

    /**
     * Returns a decode result that represents an "invalid sequence" error based on
     * the specified UTF-8 input being processed.
     */
    DecodeResult<cl7::u8char_type> on_invalid_sequence(cl7::u8string_view input_read) const { return _on_invalid_sequence(input_read); }

    /**
     * Returns a decode result that represents an "incomplete sequence" error based
     * on the specified UTF-8 input being processed.
     */
    DecodeResult<cl7::u8char_type> on_incomplete_sequence(cl7::u8string_view input_read, size_t expected_length) const { return _on_incomplete_sequence(input_read, expected_length); }

    /**
     * Returns a decode result that represents an "incomplete sequence" error based
     * on the specified UTF-16 input being processed.
     */
    DecodeResult<cl7::u16char_type> on_incomplete_sequence(cl7::u16string_view input_read) const { return _on_incomplete_sequence(input_read); }

    /**
     * Returns a decode result that represents an "overlong encoding" error based on
     * the specified UTF-8 input being processed.
     */
    DecodeResult<cl7::u8char_type> on_overlong_encoding(cl7::u8string_view input_read) const { return _on_overlong_encoding(input_read); }

    /**
     * Returns a decode result that represents an "unpaired surrogate" error based
     * on the specified UTF-16 input being processed.
     */
    DecodeResult<cl7::u16char_type> on_unpaired_surrogate(cl7::u16string_view input_read) const { return _on_unpaired_surrogate(input_read); }



protected:

    virtual CodepointResult _check_adjust_ascii(codepoint codepoint) const;
    virtual CodepointResult _check_adjust_unicode(codepoint codepoint) const;

    virtual CodepointResult _on_exhausted_output_space(Encoding encoding, CodepointResult codepoint_result) const;
    virtual CodepointResult _on_insufficient_output_space(Encoding encoding, CodepointResult codepoint_result, size_t required, size_t available) const;

    virtual DecodeResult<cl7::achar_type> _on_invalid_code_unit(cl7::astring_view input_read) const;
    virtual DecodeResult<cl7::u8char_type> _on_invalid_code_unit(cl7::u8string_view input_read) const;
    virtual DecodeResult<cl7::u16char_type> _on_invalid_code_unit(cl7::u16string_view input_read) const;
    virtual DecodeResult<cl7::u32char_type> _on_invalid_code_unit(cl7::u32string_view input_read) const;

    virtual DecodeResult<cl7::u8char_type> _on_invalid_sequence(cl7::u8string_view input_read) const;

    virtual DecodeResult<cl7::u8char_type> _on_incomplete_sequence(cl7::u8string_view input_read, size_t expected_length) const;
    virtual DecodeResult<cl7::u16char_type> _on_incomplete_sequence(cl7::u16string_view input_read) const;

    virtual DecodeResult<cl7::u8char_type> _on_overlong_encoding(cl7::u8string_view input_read) const;

    virtual DecodeResult<cl7::u16char_type> _on_unpaired_surrogate(cl7::u16string_view input_read) const;

}; // class ErrorHandler



} // namespace cl7::strings

#endif // CL7_STRINGS_ERRORHANDLER_H

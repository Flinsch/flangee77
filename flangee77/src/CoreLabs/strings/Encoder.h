#ifndef CL7_STRINGS_ENCODER_H
#define CL7_STRINGS_ENCODER_H

#include "./EncodeResult.h"
#include "./DefaultErrorHandler.h"

#include "./traits.h"



namespace cl7::strings {



template <typename Tchar, class TDefaultErrorHandler = DefaultErrorHandler>
    requires(std::is_default_constructible_v<TDefaultErrorHandler>)
class Encoder
{

public:
    using Codec = typename traits<Tchar>::codec_type;

    using string_span_type = typename traits<Tchar>::string_span_type;
    using string_type = typename traits<Tchar>::string_type;



    Encoder(const ErrorHandler* error_handler = nullptr)
        : _default_error_handler()
        , _error_handler(error_handler ? error_handler : &_default_error_handler)
    {
    }



    /**
     * Returns the number of code units that a given code point would have in the
     * target encoding.
     */
    size_t determine_encode_length(codepoint codepoint) const
    {
        if constexpr (!Codec::variable_length_encoding)
        {
            assert(Codec::determine_encode_length({codepoint::max_unicode}) == 1);
            return 1;
        }

        strings::codepoint replacement;
        if constexpr (std::is_same_v<Codec, AsciiCodec>)
            replacement = _error_handler->replacement_ascii;
        else
            replacement = _error_handler->replacement_unicode;

        return Codec::determine_encode_length(codepoint, replacement);
    }



    /**
     * Encodes a given code point into the target encoding.
     */
    string_type encode_one(codepoint codepoint) const
    {
        string_type output(determine_encode_length(codepoint), typename string_type::value_type());
        encode_one_into(codepoint, cl7::make_string_span(output));
        return output;
    }

    /**
     * Encodes a given code point into the target encoding and stores the result in
     * the provided output string.
     */
    void encode_one_into(codepoint codepoint, string_type& output) const
    {
        output.resize(determine_encode_length(codepoint));
        encode_one_into(codepoint, cl7::make_string_span(output));
    }

    /**
     * Encodes a given code point into the target encoding and stores the result in
     * the provided output buffer, if space allows. The operation does not
     * null-terminate the output.
     */
    void encode_one_into(codepoint codepoint, string_span_type output) const
    {
        ErrorStatus error_status;
        _do_encode_one(error_status, codepoint, output);
    }

    /**
     * Encodes a given code point into the target encoding and stores the result in
     * the provided output string, returning a status indicating any errors
     * encountered.
     */
    ErrorStatus encode_one_with_status(codepoint codepoint, string_type& output) const
    {
        output.resize(determine_encode_length(codepoint));
        return encode_one_with_status(codepoint, cl7::make_string_span(output));
    }

    /**
     * Encodes a given code point into the target encoding and stores the result in
     * the provided output buffer, if space allows, returning a status indicating
     * any errors encountered. The operation does not null-terminate the output.
     */
    ErrorStatus encode_one_with_status(codepoint codepoint, string_span_type output) const
    {
        ErrorStatus error_status;
        _do_encode_one(error_status, codepoint, output);
        return error_status;
    }



private:
    void _do_encode_one(ErrorStatus& error_status, codepoint codepoint, string_span_type output) const
    {
        Codec::encode_one(error_status, codepoint, output, *_error_handler);
    }



    TDefaultErrorHandler _default_error_handler;
    const ErrorHandler* _error_handler;

}; // class Encoder



} // namespace cl7::strings

#endif // CL7_STRINGS_ENCODER_H

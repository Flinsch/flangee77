#ifndef CL7_TEXT_CODEC_ENCODER_H
#define CL7_TEXT_CODEC_ENCODER_H

#include "./EncodeResult.h"
#include "./DefaultErrorHandler.h"

#include "./traits.h"



namespace cl7::text::codec {



template <typename Tchar, class TDefaultErrorHandler = DefaultErrorHandler>
    requires(std::is_default_constructible_v<TDefaultErrorHandler>)
class Encoder
{

public:
    using Codec = traits<Tchar>::codec_type;
    using EncodeResult = Codec::EncodeResult;

    using string_span_type = traits<Tchar>::string_span_type;
    using string_type = traits<Tchar>::string_type;



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

        codec::codepoint replacement;
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
        _do_encode_one(codepoint, cl7::make_string_span(output));
        return output;
    }

    /**
     * Encodes a given code point into the target encoding and stores the result in
     * the provided output string, returning the number of code units written.
     */
    size_t encode_one_into(codepoint codepoint, string_type& output) const
    {
        output.resize(determine_encode_length(codepoint));
        return encode_one_into(codepoint, cl7::make_string_span(output));
    }

    /**
     * Encodes a given code point into the target encoding and stores the result in
     * the provided output buffer, if space allows, returning the number of code
     * units written. The operation does not null-terminate the output.
     */
    size_t encode_one_into(codepoint codepoint, string_span_type output) const
    {
        return _do_encode_one(codepoint, output).output_written.size();
    }

    /**
     * Encodes a given code point into the target encoding and stores the result in
     * the provided output string, returning a result indicating any errors
     * encountered, the number of code units written, etc.
     */
    EncodeResult encode_one_with_result(codepoint codepoint, string_type& output) const
    {
        output.resize(determine_encode_length(codepoint));
        return encode_one_with_result(codepoint, cl7::make_string_span(output));
    }

    /**
     * Encodes a given code point into the target encoding and stores the result in
     * the provided output buffer, if space allows, returning a result indicating
     * any errors encountered, the number of code units written, etc. The operation
     * does not null-terminate the output.
     */
    EncodeResult encode_one_with_result(codepoint codepoint, string_span_type output) const
    {
        return _do_encode_one(codepoint, output);
    }



private:
    EncodeResult _do_encode_one(codepoint codepoint, string_span_type output) const
    {
        ErrorStatus error_status;
        return Codec::encode_one(error_status, codepoint, output, *_error_handler);
    }



    TDefaultErrorHandler _default_error_handler;
    const ErrorHandler* _error_handler;

}; // class Encoder



} // namespace cl7::text::codec

#endif // CL7_TEXT_CODEC_ENCODER_H

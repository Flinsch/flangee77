#ifndef CL7_STRINGS_DECODER_H
#define CL7_STRINGS_DECODER_H

#include "./Analyzer.h"



namespace cl7::strings {



template <typename Tchar, class TDefaultErrorHandler = DefaultErrorHandler>
    requires(std::is_default_constructible_v<TDefaultErrorHandler>)
class Decoder
{

public:
    using Codec = typename traits<Tchar>::codec_type;

    using string_view_type = typename traits<Tchar>::string_view_type;



    Decoder(const ErrorHandler* error_handler = nullptr)
        : _default_error_handler()
        , _error_handler(error_handler ? error_handler : &_default_error_handler)
    {
    }



    /**
     * Returns the number of code points that a given input represents.
     */
    size_t determine_length(string_view_type input) const
    {
        return Analyzer<Tchar>(_error_handler).determine_length(input);
    }

    /**
     * Returns the number of code units that would be read/consumed when decoding a
     * single code point from the given input.
     */
    size_t determine_decode_length(string_view_type input) const
    {
        return Codec::determine_decode_length(input);
    }



    /**
     * Decodes a single code point from the given input string.
     */
    codepoint decode_one(string_view_type input) const
    {
        ErrorStatus error_status;
        return _do_decode_one(error_status, input);
    }

    /**
     * Decodes a single code point from the given input string, returning a status
     * indicating any errors encountered.
     */
    ErrorStatus decode_one_with_status(string_view_type input, codepoint& codepoint) const
    {
        ErrorStatus error_status;
        codepoint = _do_decode_one(error_status, input);
        return error_status;
    }



private:
    codepoint _do_decode_one(ErrorStatus& error_status, string_view_type input) const
    {
        return Codec::decode_one(error_status, input, *_error_handler).codepoint;
    }



    TDefaultErrorHandler _default_error_handler;
    const ErrorHandler* _error_handler;

}; // class Decoder



} // namespace cl7::strings

#endif // CL7_STRINGS_DECODER_H

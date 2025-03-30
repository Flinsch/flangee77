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
    using DecodeResult = typename Codec::DecodeResult;

    using string_view_type = typename traits<Tchar>::string_view_type;



    class sentinel {};

    class iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = codepoint;
        using pointer = const codepoint*;
        using reference = codepoint;

        iterator() = delete;
        iterator(string_view_type input, const ErrorHandler* error_handler = nullptr)
            : _input(input)
            , _default_error_handler()
            , _error_handler(error_handler ? error_handler : &_default_error_handler)
        {
        }

        reference operator*() const { return _codepoint; }
        pointer operator->() const { return &_codepoint; }
        iterator& operator++() { _advance(); return *this; }
        iterator operator++(int) { iterator it = *this; ++(*this); return it; }
        friend bool operator==(const iterator& a, sentinel b) noexcept { return a._pos >= a._input.size(); }
        friend bool operator!=(const iterator& a, sentinel b) noexcept { return !(a == b); }

    private:
        void _advance()
        {
            if (_pos >= _input.size())
            {
                _codepoint = {0};
                return;
            }

            ErrorStatus error_status;
            auto decode_result = Codec::decode_one(error_status, _input.substr(_pos), *_error_handler);
            _codepoint = decode_result.codepoint;
            _pos += decode_result.input_read.size();
        }

        string_view_type _input;

        size_t _pos = 0;
        codepoint _codepoint = {0};

        TDefaultErrorHandler _default_error_handler;
        const ErrorHandler* _error_handler;
    }; // class iterator



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
        return Codec::decode_one(error_status, input, *_error_handler).codepoint;
    }

    /**
     * Decodes a single code point from the given input string, returning the number
     * of code units read.
     */
    size_t decode_one_into(string_view_type input, codepoint& codepoint) const
    {
        const auto decode_result = _do_decode_one(input);
        codepoint = decode_result.codepoint;
        return decode_result.input_read.size();
    }

    /**
     * Decodes a single code point from the given input string, returning a result
     * indicating any errors encountered, the number of code units read, etc.
     */
    DecodeResult decode_one_with_result(string_view_type input) const
    {
        return _do_decode_one(input);
    }



private:
    DecodeResult _do_decode_one(string_view_type input) const
    {
        ErrorStatus error_status;
        return Codec::decode_one(error_status, input, *_error_handler);
    }



    TDefaultErrorHandler _default_error_handler;
    const ErrorHandler* _error_handler;

}; // class Decoder



} // namespace cl7::strings

#endif // CL7_STRINGS_DECODER_H

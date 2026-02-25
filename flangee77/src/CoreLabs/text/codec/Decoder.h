#ifndef CL7_TEXT_CODEC_DECODER_H
#define CL7_TEXT_CODEC_DECODER_H

#include "./Analyzer.h"



namespace cl7::text::codec {



/**
 * Decodes sequences of characters (code units, e.g., encoded as ASCII, UTF-8,
 * UTF-16, etc.) into (Unicode) code points.
 */
template <cl7::any_char Tchar, class TDefaultErrorHandler = DefaultErrorHandler>
    requires(std::is_default_constructible_v<TDefaultErrorHandler>)
class Decoder
{

public:
    using Codec = traits<Tchar>::codec_type;
    using DecodeResult = Codec::DecodeResult;

    using string_view_type = traits<Tchar>::string_view_type;



    /**
     * A forward iterator for traversing decoded (Unicode) code points from an
     * encoded character sequence.
     */
    class iterator
    {
        static_assert(std::is_copy_constructible_v<TDefaultErrorHandler> && std::is_copy_assignable_v<TDefaultErrorHandler>);

    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = codepoint;
        using pointer = const codepoint*;
        using reference = codepoint;

        iterator() = default;
        iterator(string_view_type input, const ErrorHandler* error_handler = nullptr)
            : _input(input)
            , _default_error_handler()
            , _error_handler(error_handler ? error_handler : &_default_error_handler)
        {
            _decode();
        }

        friend void swap(iterator& a, iterator& b) noexcept { std::swap(a, b); }

        /** Returns the input string this iterator traverses. */
        string_view_type input() const { return _input; }

        /**
         * Resets the current offset position of the read cursor.
         * Caution! Since the cursor is measured in code units, you can mess things
         * up if you place it on a code unit that doesn't represent the beginning of
         * a code point.
         */
        void reset_pos(size_t pos = 0) { _pos = pos; _decode(); }
        /** Resets the cumulative error status. */
        void reset_status() { _error_status = {}; }

        /** Returns the current offset position of the read cursor pointing to the first code unit of the current code point. */
        size_t pos() const { return _pos; }
        /** Returns the cumulative error status. */
        const ErrorStatus& status() const { return _error_status; }
        /** Returns the current decode result. */
        const DecodeResult& result() const { return _decode_result; }

        iterator& operator++() { _advance_and_decode(); return *this; }
        iterator operator++(int) { iterator it = *this; ++(*this); return it; }

        friend bool operator==(const iterator& a, const iterator& b) noexcept { return (a._input == b._input && a._pos == b._pos) || (a._pos >= a._input.size() && b._pos >= b._input.size()); }

        reference operator*() const { return _decode_result.codepoint; }
        pointer operator->() const { return &_decode_result.codepoint; }

    private:
        void _decode()
        {
            if (_pos >= _input.size())
            {
                _decode_result = {};
                return;
            }

            _decode_result = Codec::decode_one(_error_status, _input.substr(_pos), *_error_handler);
        }

        void _advance_and_decode()
        {
            _pos += _decode_result.input_read.size();
            _decode();
        }

        string_view_type _input;

        size_t _pos = 0;
        ErrorStatus _error_status;
        DecodeResult _decode_result;

        TDefaultErrorHandler _default_error_handler;
        const ErrorHandler* _error_handler;
    }; // class iterator



    explicit Decoder(const ErrorHandler* error_handler = nullptr)
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

static_assert(std::forward_iterator<Decoder<char>::iterator>);



} // namespace cl7::text::codec

#endif // CL7_TEXT_CODEC_DECODER_H

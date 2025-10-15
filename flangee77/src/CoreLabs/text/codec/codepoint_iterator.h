#ifndef CL7_TEXT_CODEC_CODEPOINTITERATOR_H
#define CL7_TEXT_CODEC_CODEPOINTITERATOR_H

#include "./Decoder.h"



namespace cl7::text::codec {



/**
 * Provides random access to the sequence of (Unicode) code points within a string.
 * It abstracts away the underlying encoding (e.g., ASCII, UTF-8, UTF-16, etc.) and
 * allows iteration, indexing, and arithmetic operations directly on code points,
 * not code units.
 */
template <cl7::any_char Tchar, class TDefaultErrorHandler = DefaultErrorHandler>
    requires(std::is_default_constructible_v<TDefaultErrorHandler>)
class codepoint_iterator
{
    static_assert(std::is_copy_constructible_v<TDefaultErrorHandler> && std::is_copy_assignable_v<TDefaultErrorHandler>);

public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = codepoint;
    using pointer = const codepoint*;
    using reference = codepoint;

    using string_view_type = traits<Tchar>::string_view_type;



    codepoint_iterator() = default;

    codepoint_iterator(string_view_type input, const ErrorHandler* error_handler = nullptr)
        : _decoder_iterator(input, error_handler)
    {
        Decoder<Tchar, TDefaultErrorHandler> decoder(error_handler);
        _codepoint_count = decoder.determine_length(input);
    }

    codepoint_iterator(string_view_type input, size_t codepoint_index, const ErrorHandler* error_handler = nullptr)
        : codepoint_iterator(input, error_handler)
    {
        *this += static_cast<difference_type>(codepoint_index);
    }

    friend void swap(codepoint_iterator& a, codepoint_iterator& b) noexcept { std::swap(a, b); }



    /** Returns the input string this iterator traverses. */
    string_view_type input() const { return _decoder_iterator.input(); }



    /** Returns the total length of the input text in code units. */
    size_t input_length() const { return input().size(); }
    /** Returns the total number of code points in the input text. */
    size_t codepoint_count() const { return _codepoint_count; }

    /** Returns the index of the first code unit belonging to the current code point. */
    size_t input_index() const { return _decoder_iterator.pos(); }
    /** Returns the current code point index (i.e., the logical position in the sequence of code points). */
    size_t codepoint_index() const { return _codepoint_index; }



    codepoint_iterator& operator++() { *this += 1; return *this; }
    codepoint_iterator operator++(int) { codepoint_iterator it = *this; ++(*this); return it; }
    codepoint_iterator& operator--() { *this -= 1; return *this; }
    codepoint_iterator operator--(int) { codepoint_iterator it = *this; --(*this); return it; }

    friend bool operator==(const codepoint_iterator& a, const codepoint_iterator& b) noexcept { return (a.input() == b.input() && a._codepoint_index == b._codepoint_index) || (a._codepoint_index >= a._codepoint_count && b._codepoint_index >= b._codepoint_count); }
    friend auto operator<=>(const codepoint_iterator& a, const codepoint_iterator& b) noexcept { assert(a.input() == b.input()); return a._codepoint_index <=> b._codepoint_index; }

    friend codepoint_iterator operator+(difference_type n, const codepoint_iterator& it) noexcept { return it + n; }
    friend codepoint_iterator operator+(codepoint_iterator it, difference_type n) noexcept { it += n; return it; }
    friend codepoint_iterator operator-(codepoint_iterator it, difference_type n) noexcept { it -= n; return it; }
    friend difference_type operator-(const codepoint_iterator& a, const codepoint_iterator& b) noexcept { assert(a.input() == b.input()); return a._codepoint_index - b._codepoint_index; }

    codepoint_iterator& operator+=(difference_type n) noexcept
    {
        if (n < 0)
            return *this -= -n;

        assert(_codepoint_index + n <= _codepoint_count);
        if (_codepoint_index + n > _codepoint_count)
            n = _codepoint_count - _codepoint_index;
        assert(_codepoint_index + n <= _codepoint_count);

        while (n--)
        {
            ++_codepoint_index;
            ++_decoder_iterator;
        }

        assert(_codepoint_index <= _codepoint_count);

        return *this;
    }

    codepoint_iterator& operator-=(difference_type n) noexcept
    {
        if (n < 0)
            return *this += -n;

        const auto i = static_cast<size_t>(n);
        assert(i <= _codepoint_index);
        if (i >= _codepoint_index)
            n = 0;
        else
            n = _codepoint_index - i;
        assert(n >= 0);

        _codepoint_index = 0;
        _decoder_iterator.reset_pos(0);

        return *this += n;
    }

    reference operator[](difference_type n) const { return *(*this + n); }
    reference operator*() const { return _decoder_iterator.operator*(); }
    pointer operator->() const { return _decoder_iterator.operator->(); }



private:
    size_t _codepoint_count = 0;
    size_t _codepoint_index = 0;
    Decoder<Tchar, TDefaultErrorHandler>::iterator _decoder_iterator;

}; // class codepoint_iterator

static_assert(std::random_access_iterator<codepoint_iterator<char>>);



} // namespace cl7::text::codec

#endif // CL7_TEXT_CODEC_CODEPOINTITERATOR_H

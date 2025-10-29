#ifndef CL7_TEXT_TRANSFORM_H
#define CL7_TEXT_TRANSFORM_H

#include "./inspect.h"

#include <CoreLabs/string.h>



namespace cl7::text::transform {



    /** Removes leading (Unicode) whitespace characters from the string in-place. */
    template <any_string Tstring>
        requires(!std::is_const_v<Tstring>)
    void trim_left(Tstring& s)
    {
        const size_t k = inspect::count_whitespace_prefix(make_string_view(s));
        assert(k <= s.size());
        s.erase(0, k);
    }

    /** Removes leading (Unicode) whitespace characters from the string in-place. */
    template <any_string_view Tstring_view>
        requires(!std::is_const_v<Tstring_view>)
    void trim_left(Tstring_view& s)
    {
        const size_t k = inspect::count_whitespace_prefix(Tstring_view(s));
        assert(k <= s.size());
        s.remove_prefix(k);
    }

    /** Removes trailing (Unicode) whitespace characters from the string in-place. */
    template <any_string Tstring>
        requires(!std::is_const_v<Tstring>)
    void trim_right(Tstring& s)
    {
        const size_t k = inspect::count_whitespace_suffix(make_string_view(s));
        assert(k <= s.size());
        s.erase(s.size() - k, k);
    }

    /** Removes trailing (Unicode) whitespace characters from the string in-place. */
    template <any_string_view Tstring_view>
        requires(!std::is_const_v<Tstring_view>)
    void trim_right(Tstring_view& s)
    {
        const size_t k = inspect::count_whitespace_suffix(Tstring_view(s));
        assert(k <= s.size());
        s.remove_suffix(k);
    }

    /** Removes both leading and trailing (Unicode) whitespace characters from the string in-place. */
    template <any_string_or_view Tstring_or_view>
        requires(!std::is_const_v<Tstring_or_view>)
    void trim(Tstring_or_view& s)
    {
        trim_right(s);
        trim_left(s);
    }



    /** Removes leading occurrences of the specified character from the string in-place. */
    template <any_string Tstring, any_char Tchar>
        requires(!std::is_const_v<Tstring>)
    void trim_left(Tstring& s, Tchar c)
    {
        while (!s.empty() && s.front() == c)
            s.erase(0, 1);
    }

    /** Removes leading occurrences of the specified character from the string in-place. */
    template <any_string_view Tstring_view, any_char Tchar>
        requires(!std::is_const_v<Tstring_view>)
    void trim_left(Tstring_view& s, Tchar c)
    {
        while (!s.empty() && s.front() == c)
            s.remove_prefix(1);
    }

    /** Removes trailing occurrences of the specified character from the string in-place. */
    template <any_string Tstring, any_char Tchar>
        requires(!std::is_const_v<Tstring>)
    void trim_right(Tstring& s, Tchar c)
    {
        while (!s.empty() && s.back() == c)
            s.erase(s.size() - 1, 1);
    }

    /** Removes trailing occurrences of the specified character from the string in-place. */
    template <any_string_view Tstring_view, any_char Tchar>
        requires(!std::is_const_v<Tstring_view>)
    void trim_right(Tstring_view& s, Tchar c)
    {
        while (!s.empty() && s.back() == c)
            s.remove_suffix(1);
    }

    /** Removes both leading and trailing occurrences of the specified character from the string in-place. */
    template <any_string_or_view Tstring_or_view, any_char Tchar>
        requires(!std::is_const_v<Tstring_or_view>)
    void trim(Tstring_or_view& s, Tchar c)
    {
        trim_right(s, c);
        trim_left(s, c);
    }



    /** Removes leading occurrences of the specified characters from the string in-place. */
    template <any_string Tstring, any_string_view_like Tstring_view_like>
        requires(!std::is_const_v<Tstring> && std::same_as<typename Tstring::value_type, char_type_of_t<Tstring_view_like>>)
    void trim_left(Tstring& s, Tstring_view_like chars)
    {
        using Tchar = char_type_of_t<Tstring_view_like>;
        using Tstring_view = std::basic_string_view<Tchar>;
        Tstring_view sv{chars};
        while (!s.empty() && sv.find(s.front()) != Tstring_view::npos)
            s.erase(0, 1);
    }

    /** Removes leading occurrences of the specified characters from the string in-place. */
    template <any_string_view Tstring_view, any_string_view_like Tstring_view_like>
        requires(!std::is_const_v<Tstring_view> && std::same_as<typename Tstring_view::value_type, char_type_of_t<Tstring_view_like>>)
    void trim_left(Tstring_view& s, Tstring_view_like chars)
    {
        Tstring_view sv{chars};
        while (!s.empty() && sv.find(s.front()) != Tstring_view::npos)
            s.remove_prefix(1);
    }

    /** Removes trailing occurrences of the specified characters from the string in-place. */
    template <any_string Tstring, any_string_view_like Tstring_view_like>
        requires(!std::is_const_v<Tstring> && std::same_as<typename Tstring::value_type, char_type_of_t<Tstring_view_like>>)
    void trim_right(Tstring& s, Tstring_view_like chars)
    {
        using Tchar = char_type_of_t<Tstring_view_like>;
        using Tstring_view = std::basic_string_view<Tchar>;
        Tstring_view sv{chars};
        while (!s.empty() && sv.find(s.back()) != Tstring_view::npos)
            s.erase(s.size() - 1, 1);
    }

    /** Removes trailing occurrences of the specified characters from the string in-place. */
    template <any_string_view Tstring_view, any_string_view_like Tstring_view_like>
        requires(!std::is_const_v<Tstring_view> && std::same_as<typename Tstring_view::value_type, char_type_of_t<Tstring_view_like>>)
    void trim_right(Tstring_view& s, Tstring_view_like chars)
    {
        Tstring_view sv{chars};
        while (!s.empty() && sv.find(s.back()) != Tstring_view::npos)
            s.remove_suffix(1);
    }

    /** Removes both leading and trailing occurrences of the specified characters from the string in-place. */
    template <any_string_or_view Tstring_or_view, any_string_view_like Tstring_view_like>
        requires(!std::is_const_v<Tstring_or_view> && std::same_as<typename Tstring_or_view::value_type, char_type_of_t<Tstring_view_like>>)
    void trim(Tstring_or_view& s, Tstring_view_like chars)
    {
        trim_right(s, chars);
        trim_left(s, chars);
    }



    /** Returns (a copy of) the string with leading (Unicode) whitespace removed. */
    template <any_string_or_view Tstring_or_view>
    Tstring_or_view trimmed_left(Tstring_or_view s)
    {
        trim_left(s);
        return s;
    }

    /** Returns (a copy of) the string with trailing (Unicode) whitespace removed. */
    template <any_string_or_view Tstring_or_view>
    Tstring_or_view trimmed_right(Tstring_or_view s)
    {
        trim_right(s);
        return s;
    }

    /** Returns (a copy of) the string with leading and trailing (Unicode) whitespace removed. */
    template <any_string_or_view Tstring_or_view>
    Tstring_or_view trimmed(Tstring_or_view s)
    {
        trim(s);
        return s;
    }



    /** Returns (a copy of) the string with leading occurrences of the specified character removed. */
    template <any_string_or_view Tstring_or_view, any_char Tchar>
    Tstring_or_view trimmed_left(Tstring_or_view s, Tchar c)
    {
        trim_left(s, c);
        return s;
    }

    /** Returns (a copy of) the string with trailing occurrences of the specified character removed. */
    template <any_string_or_view Tstring_or_view, any_char Tchar>
    Tstring_or_view trimmed_right(Tstring_or_view s, Tchar c)
    {
        trim_right(s, c);
        return s;
    }

    /** Returns (a copy of) the string with leading and trailing occurrences of the specified character removed. */
    template <any_string_or_view Tstring_or_view, any_char Tchar>
    Tstring_or_view trimmed(Tstring_or_view s, Tchar c)
    {
        trim(s, c);
        return s;
    }



    /** Returns (a copy of) the string with leading occurrences of the specified characters removed. */
    template <any_string_or_view Tstring_or_view, any_string_view_like Tstring_view_like>
        requires(std::same_as<typename Tstring_or_view::value_type, char_type_of_t<Tstring_view_like>>)
    Tstring_or_view trimmed_left(Tstring_or_view s, Tstring_view_like chars)
    {
        trim_left(s, chars);
        return s;
    }

    /** Returns (a copy of) the string with trailing occurrences of the specified characters removed. */
    template <any_string_or_view Tstring_or_view, any_string_view_like Tstring_view_like>
        requires(std::same_as<typename Tstring_or_view::value_type, char_type_of_t<Tstring_view_like>>)
    Tstring_or_view trimmed_right(Tstring_or_view s, Tstring_view_like chars)
    {
        trim_right(s, chars);
        return s;
    }

    /** Returns (a copy of) the string with leading and trailing occurrences of the specified characters removed. */
    template <any_string_or_view Tstring_or_view, any_string_view_like Tstring_view_like>
        requires(std::same_as<typename Tstring_or_view::value_type, char_type_of_t<Tstring_view_like>>)
    Tstring_or_view trimmed(Tstring_or_view s, Tstring_view_like chars)
    {
        trim(s, chars);
        return s;
    }


    /** Pads the string on the left with the specified character until it has at least the desired length. */
    template <any_string Tstring, any_char Tchar>
        requires(!std::is_const_v<Tstring>)
    void pad_left(Tstring& s, size_t min_length, Tchar c)
    {
        if (min_length > s.length())
            s.insert(s.begin(), min_length - s.length(), c);
    }

    /** Pads the string on the right with the specified character until it has at least the desired length. */
    template <any_string Tstring, any_char Tchar>
        requires(!std::is_const_v<Tstring>)
    void pad_right(Tstring& s, size_t min_length, Tchar c)
    {
        if (min_length > s.length())
            s.append(min_length - s.length(), c);
    }



    /** Returns a copy of the string left-padded with the specified character to reach the desired minimum length. */
    template <any_string Tstring, any_char Tchar>
    Tstring padded_left(Tstring s, size_t min_length, Tchar c)
    {
        pad_left(s, min_length, c);
        return s;
    }

    /** Returns a copy of the string right-padded with the specified character to reach the desired minimum length. */
    template <any_string Tstring, any_char Tchar>
    Tstring padded_right(Tstring s, size_t min_length, Tchar c)
    {
        pad_right(s, min_length, c);
        return s;
    }



    /** Converts a single ASCII character to lowercase. */
    template <any_char Tchar>
    Tchar to_lower_ascii(Tchar c)
    {
        if (c >= Tchar{'A'} && c <= Tchar{'Z'})
            return c + 0x20;
        return c;
    }

    /** Converts a single ASCII character to uppercase. */
    template <any_char Tchar>
    Tchar to_upper_ascii(Tchar c)
    {
        if (c >= Tchar{'a'} && c <= Tchar{'z'})
            return c - 0x20;
        return c;
    }



    /** Converts all ASCII letters in the string to lowercase in-place. */
    template <any_string_or_span Tstring_or_span>
        requires(!std::is_const_v<Tstring_or_span>)
    Tstring_or_span& to_lower_ascii_inplace(Tstring_or_span& s)
    {
        for (auto& c : s)
        {
            if (c >= typename Tstring_or_span::value_type('A') && c <= typename Tstring_or_span::value_type('Z'))
                c += 0x20;
        }
        return s;
    }

    /** Converts all ASCII letters in the string to uppercase in-place. */
    template <any_string_or_span Tstring_or_span>
        requires(!std::is_const_v<Tstring_or_span>)
    Tstring_or_span& to_upper_ascii_inplace(Tstring_or_span& s)
    {
        for (auto& c : s)
        {
            if (c >= typename Tstring_or_span::value_type('a') && c <= typename Tstring_or_span::value_type('z'))
                c -= 0x20;
        }
        return s;
    }



    /** Returns a copy of the string with all ASCII letters converted to lowercase. */
    template <any_string_or_view Tstring_or_view, any_string Tstring = std::basic_string<typename Tstring_or_view::value_type, typename Tstring_or_view::traits_type>>
    Tstring to_lower_ascii(Tstring_or_view s)
    {
        Tstring s_{std::move(s)};
        to_lower_ascii_inplace(s_);
        return s_;
    }

    /** Returns a copy of the string with all ASCII letters converted to uppercase. */
    template <any_string_or_view Tstring_or_view, any_string Tstring = std::basic_string<typename Tstring_or_view::value_type, typename Tstring_or_view::traits_type>>
    Tstring to_upper_ascii(Tstring_or_view s)
    {
        Tstring s_{std::move(s)};
        to_upper_ascii_inplace(s_);
        return s_;
    }



} // namespace cl7::text::transform

#endif // CL7_TEXT_TRANSFORM_H

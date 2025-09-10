#ifndef CL7_STRINGS_TRANSFORM_H
#define CL7_STRINGS_TRANSFORM_H

#include "./inspect.h"

#include <CoreLabs/string.h>



namespace cl7::strings::transform {



    template <class Tstring>
        requires(is_any_string_v<Tstring> && !std::is_const_v<Tstring>)
    void trim_left(Tstring& s)
    {
        const size_t k = inspect::count_whitespace_prefix(make_string_view(s));
        assert(k <= s.size());
        s.erase(0, k);
    }

    template <class Tstring>
        requires(is_any_string_v<Tstring> && !std::is_const_v<Tstring>)
    void trim_right(Tstring& s)
    {
        const size_t k = inspect::count_whitespace_suffix(make_string_view(s));
        assert(k <= s.size());
        s.erase(s.size() - k, k);
    }

    template <class Tstring_view>
        requires(is_any_string_view_v<Tstring_view>)
    void trim_left(Tstring_view& s)
    {
        const size_t k = inspect::count_whitespace_prefix(Tstring_view(s));
        assert(k <= s.size());
        s = s.substr(k);
    }

    template <class Tstring_view>
        requires(is_any_string_view_v<Tstring_view>)
    void trim_right(Tstring_view& s)
    {
        const size_t k = inspect::count_whitespace_suffix(Tstring_view(s));
        assert(k <= s.size());
        s = s.substr(0, s.size() - k);
    }

    template <class Tstring_or_view>
        requires(is_any_string_or_view_v<Tstring_or_view>)
    void trim(Tstring_or_view& s)
    {
        trim_right(s);
        trim_left(s);
    }



    template <class Tstring_or_view>
        requires(is_any_string_or_view_v<Tstring_or_view>)
    Tstring_or_view trimmed_left(Tstring_or_view s)
    {
        trim_left(s);
        return s;
    }

    template <class Tstring_or_view>
        requires(is_any_string_or_view_v<Tstring_or_view>)
    Tstring_or_view trimmed_right(Tstring_or_view s)
    {
        trim_right(s);
        return s;
    }

    template <class Tstring_or_view>
        requires(is_any_string_or_view_v<Tstring_or_view>)
    Tstring_or_view trimmed(Tstring_or_view s)
    {
        trim(s);
        return s;
    }



    template <class Tstring, typename Tchar = Tstring::value_type>
        requires(is_any_string_v<Tstring> && !std::is_const_v<Tstring> && std::is_integral_v<Tchar>)
    void pad_left(Tstring& s, size_t min_length, Tchar c)
    {
        if (min_length > s.length())
            s.insert(s.begin(), min_length - s.length(), c);
    }

    template <class Tstring, typename Tchar = Tstring::value_type>
        requires(is_any_string_v<Tstring> && !std::is_const_v<Tstring> && std::is_integral_v<Tchar>)
    void pad_right(Tstring& s, size_t min_length, Tchar c)
    {
        if (min_length > s.length())
            s.append(min_length - s.length(), c);
    }



    template <class Tstring, typename Tchar = Tstring::value_type>
        requires(is_any_string_v<Tstring> && std::is_integral_v<Tchar>)
    Tstring padded_left(Tstring s, size_t min_length, Tchar c)
    {
        pad_left(s, min_length, c);
        return s;
    }

    template <class Tstring, typename Tchar = Tstring::value_type>
        requires(is_any_string_v<Tstring> && std::is_integral_v<Tchar>)
    Tstring padded_right(Tstring s, size_t min_length, Tchar c)
    {
        pad_right(s, min_length, c);
        return s;
    }



    template <typename Tchar>
        requires(std::is_integral_v<Tchar>)
    Tchar to_lower_ascii(Tchar c)
    {
        if (c >= Tchar{'A'} && c <= Tchar{'Z'})
            return c + 0x20;
        return c;
    }

    template <typename Tchar>
        requires(std::is_integral_v<Tchar>)
    Tchar to_upper_ascii(Tchar c)
    {
        if (c >= Tchar{'a'} && c <= Tchar{'z'})
            return c - 0x20;
        return c;
    }



    template <class Tstring_or_span>
        requires(is_any_string_or_span_v<Tstring_or_span> && !std::is_const_v<Tstring_or_span>)
    Tstring_or_span& to_lower_ascii_inplace(Tstring_or_span& s)
    {
        for (auto& c : s)
        {
            if (c >= typename Tstring_or_span::value_type('A') && c <= typename Tstring_or_span::value_type('Z'))
                c += 0x20;
        }
        return s;
    }

    template <class Tstring_or_span>
        requires(is_any_string_or_span_v<Tstring_or_span> && !std::is_const_v<Tstring_or_span>)
    Tstring_or_span& to_upper_ascii_inplace(Tstring_or_span& s)
    {
        for (auto& c : s)
        {
            if (c >= typename Tstring_or_span::value_type('a') && c <= typename Tstring_or_span::value_type('z'))
                c -= 0x20;
        }
        return s;
    }



    template <class Tstring_or_view, class Tstring = std::basic_string<typename Tstring_or_view::value_type>>
        requires(is_any_string_or_view_v<Tstring_or_view>)
    Tstring to_lower_ascii(Tstring_or_view s)
    {
        Tstring s_{std::move(s)};
        to_lower_ascii_inplace(s_);
        return s_;
    }

    template <class Tstring_or_view, class Tstring = std::basic_string<typename Tstring_or_view::value_type>>
        requires(is_any_string_or_view_v<Tstring_or_view>)
    Tstring to_upper_ascii(Tstring_or_view s)
    {
        Tstring s_{std::move(s)};
        to_upper_ascii_inplace(s_);
        return s_;
    }



} // namespace cl7::strings::transform

#endif // CL7_STRINGS_TRANSFORM_H

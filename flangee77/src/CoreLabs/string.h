#ifndef CL7_STRING_H
#define CL7_STRING_H

#include <CoreLabs/root.h>

#include <string>
#include <string_view>
#include <span>



#if (defined(UNICODE) && !defined(_UNICODE)) || (!defined(UNICODE) && defined(_UNICODE))
#error Inconsistent UNICODE and _UNICODE definition.
#elif !defined(UNICODE) || !defined(_UNICODE)
// Force UNICODE/_UNICODE definition, as suggested by the "UTF-8 Everywhere" manifesto.
#error Missing UNICODE and/or _UNICODE definition.
#endif



namespace cl7 {



    using achar_t           = char;
    using u8char_t          = char8_t;
    using u16char_t         = char16_t;
    using u32char_t         = char32_t;

    using astring           = std::basic_string<achar_t>;
    using u8string          = std::basic_string<u8char_t>;
    using u16string         = std::basic_string<u16char_t>;
    using u32string         = std::basic_string<u32char_t>;

    using astring_view      = std::basic_string_view<achar_t>;
    using u8string_view     = std::basic_string_view<u8char_t>;
    using u16string_view    = std::basic_string_view<u16char_t>;
    using u32string_view    = std::basic_string_view<u32char_t>;

    using astring_span      = std::span<achar_t>;
    using u8string_span     = std::span<u8char_t>;
    using u16string_span    = std::span<u16char_t>;
    using u32string_span    = std::span<u32char_t>;



    template <class Tstring>
    concept is_any_string_v = std::is_base_of_v<std::basic_string<typename Tstring::value_type, typename Tstring::traits_type, typename Tstring::allocator_type>, Tstring>;

    template <class Tstring_view>
    concept is_any_string_view_v = std::is_base_of_v<std::basic_string_view<typename Tstring_view::value_type, typename Tstring_view::traits_type>, Tstring_view>;

    template <class Tstring_span>
    concept is_any_string_span_v = std::is_base_of_v<std::span<typename Tstring_span::value_type>, Tstring_span>;

    template <class Tstring_or_view>
    concept is_any_string_or_view_v = is_any_string_v<Tstring_or_view> || is_any_string_view_v<Tstring_or_view>;

    template <class Tstring_or_span>
    concept is_any_string_or_span_v = is_any_string_v<Tstring_or_span> || is_any_string_span_v<Tstring_or_span>;



    template <class Tstring_or_span>
        requires(is_any_string_or_span_v<Tstring_or_span>)
    auto make_string_view(const Tstring_or_span& s)
    {
        return std::basic_string_view<typename Tstring_or_span::value_type>(s.data(), s.size());
    }

    template <class Tstring>
        requires(is_any_string_v<Tstring>)
    auto make_string_span(Tstring& s)
    {
        return std::span<typename Tstring::value_type>(s.data(), s.size());
    }



    template <typename Tchar = cl7::u8char_t>
        requires(std::is_integral_v<Tchar>)
    struct string_hash
    {
        using is_transparent = void;
        using Tstring_view = std::basic_string_view<Tchar>;
        using Tstring = std::basic_string<Tchar>;
        size_t operator()(const Tchar* str) const { return std::hash<Tstring_view>{}(str); }
        size_t operator()(Tstring_view str) const { return std::hash<Tstring_view>{}(str); }
        size_t operator()(const Tstring& str) const { return std::hash<Tstring>{}(str); }
    };

    using astring_hash = string_hash<cl7::achar_t>;
    using u8string_hash = string_hash<cl7::u8char_t>;
    using u16string_hash = string_hash<cl7::u16char_t>;
    using u32string_hash = string_hash<cl7::u32char_t>;



    u8string to_string(bool val);

    u8string to_string(signed val);
    u8string to_string(signed long val);
    u8string to_string(signed long long val);
    u8string to_string(unsigned val);
    u8string to_string(unsigned long val);
    u8string to_string(unsigned long long val);
    u8string to_string(float val);
    u8string to_string(double val);
    u8string to_string(long double val);



} // namespace cl7

#endif // CL7_STRING_H

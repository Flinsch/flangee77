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



    template <typename Tchar>
    struct is_any_char : std::bool_constant<
        std::is_same_v<Tchar, char> ||
        std::is_same_v<Tchar, char8_t> ||
        std::is_same_v<Tchar, char16_t> ||
        std::is_same_v<Tchar, char32_t> ||
        std::is_same_v<Tchar, wchar_t> ||
        // This might seem a bit redundant,
        // but we prefer to be explicit here.
        std::is_same_v<Tchar, achar_t> ||
        std::is_same_v<Tchar, u8char_t> ||
        std::is_same_v<Tchar, u16char_t> ||
        std::is_same_v<Tchar, u32char_t>>
    {
    };

    template <typename Tchar>
    inline constexpr bool is_any_char_v = is_any_char<Tchar>::value;

    template <typename Tchar>
    concept any_char = is_any_char_v<Tchar>;



namespace detail {
    template <typename T>
    struct _is_any_string : std::false_type
    {
    };

    template <any_char Tchar, typename Ttraits, typename Talloc>
    struct _is_any_string<std::basic_string<Tchar, Ttraits, Talloc>> : std::true_type
    {
    };

    template <typename T>
    struct _is_any_string_view : std::false_type
    {
    };

    template <any_char Tchar, typename Ttraits>
    struct _is_any_string_view<std::basic_string_view<Tchar, Ttraits>> : std::true_type
    {
    };

    template <typename T>
    struct _is_any_string_span : std::false_type
    {
    };

    template <any_char Tchar>
    struct _is_any_string_span<std::span<Tchar>> : std::true_type
    {
    };
}



    template <typename Tstring>
    struct is_any_string : detail::_is_any_string<std::remove_cvref_t<Tstring>>
    {
    };

    template <typename Tstring_view>
    struct is_any_string_view : detail::_is_any_string_view<std::remove_cvref_t<Tstring_view>>
    {
    };

    template <typename Tstring_span>
    struct is_any_string_span : detail::_is_any_string_span<std::remove_cvref_t<Tstring_span>>
    {
    };

    template <typename Tstring_or_view>
    struct is_any_string_or_view
        : std::bool_constant<is_any_string<Tstring_or_view>::value || is_any_string_view<Tstring_or_view>::value>
    {
    };

    template <typename Tstring_or_span>
    struct is_any_string_or_span
        : std::bool_constant<is_any_string<Tstring_or_span>::value || is_any_string_span<Tstring_or_span>::value>
    {
    };

    template <typename Tstring>
    inline constexpr bool is_any_string_v = is_any_string<Tstring>::value;

    template <typename Tstring_view>
    inline constexpr bool is_any_string_view_v = is_any_string_view<Tstring_view>::value;

    template <typename Tstring_span>
    inline constexpr bool is_any_string_span_v = is_any_string_span<Tstring_span>::value;

    template <typename Tstring_or_view>
    inline constexpr bool is_any_string_or_view_v = is_any_string_or_view<Tstring_or_view>::value;

    template <typename Tstring_or_span>
    inline constexpr bool is_any_string_or_span_v = is_any_string_or_span<Tstring_or_span>::value;

    template <typename Tstring>
    concept any_string = is_any_string_v<Tstring>;

    template <typename Tstring_view>
    concept any_string_view = is_any_string_view_v<Tstring_view>;

    template <typename Tstring_span>
    concept any_string_span = is_any_string_span_v<Tstring_span>;

    template <typename Tstring_or_view>
    concept any_string_or_view = is_any_string_or_view_v<Tstring_or_view>;

    template <typename Tstring_or_span>
    concept any_string_or_span = is_any_string_or_span_v<Tstring_or_span>;



    template <any_string_or_view Tstring_or_view>
    auto make_string_view(const Tstring_or_view& s)
    {
        return std::basic_string_view<typename Tstring_or_view::value_type, typename Tstring_or_view::traits_type>(s.data(), s.size());
    }

    template <any_string_span Tstring_span>
    auto make_string_view(const Tstring_span& s)
    {
        return std::basic_string_view<typename Tstring_span::value_type>(s.data(), s.size());
    }

    template <any_string_or_span Tstring_or_span>
    auto make_string_span(Tstring_or_span& s)
    {
        return std::span<typename Tstring_or_span::value_type>(s.data(), s.size());
    }



    template <any_char Tchar = u8char_t>
    struct string_hash
    {
        using is_transparent = void;
        using Tstring_view = std::basic_string_view<Tchar>;
        using Tstring = std::basic_string<Tchar>;
        size_t operator()(const Tchar* str) const { return std::hash<Tstring_view>{}(str); }
        size_t operator()(Tstring_view str) const { return std::hash<Tstring_view>{}(str); }
        size_t operator()(const Tstring& str) const { return std::hash<Tstring>{}(str); }
    };

    using astring_hash = string_hash<achar_t>;
    using u8string_hash = string_hash<u8char_t>;
    using u16string_hash = string_hash<u16char_t>;
    using u32string_hash = string_hash<u32char_t>;



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

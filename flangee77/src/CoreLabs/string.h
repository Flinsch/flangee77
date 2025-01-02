#ifndef CL7_STRING_H
#define CL7_STRING_H

#include <CoreLabs/root.h>

#include <string>



#if defined(UNICODE) ^ defined(_UNICODE)
#error Inconsistent UNICODE and _UNICODE definition.
#elif !defined(UNICODE) || !defined(_UNICODE)
#error Missing UNICODE and/or _UNICODE definition.
#endif



namespace cl7 {



    using achar_type        = char;
    using u8char_type       = char8_t;
    using u16char_type      = char16_t;
    using u32char_type      = char32_t;
    using wchar_type        = wchar_t;

    using astring           = std::string;
    using u8string          = std::u8string;
    using u16string         = std::u16string;
    using u32string         = std::u32string;
    using wstring           = std::wstring;

    using astring_view      = std::string_view;
    using u8string_view     = std::u8string_view;
    using u16string_view    = std::u16string_view;
    using u32string_view    = std::u32string_view;
    using wstring_view      = std::wstring_view;



    template <class Tstring>
    concept is_any_string_v = std::is_base_of_v<std::basic_string<typename Tstring::value_type, typename Tstring::traits_type, typename Tstring::allocator_type>, Tstring>;

    template <class Tstring_view>
    concept is_any_string_view_v = std::is_base_of_v<std::basic_string_view<typename Tstring_view::value_type, typename Tstring_view::traits_type>, Tstring_view>;

    template <class Tstring_or_view>
    concept is_any_string_or_view_v = is_any_string_v<Tstring_or_view> || is_any_string_view_v<Tstring_or_view>;



    template <class Tstring>
        requires(is_any_string_v<Tstring>)
    auto make_string_view(const Tstring& s)
    {
        return std::basic_string_view<typename Tstring::value_type, typename Tstring::traits_type>(s.data(), s.size());
    }



    template <typename Tchar = cl7::u8char_type>
    struct string_hash
    {
        using is_transparent = void;
        using Tstring_view = std::basic_string_view<Tchar>;
        using Tstring = std::basic_string<Tchar>;
        size_t operator()(const Tchar* str) const { return std::hash<Tstring_view>{}(str); }
        size_t operator()(Tstring_view str) const { return std::hash<Tstring_view>{}(str); }
        size_t operator()(const Tstring& str) const { return std::hash<Tstring>{}(str); }
    };

    using astring_hash = string_hash<cl7::achar_type>;
    using u8string_hash = string_hash<cl7::u8char_type>;
    using u16string_hash = string_hash<cl7::u16char_type>;
    using u32string_hash = string_hash<cl7::u32char_type>;
    using wstring_hash = string_hash<cl7::wchar_type>;



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

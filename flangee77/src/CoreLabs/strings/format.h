#ifndef CL7_STRINGS_FORMAT_H
#define CL7_STRINGS_FORMAT_H

#include "./inspect.h"
#include "./transform.h"

#include <CoreLabs/string.h>

#include <algorithm>



namespace cl7::strings::format {



    template <class Tstring, typename Tval, typename Tchar = Tstring::value_type>
        requires(is_any_string_v<Tstring> && std::is_unsigned_v<Tval> && std::is_integral_v<Tchar>)
    Tstring to_hex(Tval val, unsigned pad_zeros = 0, Tchar ca = Tchar{'A'})
    {
        static constexpr auto c0 = Tchar{'0'};
        if (val == 0)
            return pad_zeros ? Tstring(pad_zeros, c0) : Tstring();
        Tstring s;
        s.reserve(18); // 16 + 2 (up to 64-bit hexadecimal number with possible prefix "0x")
        while (val)
        {
            auto x = static_cast<Tchar>(val & 0xf);
            val >>= 4;
            Tchar c = x < 0xa ? c0+x : ca+x-0xa;
            s += c;
        }
        transform::pad_right(s, pad_zeros, c0);
        std::reverse(s.begin(), s.end());
        return s;
    }

    template <class Tstring, typename Tval, typename Tchar = Tstring::value_type>
        requires(is_any_string_v<Tstring> && std::is_unsigned_v<Tval> && std::is_integral_v<Tchar>)
    Tstring to_0xhex(Tval val, unsigned pad_zeros = 0, Tchar ca = Tchar{'A'})
    {
        static constexpr auto c0 = Tchar{'0'};
        static constexpr auto cx = Tchar{'x'};
        auto s = to_hex<Tstring>(val, pad_zeros, ca);
        s.insert(s.begin(), 2, c0);
        s[1] = cx;
        return s;
    }

    template <class Tstring, typename Tval, typename Tchar = Tstring::value_type>
        requires(is_any_string_v<Tstring> && std::is_unsigned_v<Tval> && std::is_integral_v<Tchar>)
    Tstring to_hex_lc(Tval val, unsigned pad_zeros = 0)
    {
        return to_hex<Tstring>(val, pad_zeros, Tchar{'a'});
    }

    template <class Tstring, typename Tval, typename Tchar = Tstring::value_type>
        requires(is_any_string_v<Tstring> && std::is_unsigned_v<Tval> && std::is_integral_v<Tchar>)
    Tstring to_hex_uc(Tval val, unsigned pad_zeros = 0)
    {
        return to_hex<Tstring>(val, pad_zeros, Tchar{'A'});
    }

    template <class Tstring, typename Tval, typename Tchar = Tstring::value_type>
        requires(is_any_string_v<Tstring> && std::is_unsigned_v<Tval> && std::is_integral_v<Tchar>)
    Tstring to_0xhex_lc(Tval val, unsigned pad_zeros = 0)
    {
        return to_0xhex<Tstring>(val, pad_zeros, Tchar{'a'});
    }

    template <class Tstring, typename Tval, typename Tchar = Tstring::value_type>
        requires(is_any_string_v<Tstring> && std::is_unsigned_v<Tval> && std::is_integral_v<Tchar>)
    Tstring to_0xhex_uc(Tval val, unsigned pad_zeros = 0)
    {
        return to_0xhex<Tstring>(val, pad_zeros, Tchar{'A'});
    }

    u8string to_hex(unsigned long long val, unsigned pad_zeros = 0, u8char_t ca = u8'A');
    u8string to_0xhex(unsigned long long val, unsigned pad_zeros = 0, u8char_t ca = u8'A');

    inline u8string to_hex_lc(unsigned long long val, unsigned pad_zeros = 0) { return to_hex(val, pad_zeros, u8'a'); }
    inline u8string to_hex_uc(unsigned long long val, unsigned pad_zeros = 0) { return to_hex(val, pad_zeros, u8'A'); }
    inline u8string to_0xhex_lc(unsigned long long val, unsigned pad_zeros = 0) { return to_0xhex(val, pad_zeros, u8'a'); }
    inline u8string to_0xhex_uc(unsigned long long val, unsigned pad_zeros = 0) { return to_0xhex(val, pad_zeros, u8'A'); }



    template <typename Tval = unsigned, class Tstring_or_view, typename Tchar = std::remove_cvref_t<Tstring_or_view>::value_type>
        requires(std::is_integral_v<Tval> && is_any_string_or_view_v<Tstring_or_view>)
    Tval parse_hex(Tstring_or_view&& s)
    {
        static constexpr auto c0 = Tchar{'0'};
        static constexpr auto cx = Tchar{'x'};
        std::basic_string_view<typename std::remove_cvref_t<Tstring_or_view>::value_type> sv{std::forward<Tstring_or_view>(s)};
        Tval val = 0;
        size_t p = inspect::count_whitespace_prefix(sv);
        if (p + 1 < sv.size() && sv[p] == c0 && sv[p + 1] == cx)
            p += 2;
        for (; p < sv.size() && inspect::is_hex_digit(sv[p]); ++p)
        {
            val *= 0x10;
            val += inspect::is_digit(sv[p])
                ? static_cast<Tval>(sv[p]) - 0x30 // '0' = 48 = 0x30
                : (static_cast<Tval>(sv[p]) | 0x20) - 0x57; // 'a' = 97 = 0x61; set off 10: 87 = 0x57
        }
        return val;
    }



} // namespace cl7::strings::format

#endif // CL7_STRINGS_FORMAT_H

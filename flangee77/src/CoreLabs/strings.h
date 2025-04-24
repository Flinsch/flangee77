#ifndef CL7_STRINGS_H
#define CL7_STRINGS_H

#include "./strings/Encoding.h"

#include <CoreLabs/string.h>
#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>

#include <algorithm>
#include <bit>



namespace cl7::strings {



    astring to_ascii(astring_view as);
    astring to_ascii(u8string_view u8s);
    astring to_ascii(u16string_view u16s);
    astring to_ascii(u32string_view u32s);

    u8string to_utf8(astring_view as);
    u8string to_utf8(u8string_view u8s);
    u8string to_utf8(u16string_view u16s);
    u8string to_utf8(u32string_view u32s);

    u16string to_utf16(astring_view as);
    u16string to_utf16(u8string_view u8s);
    u16string to_utf16(u16string_view u16s);
    u16string to_utf16(u32string_view u32s);

    u32string to_utf32(astring_view as);
    u32string to_utf32(u8string_view u8s);
    u32string to_utf32(u16string_view u16s);
    u32string to_utf32(u32string_view u32s);

#if WCHAR_MAX == UINT16_MAX
    u16string to_utfx(astring_view as);
    u16string to_utfx(u8string_view u8s);
    u16string to_utfx(u16string_view u16s);
    u16string to_utfx(u32string_view u32s);
#endif
#if WCHAR_MAX == UINT32_MAX
    u32string to_utfx(astring_view as);
    u32string to_utfx(u8string_view u8s);
    u32string to_utfx(u16string_view u16s);
    u32string to_utfx(u32string_view u32s);
#endif

    astring to_ascii_unchecked(byte_view bys);
    u8string to_utf8_unchecked(byte_view bys);
    u16string to_utf16_unchecked(byte_view bys);
    u32string to_utf32_unchecked(byte_view bys);

    byte_vector to_bytes(astring_view as);
    byte_vector to_bytes(u8string_view u8s, bool add_bom = false);
    byte_vector to_bytes(u16string_view u16s, bool add_bom = false, std::endian endian = std::endian::native);
    byte_vector to_bytes(u32string_view u32s, bool add_bom = false, std::endian endian = std::endian::native);

    bool check_ascii(astring_view as, bool log_warning = false);
    bool check_utf8(u8string_view u8s, bool log_warning = false);
    bool check_utf16(u16string_view u16s, bool log_warning = false);
    bool check_utf32(u32string_view u32s, bool log_warning = false);

    bool parse_utf8(u8string_view u8s, u32string& u32s, bool log_warning = false);
    bool parse_utf16(u16string_view u16s, u32string& u32s, bool log_warning = false);

    /** Calculates the length of the specified UTF-8 string in terms of Unicode code points. */
    size_t utf8_length(u8string_view u8s);
    /** Calculates the length of the specified UTF-16 string in terms of Unicode code points. */
    size_t utf16_length(u16string_view u16s);

    /** Reinterprets the character format of the specified UTF-8 string. Attention: It is not checked whether a correct UTF-8 encoding is given. */
    std::string_view reinterpret_utf8(u8string_view u8s);
    /** Reinterprets the character format of the specified UTF-8 string. Attention: It is not checked whether a correct UTF-8 encoding is given. */
    u8string_view reinterpret_utf8(std::string_view s);

#if WCHAR_MAX == UINT16_MAX
    /** Reinterprets the character format of the specified UTF-16 string. Attention: It is not checked whether a correct UTF-16 encoding is given. */
    std::wstring_view reinterpret_utf16(u16string_view u16s);
    /** Reinterprets the character format of the specified UTF-16 string. Attention: It is not checked whether a correct UTF-16 encoding is given. */
    u16string_view reinterpret_utf16(std::wstring_view ws);
#endif
#if WCHAR_MAX == UINT32_MAX
    /** Reinterprets the character format of the specified UTF-32 string. Attention: It is not checked whether a correct UTF-32 encoding is given. */
    std::wstring_view reinterpret_utf32(u32string_view u32s);
    /** Reinterprets the character format of the specified UTF-32 string. Attention: It is not checked whether a correct UTF-32 encoding is given. */
    u32string_view reinterpret_utf32(std::wstring_view ws);
#endif

#if WCHAR_MAX == UINT16_MAX
    /** Reinterprets the character format of the specified UTF-16 (Windows) or UTF-32 (non-Windows) string. Attention: It is not checked whether a correct UTF-16/UTF-32 encoding is given. */
    std::wstring_view reinterpret_utfx(u16string_view u16s);
    /** Reinterprets the character format of the specified UTF-16 (Windows) or UTF-32 (non-Windows) string. Attention: It is not checked whether a correct UTF-16/UTF-32 encoding is given. */
    u16string_view reinterpret_utfx(std::wstring_view ws);
#endif
#if WCHAR_MAX == UINT32_MAX
    /** Reinterprets the character format of the specified UTF-16 (Windows) or UTF-32 (non-Windows) string. Attention: It is not checked whether a correct UTF-16/UTF-32 encoding is given. */
    std::wstring_view reinterpret_utfx(u32string_view u32s);
    /** Reinterprets the character format of the specified UTF-16 (Windows) or UTF-32 (non-Windows) string. Attention: It is not checked whether a correct UTF-16/UTF-32 encoding is given. */
    u32string_view reinterpret_utfx(std::wstring_view ws);
#endif

    Encoding detect_encoding(byte_view bys);



    /**
     * Checks whether the specified character is a whitespace character.
     */
    template <typename Tchar>
        requires(std::is_integral_v<Tchar>)
    bool is_whitespace(Tchar c)
    {
        // https://www.unicode.org/Public/UCD/latest/ucd/PropList.txt
        const auto ui32 = static_cast<uint32_t>(c);
        return 
            (ui32 >= 0x0009 && ui32 <= 0x000d) ||   // tab ... carriage return
            ui32 == 0x0020 ||                       // space
            ui32 == 0x0085 ||                       // next line
            ui32 == 0x00a0 ||                       // no-break space
            ui32 == 0x1680 ||                       // Ogham space mark
            (ui32 >= 0x2000 && ui32 <= 0x200a) ||   // en quad ... hair space
            ui32 == 0x2028 ||                       // line separator
            ui32 == 0x2029 ||                       // paragraph separator
            ui32 == 0x202f ||                       // narrow no-break space
            ui32 == 0x205f ||                       // medium mathematical space
            ui32 == 0x3000;                         // ideographic space

        static_assert(!std::is_same_v<Tchar, u8char_t>, "Not implemented for UTF-8 and its variable-length character encoding.");
    }

    /**
     * Checks whether the specified character is a whitespace character.
     */
    template <> inline
    bool is_whitespace(achar_t c)
    {
        const auto ui8 = static_cast<uint8_t>(c);
        return 
            (ui8 >= 0x09 && ui8 <= 0x0d) || // tab ... carriage return
            ui8 == 0x20/* ||                  // space
            ui8 == 0x85 ||                  // next line
            ui8 == 0xa0*/;                    // no-break space
        // Since ASCII only has 128 characters, 0x85 and 0xa0 should actually not be possible.
    }

    /**
     * Checks whether the specified Unicode code point is whitespace and, if yes,
     * returns the number of corresponding UTF-8 characters (code units).
     * This function enforces strict matching rules:
     * - For single-byte whitespace, c1 and c2 must be 0.
     * - For two-byte whitespace, c2 must be 0.
     */
    size_t is_whitespace_strict(u8char_t c0, u8char_t c1 = 0, u8char_t c2 = 0);

    /**
     * Checks whether the specified Unicode code point is whitespace and, if yes,
     * returns the number of corresponding UTF-8 characters (code units).
     * Unlike the strict version, this function does not require c1 or c2 to be 0
     * for shorter combinations.
     */
    size_t is_whitespace_relaxed(u8char_t c0, u8char_t c1 = 0, u8char_t c2 = 0);



    /**
     * Checks whether the specified character is a line break character (LF or CR).
     */
    template <typename Tchar>
        requires(std::is_integral_v<Tchar>)
    bool is_line_break(Tchar c)
    {
        return c == Tchar{'\n'} || c == Tchar{'\r'};
    }

    /**
     * Checks whether the specified character sequence represents a line break and,
     * if yes, returns the number of characters (code units) involved.
     * Possible combinations are (regardless of the current platform):
     * - CRLF (`\r\n`): Windows, DOS
     * - LF (`\n`): Unix, Linux, macOS, and "modern" line-ending styles in general
     * - CR (`\r`): Legacy Mac
     * This function enforces strict matching rules and requires c1 to be 0 for
     * single-character line breaks (LF or CR).
     */
    template <typename Tchar>
        requires(std::is_integral_v<Tchar>)
    size_t is_line_break_strict(Tchar c0, Tchar c1 = 0)
    {
        if (c0 == Tchar{'\r'} && c1 == Tchar{'\n'}) return 2;
        if (c0 == Tchar{'\n'} && c1 == 0) return 1;
        if (c0 == Tchar{'\r'} && c1 == 0) return 1;
        return 0;
    }

    /**
     * Checks whether the specified character sequence represents a line break and,
     * if yes, returns the number of characters (code units) involved.
     * Possible combinations are (regardless of the current platform):
     * - CRLF (`\r\n`): Windows, DOS
     * - LF (`\n`): Unix, Linux, macOS, and "modern" line-ending styles in general
     * - CR (`\r`): Legacy Mac
     * Unlike the strict version, this function does not require c1 to be 0 for
     * single-byte line breaks.
     */
    template <typename Tchar>
        requires(std::is_integral_v<Tchar>)
    size_t is_line_break_relaxed(Tchar c0, Tchar c1 = 0)
    {
        if (c0 == Tchar{'\r'})
            return c1 == Tchar{'\n'} ? 2 : 1;
        if (c0 == Tchar{'\n'}) return 1;
        return 0;
    }



    /**
     * Checks whether the specified character is a (decimal) digit: 0-9.
     */
    template <typename Tchar>
        requires(std::is_integral_v<Tchar>)
    bool is_digit(Tchar c)
    {
        return c >= Tchar{'0'} && c <= Tchar{'9'};
    }

    /**
     * Checks whether the specified character is a hexadecimal numeric character:
     * 0-9, A-F, a-f.
     */
    template <typename Tchar>
        requires(std::is_integral_v<Tchar>)
    bool is_hex_digit(Tchar c)
    {
        Tchar c_ = c | 0x20; // Force lowercase bit
        return (c >= Tchar{'0'} && c <= Tchar{'9'}) || (c_ >= Tchar{'a'} && c_ <= Tchar{'f'});
    }

    /**
     * Checks whether the specified character is an ASCII letter (either lowercase
     * or uppercase).
     */
    template <typename Tchar>
        requires(std::is_integral_v<Tchar>)
    bool is_ascii_letter(Tchar c)
    {
        c |= 0x20; // Force lowercase bit
        return c >= Tchar{'a'} && c <= Tchar{'z'};
    }

    /**
     * Checks whether the specified character is a lowercase ASCII letter.
     */
    template <typename Tchar>
        requires(std::is_integral_v<Tchar>)
    bool is_ascii_lower(Tchar c)
    {
        return c >= Tchar{'a'} && c <= Tchar{'z'};
    }

    /**
     * Checks whether the specified character is an uppercase ASCII letter.
     */
    template <typename Tchar>
        requires(std::is_integral_v<Tchar>)
    bool is_ascii_upper(Tchar c)
    {
        return c >= Tchar{'A'} && c <= Tchar{'Z'};
    }



    /**
     * Checks whether the specified UTF-8 string starts with whitespace and, if yes,
     * returns the number of UTF-8 characters (code units) of the corresponding code
     * point.
     */
    size_t is_whitespace_prefix(u8string_view s);

    /**
     * Checks whether the specified UTF-8 string ends with whitespace and, if yes,
     * returns the number of UTF-8 characters (code units) of the corresponding code
     * point.
     */
    size_t is_whitespace_suffix(u8string_view s);



    /**
     * Checks whether the specified string starts with a line break and, if yes,
     * returns the number of characters (code units) involved.
     * Possible combinations are (regardless of the current platform):
     * - CRLF (`\r\n`): Windows, DOS
     * - LF (`\n`): Unix, Linux, macOS, and "modern" line-ending styles in general
     * - CR (`\r`): Legacy Mac
     */
    template <class Tstring_or_view>
        requires(is_any_string_or_view_v<Tstring_or_view>)
    size_t is_line_break_prefix(Tstring_or_view&& s)
    {
        std::basic_string_view<typename std::remove_cvref_t<Tstring_or_view>::value_type> sv{std::forward<Tstring_or_view>(s)};
        const size_t n = sv.size();
        if (n >= 2) return is_line_break_relaxed(sv[0], sv[1]);
        if (n >= 1) return is_line_break_relaxed(sv[0]);
        return 0;
    }

    /**
     * Checks whether the specified string ends with a line break and, if yes,
     * returns the number of characters (code units) involved.
     * Possible combinations are (regardless of the current platform):
     * - CRLF (`\r\n`): Windows, DOS
     * - LF (`\n`): Unix, Linux, macOS, and "modern" line-ending styles in general
     * - CR (`\r`): Legacy Mac
     */
    template <class Tstring_or_view>
        requires(is_any_string_or_view_v<Tstring_or_view>)
    size_t is_line_break_suffix(Tstring_or_view&& s)
    {
        std::basic_string_view<typename std::remove_cvref_t<Tstring_or_view>::value_type> sv{std::forward<Tstring_or_view>(s)};
        const size_t n = sv.size();
        size_t k;
        // NOLINTBEGIN(bugprone-assignment-in-if-condition)
        if (n >= 1 && (k = is_line_break_strict(sv[n - 1])) > 0) return k;
        if (n >= 2 && (k = is_line_break_strict(sv[n - 2], sv[n - 1])) > 0) return k;
        // NOLINTEND(bugprone-assignment-in-if-condition)
        return 0;
    }



    /**
     * Counts and returns the number of whitespace characters that begin the string.
     */
    template <class Tstring_or_view>
        requires(is_any_string_or_view_v<Tstring_or_view>)
    size_t count_whitespace_prefix(Tstring_or_view&& s)
    {
        std::basic_string_view<typename std::remove_cvref_t<Tstring_or_view>::value_type> sv{std::forward<Tstring_or_view>(s)};
        const size_t n = sv.size();
        size_t i = 0;
        while (i < n && is_whitespace(sv[i]))
            ++i;
        return i;
    }

    /**
     * Counts and returns the number of whitespace characters that begin the string.
     * The number is calculated in terms of UTF-8 characters (code units), not in
     * terms of Unicode code points.
     */
    /*template <> */inline
    size_t count_whitespace_prefix(u8string_view s)
    {
        u8string_view sv{s};
        const size_t n = sv.size();
        size_t i = 0;
        size_t k;
        while (i < n && (k = is_whitespace_prefix(sv.substr(i))) > 0)
            i += k;
        return i;
    }

    /**
     * Counts and returns the number of whitespace characters that end the string.
     */
    template <class Tstring_or_view>
        requires(is_any_string_or_view_v<Tstring_or_view>)
    size_t count_whitespace_suffix(Tstring_or_view&& s)
    {
        std::basic_string_view<typename std::remove_cvref_t<Tstring_or_view>::value_type> sv{std::forward<Tstring_or_view>(s)};
        const size_t n = sv.size();
        size_t i = 0;
        while (i < n && is_whitespace(sv[n - i - 1]))
            ++i;
        return i;
    }

    /**
     * Counts and returns the number of whitespace characters that end the string.
     * The number is calculated in terms of UTF-8 characters (code units), not in
     * terms of Unicode code points.
     */
    /*template <> */inline
    size_t count_whitespace_suffix(u8string_view s)
    {
        u8string_view sv{s};
        const size_t n = sv.size();
        size_t i = 0;
        size_t k;
        while (i < n && (k = is_whitespace_suffix(sv.substr(0, n - i))) > 0)
            i += k;
        return i;
    }



    template <class Tstring>
        requires(is_any_string_v<Tstring> && !std::is_const_v<Tstring>)
    void ltrim(Tstring& s)
    {
        const size_t k = count_whitespace_prefix(make_string_view(s));
        assert(k <= s.size());
        s.erase(0, k);
    }

    template <class Tstring>
        requires(is_any_string_v<Tstring> && !std::is_const_v<Tstring>)
    void rtrim(Tstring& s)
    {
        const size_t k = count_whitespace_suffix(make_string_view(s));
        assert(k <= s.size());
        s.erase(s.size() - k, k);
    }

    template <class Tstring_view>
        requires(is_any_string_view_v<Tstring_view>)
    void ltrim(Tstring_view& s)
    {
        const size_t k = count_whitespace_prefix(Tstring_view(s));
        assert(k <= s.size());
        s = s.substr(k);
    }

    template <class Tstring_view>
        requires(is_any_string_view_v<Tstring_view>)
    void rtrim(Tstring_view& s)
    {
        const size_t k = count_whitespace_suffix(Tstring_view(s));
        assert(k <= s.size());
        s = s.substr(0, s.size() - k);
    }

    template <class Tstring_or_view>
        requires(is_any_string_or_view_v<Tstring_or_view>)
    void trim(Tstring_or_view& s)
    {
        rtrim(s);
        ltrim(s);
    }



    template <class Tstring_or_view>
        requires(is_any_string_or_view_v<Tstring_or_view>)
    Tstring_or_view ltrimmed(Tstring_or_view s)
    {
        ltrim(s);
        return s;
    }

    template <class Tstring_or_view>
        requires(is_any_string_or_view_v<Tstring_or_view>)
    Tstring_or_view rtrimmed(Tstring_or_view s)
    {
        rtrim(s);
        return s;
    }

    template <class Tstring_or_view>
        requires(is_any_string_or_view_v<Tstring_or_view>)
    Tstring_or_view trimmed(Tstring_or_view s)
    {
        trim(s);
        return s;
    }



    template <class Tstring, typename Tchar = typename Tstring::value_type>
        requires(is_any_string_v<Tstring> && !std::is_const_v<Tstring> && std::is_integral_v<Tchar>)
    void lpad(Tstring& s, size_t min_length, Tchar c)
    {
        if (min_length > s.length())
            s.insert(s.begin(), min_length - s.length(), c);
    }

    template <class Tstring, typename Tchar = typename Tstring::value_type>
        requires(is_any_string_v<Tstring> && !std::is_const_v<Tstring> && std::is_integral_v<Tchar>)
    void rpad(Tstring& s, size_t min_length, Tchar c)
    {
        if (min_length > s.length())
            s.append(min_length - s.length(), c);
    }



    template <class Tstring, typename Tchar = typename Tstring::value_type>
        requires(is_any_string_v<Tstring> && std::is_integral_v<Tchar>)
    Tstring lpadded(Tstring s, size_t min_length, Tchar c)
    {
        lpad(s, min_length, c);
        return s;
    }

    template <class Tstring, typename Tchar = typename Tstring::value_type>
        requires(is_any_string_v<Tstring> && std::is_integral_v<Tchar>)
    Tstring rpadded(Tstring s, size_t min_length, Tchar c)
    {
        rpad(s, min_length, c);
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



    template <class Tstring, typename Tval, typename Tchar = typename Tstring::value_type>
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
        rpad(s, pad_zeros, c0);
        std::reverse(s.begin(), s.end());
        return s;
    }

    template <class Tstring, typename Tval, typename Tchar = typename Tstring::value_type>
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

    template <class Tstring, typename Tval, typename Tchar = typename Tstring::value_type>
        requires(is_any_string_v<Tstring> && std::is_unsigned_v<Tval> && std::is_integral_v<Tchar>)
    Tstring to_hex_lc(Tval val, unsigned pad_zeros = 0)
    {
        return to_hex<Tstring>(val, pad_zeros, Tchar{'a'});
    }

    template <class Tstring, typename Tval, typename Tchar = typename Tstring::value_type>
        requires(is_any_string_v<Tstring> && std::is_unsigned_v<Tval> && std::is_integral_v<Tchar>)
    Tstring to_hex_uc(Tval val, unsigned pad_zeros = 0)
    {
        return to_hex<Tstring>(val, pad_zeros, Tchar{'A'});
    }

    template <class Tstring, typename Tval, typename Tchar = typename Tstring::value_type>
        requires(is_any_string_v<Tstring> && std::is_unsigned_v<Tval> && std::is_integral_v<Tchar>)
    Tstring to_0xhex_lc(Tval val, unsigned pad_zeros = 0)
    {
        return to_0xhex<Tstring>(val, pad_zeros, Tchar{'a'});
    }

    template <class Tstring, typename Tval, typename Tchar = typename Tstring::value_type>
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



    template <typename Tval = unsigned, class Tstring_or_view, typename Tchar = typename std::remove_cvref_t<Tstring_or_view>::value_type>
        requires(std::is_integral_v<Tval> && is_any_string_or_view_v<Tstring_or_view>)
    Tval parse_hex(Tstring_or_view&& s)
    {
        static constexpr auto c0 = Tchar{'0'};
        static constexpr auto cx = Tchar{'x'};
        std::basic_string_view<typename std::remove_cvref_t<Tstring_or_view>::value_type> sv{std::forward<Tstring_or_view>(s)};
        Tval val = 0;
        size_t p = count_whitespace_prefix(sv);
        if (p + 1 < sv.size() && sv[p] == c0 && sv[p + 1] == cx)
            p += 2;
        for (; p < sv.size() && is_hex_digit(sv[p]); ++p)
        {
            val *= 0x10;
            val += is_digit(sv[p])
                ? static_cast<Tval>(sv[p]) - 0x30 // '0' = 48 = 0x30
                : (static_cast<Tval>(sv[p]) | 0x20) - 0x57; // 'a' = 97 = 0x61; set off 10: 87 = 0x57
        }
        return val;
    }



    /**
     * Calculates the Levenshtein distance between two strings. The difference is
     * calculated in terms of characters (code units), not in terms of (Unicode)
     * code points.
     */
    template <class Tstring_or_view1, class Tstring_or_view2>
        requires(is_any_string_or_view_v<Tstring_or_view1> && is_any_string_or_view_v<Tstring_or_view2>)
    size_t levenshtein(Tstring_or_view1&& s1, Tstring_or_view2&& s2)
    {
        std::basic_string_view<typename std::remove_cvref_t<Tstring_or_view1>::value_type> sv1{std::forward<Tstring_or_view1>(s1)};
        std::basic_string_view<typename std::remove_cvref_t<Tstring_or_view2>::value_type> sv2{std::forward<Tstring_or_view2>(s2)};

        const size_t size1 = sv1.size();
        const size_t size2 = sv2.size();
        if (size1 == 0) return size2;
        if (size2 == 0) return size1;

        const size_t len = size2 + 1;
        const size_t total_size = (size1 + 1) * (size2 + 1);

        auto mat = std::make_unique_for_overwrite<size_t[]>(total_size);

        for (size_t i = 0; i <= size1; ++i)
            mat[i * len + 0] = i;
        for (size_t j = 0; j <= size2; ++j)
            mat[0 * len + j] = j;

        for (size_t i = 1; i <= size1; ++i)
        {
            for (size_t j = 1; j <= size2; ++j)
            {
                const size_t cost = sv1[i - 1] == sv2[j - 1] ? 0 : 1;
                mat[i * len + j] = (std::min)({
                    mat[(i - 1) * len + (j)] + 1,
                    mat[(i) * len + (j - 1)] + 1,
                    mat[(i - 1) * len + (j - 1)] + cost,
                });
            } // for ... j
        } // for ... i

        return mat[size1 * len + size2];
    }

    /**
     * Calculates a normalized Levenshtein distance between two strings on a single
     * scale from 0 ("identical") to 1 ("nothing in common"). The difference is
     * calculated in terms of characters (code units), not in terms of (Unicode)
     * code points.
     */
    template <typename Tfloat = float, class Tstring_or_view1, class Tstring_or_view2>
        requires(std::is_floating_point_v<Tfloat> && is_any_string_or_view_v<Tstring_or_view1> && is_any_string_or_view_v<Tstring_or_view2>)
    Tfloat levenshtein_normalized(Tstring_or_view1&& s1, Tstring_or_view2&& s2)
    {
        std::basic_string_view<typename std::remove_cvref_t<Tstring_or_view1>::value_type> sv1{std::forward<Tstring_or_view1>(s1)};
        std::basic_string_view<typename std::remove_cvref_t<Tstring_or_view2>::value_type> sv2{std::forward<Tstring_or_view2>(s2)};

        const size_t size = (std::max)(sv1.size(), sv2.size());
        if (size == 0) return 0.0f;
        return static_cast<Tfloat>(levenshtein(sv1, sv2)) / static_cast<Tfloat>(size);
    }



} // namespace cl7::strings

#endif // CL7_STRINGS_H

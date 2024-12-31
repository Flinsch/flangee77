#ifndef CL7_STRINGS_H
#define CL7_STRINGS_H

#include <CoreLabs/string.h>
#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>

#include <algorithm>
#include <bit>



/**
 * In addition to what I have already said (see string.h), let me say a few words
 * regarding the functions provided here:
 * 
 * Within the given framework we want to provide some basic functionality to deal
 * with different encodings and Unicode transformation formats and to convert
 * between them. We do not want to reinvent the wheel or to push it further than
 * necessary, so we really only limit ourselves to selected aspects that the
 * framework uses internally itself, without having to include external libraries.
 * If you need more extensive or robust functionality, you could/should include and
 * use 3rd party libraries in your projects yourself, like International Components
 * for Unicode (ICU) or Boost.Locale, for example.
 */



namespace cl7::strings {


    // NOLINTBEGIN(readability-enum-initial-value)
    enum class Encoding
    {
        Unknown,

        ASCII,
        UTF8,
        UTF16,
        UTF32,

#if WCHAR_MAX == UINT16_MAX
        UTFx = UTF16,
#elif WCHAR_MAX == UINT32_MAX
        UTFx = UTF32,
#endif

        Default = UTF8,
    };
    // NOLINTEND(readability-enum-initial-value)



    astring to_ascii(astring_view as);
    astring to_ascii(u8string_view u8s);
    astring to_ascii(u16string_view u16s);
    astring to_ascii(u32string_view u32s);
    astring to_ascii(wstring_view ws);

    u8string to_utf8(astring_view as);
    u8string to_utf8(u8string_view u8s);
    u8string to_utf8(u16string_view u16s);
    u8string to_utf8(u32string_view u32s);
    u8string to_utf8(wstring_view ws);

    u16string to_utf16(astring_view as);
    u16string to_utf16(u8string_view u8s);
    u16string to_utf16(u16string_view u16s);
    u16string to_utf16(u32string_view u32s);
    u16string to_utf16(wstring_view ws);

    u32string to_utf32(astring_view as);
    u32string to_utf32(u8string_view u8s);
    u32string to_utf32(u16string_view u16s);
    u32string to_utf32(u32string_view u32s);
    u32string to_utf32(wstring_view ws);

    wstring to_utfx(astring_view as);
    wstring to_utfx(u8string_view u8s);
    wstring to_utfx(u16string_view u16s);
    wstring to_utfx(u32string_view u32s);
    wstring to_utfx(wstring_view ws);

    astring to_ascii_unchecked(byte_view bys);
    u8string to_utf8_unchecked(byte_view bys);
    u16string to_utf16_unchecked(byte_view bys);
    u32string to_utf32_unchecked(byte_view bys);
    wstring to_utfx_unchecked(byte_view bys);

    byte_vector to_bytes(astring_view as);
    byte_vector to_bytes(u8string_view u8s, bool add_bom = false);
    byte_vector to_bytes(u16string_view u16s, bool add_bom = false, std::endian endian = std::endian::native);
    byte_vector to_bytes(u32string_view u32s, bool add_bom = false, std::endian endian = std::endian::native);
    byte_vector to_bytes(wstring_view ws, bool add_bom = false, std::endian endian = std::endian::native);

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

    Encoding detect_encoding(byte_view bys);



    template <typename Tchar>
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

        static_assert(!std::is_same_v<Tchar, u8char_type>, "Not implemented for UTF-8 and its variable-length character encoding.");
    }

    template <> inline
    bool is_whitespace(achar_type c)
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
     */
    size_t is_whitespace(u8char_type c0, u8char_type c1 = 0, u8char_type c2 = 0);

    /**
     * Checks whether the given UTF-8 string starts with whitespace and, if yes,
     * returns the number of UTF-8 characters (code units) of the corresponding code
     * point.
     */
    size_t is_whitespace_prefix(u8string_view s);

    /**
     * Checks whether the given UTF-8 string ends with whitespace and, if yes,
     * returns the number of UTF-8 characters (code units) of the corresponding code
     * point.
     */
    size_t is_whitespace_suffix(u8string_view s);



    /**
     * Counts and returns the number of whitespace characters that begin the string.
     */
    template <class Tstring_view>
        requires(is_any_string_view_v<Tstring_view>)
    size_t count_whitespace_prefix(Tstring_view s)
    {
        const size_t n = s.size();
        size_t i = 0;
        while (i < n && is_whitespace(s[i]))
            ++i;
        return i;
    }

    /**
     * Counts and returns the number of whitespace characters that begin the string.
     * The number is calculated in terms of UTF-8 characters (code units), not in
     * terms of Unicode code points.
     */
    template <> inline
    size_t count_whitespace_prefix(u8string_view s)
    {
        const size_t n = s.size();
        size_t i = 0;
        size_t k;
        while (i < n && (k = is_whitespace_prefix(s.substr(i))) > 0)
            i += k;
        return i;
    }

    /**
     * Counts and returns the number of whitespace characters that end the string.
     */
    template <class Tstring_view>
        requires(is_any_string_view_v<Tstring_view>)
    size_t count_whitespace_suffix(Tstring_view s)
    {
        const size_t n = s.size();
        size_t i = 0;
        while (i < n && is_whitespace(s[n - i - 1]))
            ++i;
        return i;
    }

    /**
     * Counts and returns the number of whitespace characters that end the string.
     * The number is calculated in terms of UTF-8 characters (code units), not in
     * terms of Unicode code points.
     */
    template <> inline
    size_t count_whitespace_suffix(u8string_view s)
    {
        const size_t n = s.size();
        size_t i = 0;
        size_t k;
        while (i < n && (k = is_whitespace_suffix(s.substr(0, n - i))) > 0)
            i += k;
        return i;
    }



    template <class Tstring>
        requires(is_any_string_v<Tstring>)
    void ltrim(Tstring& s)
    {
        const size_t k = count_whitespace_prefix(make_string_view(s));
        assert(k <= s.size());
        s.erase(0, k);
    }

    template <class Tstring>
        requires(is_any_string_v<Tstring>)
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
        const size_t k = count_whitespace_prefix(s);
        assert(k <= s.size());
        s = s.substr(k);
    }

    template <class Tstring_view>
        requires(is_any_string_view_v<Tstring_view>)
    void rtrim(Tstring_view& s)
    {
        const size_t k = count_whitespace_suffix(s);
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
        requires(is_any_string_v<Tstring>)
    void lpad(Tstring& s, size_t min_length, Tchar c)
    {
        if (min_length > s.length())
            s.insert(s.begin(), min_length - s.length(), c);
    }

    template <class Tstring, typename Tchar = typename Tstring::value_type>
        requires(is_any_string_v<Tstring>)
    void rpad(Tstring& s, size_t min_length, Tchar c)
    {
        if (min_length > s.length())
            s.append(min_length - s.length(), c);
    }



    template <class Tstring, typename Tchar = typename Tstring::value_type>
        requires(is_any_string_v<Tstring>)
    Tstring lpadded(Tstring s, size_t min_length, Tchar c)
    {
        lpad(s, min_length, c);
        return s;
    }

    template <class Tstring, typename Tchar = typename Tstring::value_type>
        requires(is_any_string_v<Tstring>)
    Tstring rpadded(Tstring s, size_t min_length, Tchar c)
    {
        rpad(s, min_length, c);
        return s;
    }



    template <class Tstring, typename Tval, typename Tchar = typename Tstring::value_type>
        requires(is_any_string_v<Tstring> && std::is_unsigned_v<Tval>)
    Tstring to_hex(Tval val, Tchar ca = Tchar('A'), unsigned pad_zeros = 0)
    {
        static constexpr auto c0 = Tchar('0');
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
        requires(is_any_string_v<Tstring> && std::is_unsigned_v<Tval>)
    Tstring to_0xhex(Tval val, Tchar ca = Tchar('A'), unsigned pad_zeros = 0)
    {
        static constexpr auto c0 = Tchar('0');
        static constexpr auto cx = Tchar('x');
        auto s = to_hex<Tstring>(val, ca, pad_zeros);
        s.insert(s.begin(), 2, c0);
        s[1] = cx;
        return s;
    }

    u8string to_hex(unsigned long long val, u8char_type ca = u8'A', unsigned pad_zeros = 0);
    u8string to_0xhex(unsigned long long val, u8char_type ca = u8'A', unsigned pad_zeros = 0);



    /**
     * Calculates the Levenshtein distance between two strings. The difference is
     * calculated in terms of characters (code units), not in terms of (Unicode)
     * code points.
     */
    template <class Tstring_or_view>
        requires(is_any_string_or_view_v<Tstring_or_view>)
    size_t levenshtein(const Tstring_or_view& s1, const Tstring_or_view& s2)
    {
        const size_t size1 = s1.size();
        const size_t size2 = s2.size();
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
                const size_t cost = s1[i - 1] == s2[j - 1] ? 0 : 1;
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
    template <class Tstring_or_view, typename Tfloat = float>
        requires(is_any_string_or_view_v<Tstring_or_view> && std::is_floating_point_v<Tfloat>)
    Tfloat levenshtein_normalized(const Tstring_or_view& s1, const Tstring_or_view& s2)
    {
        const size_t size = (std::max)(s1.size(), s2.size());
        if (size == 0) return 0.0f;
        return static_cast<Tfloat>(levenshtein(s1, s2)) / static_cast<Tfloat>(size);
    }



} // namespace cl7::strings

#endif // CL7_STRINGS_H

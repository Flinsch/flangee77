#ifndef CL7_TEXT_INSPECT_H
#define CL7_TEXT_INSPECT_H

#include <CoreLabs/string.h>



namespace cl7::text::inspect {



    /**
     * Checks whether the specified character is a whitespace character.
     */
    template <any_char Tchar>
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
    template <any_char Tchar>
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
    template <any_char Tchar>
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
    template <any_char Tchar>
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
    template <any_char Tchar>
    bool is_digit(Tchar c)
    {
        return c >= Tchar{'0'} && c <= Tchar{'9'};
    }

    /**
     * Checks whether the specified character is a hexadecimal numeric character:
     * 0-9, A-F, a-f.
     */
    template <any_char Tchar>
    bool is_hex_digit(Tchar c)
    {
        Tchar c_ = c | 0x20; // Force lowercase bit
        return (c >= Tchar{'0'} && c <= Tchar{'9'}) || (c_ >= Tchar{'a'} && c_ <= Tchar{'f'});
    }

    /**
     * Checks whether the specified character is an ASCII letter (either lowercase
     * or uppercase).
     */
    template <any_char Tchar>
    bool is_ascii_letter(Tchar c)
    {
        c |= 0x20; // Force lowercase bit
        return c >= Tchar{'a'} && c <= Tchar{'z'};
    }

    /**
     * Checks whether the specified character is a lowercase ASCII letter.
     */
    template <any_char Tchar>
    bool is_ascii_lower(Tchar c)
    {
        return c >= Tchar{'a'} && c <= Tchar{'z'};
    }

    /**
     * Checks whether the specified character is an uppercase ASCII letter.
     */
    template <any_char Tchar>
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
    template <any_string_or_view Tstring_or_view>
    size_t is_line_break_prefix(Tstring_or_view&& s)
    {
        auto sv = make_string_view(std::forward<Tstring_or_view>(s));
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
    template <any_string_or_view Tstring_or_view>
    size_t is_line_break_suffix(Tstring_or_view&& s)
    {
        auto sv = make_string_view(std::forward<Tstring_or_view>(s));
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
    template <any_string_or_view Tstring_or_view>
    size_t count_whitespace_prefix(Tstring_or_view&& s)
    {
        auto sv = make_string_view(std::forward<Tstring_or_view>(s));
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
    template <any_string_or_view Tstring_or_view>
    size_t count_whitespace_suffix(Tstring_or_view&& s)
    {
        auto sv = make_string_view(std::forward<Tstring_or_view>(s));
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



} // namespace cl7::text::inspect

#endif // CL7_TEXT_INSPECT_H

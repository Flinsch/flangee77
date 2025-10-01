#include "inspect.h"



namespace cl7::text::inspect {



    /**
     * Checks whether the specified Unicode code point is whitespace and, if yes,
     * returns the number of corresponding UTF-8 characters (code units).
     * This function enforces strict matching rules:
     * - For single-byte whitespace, c1 and c2 must be 0.
     * - For two-byte whitespace, c2 must be 0.
     */
    size_t is_whitespace_strict(u8char_t c0, u8char_t c1, u8char_t c2)
    {
        if (c1 == 0 && c2 == 0)
        {
            if ((c0 >= 0x09 && c0 <= 0x0d) || // tab ... carriage return
                c0 == 0x20)                   // space
                return 1;
        }

        if (c2 == 0)
        {
            if (c0 == 0xc2 && c1 == 0x85) return 2; // next line
            if (c0 == 0xc2 && c1 == 0xa0) return 2; // no-break space
        }

        if (c0 == 0xe1 && c1 == 0x9a && c2 == 0x80) return 3; // Ogham space mark

        if (c0 == 0xe2 && c1 == 0x80)
        {
            if (c2 >= 0x80 && c2 <= 0x8a) return 3; // en quad ... hair space
            if (c2 == 0xa8) return 3; // line separator
            if (c2 == 0xa9) return 3; // paragraph separator
            if (c2 == 0xaf) return 3; // narrow no-break space
        }

        if (c0 == 0xe2 && c1 == 0x81 && c2 == 0x9f) return 3; // medium mathematical space
        if (c0 == 0xe3 && c1 == 0x80 && c2 == 0x80) return 3; // ideographic space

        return 0;
    }

    /**
     * Checks whether the specified Unicode code point is whitespace and, if yes,
     * returns the number of corresponding UTF-8 characters (code units).
     * Unlike the strict version, this function does not require c1 or c2 to be 0
     * for shorter combinations.
     */
    size_t is_whitespace_relaxed(u8char_t c0, u8char_t c1, u8char_t c2)
    {
        if ((c0 >= 0x09 && c0 <= 0x0d) || // tab ... carriage return
            c0 == 0x20)                   // space
            return 1;

        if (c0 == 0xc2 && c1 == 0x85) return 2; // next line
        if (c0 == 0xc2 && c1 == 0xa0) return 2; // no-break space

        if (c0 == 0xe1 && c1 == 0x9a && c2 == 0x80) return 3; // Ogham space mark

        if (c0 == 0xe2 && c1 == 0x80)
        {
            if (c2 >= 0x80 && c2 <= 0x8a) return 3; // en quad ... hair space
            if (c2 == 0xa8) return 3; // line separator
            if (c2 == 0xa9) return 3; // paragraph separator
            if (c2 == 0xaf) return 3; // narrow no-break space
        }

        if (c0 == 0xe2 && c1 == 0x81 && c2 == 0x9f) return 3; // medium mathematical space
        if (c0 == 0xe3 && c1 == 0x80 && c2 == 0x80) return 3; // ideographic space

        return 0;
    }



    /**
     * Checks whether the specified UTF-8 string starts with whitespace and, if yes,
     * returns the number of UTF-8 characters (code units) of the corresponding code
     * point.
     */
    size_t is_whitespace_prefix(u8string_view s)
    {
        const size_t n = s.size();
        if (n >= 3) return is_whitespace_relaxed(s[0], s[1], s[2]);
        if (n >= 2) return is_whitespace_relaxed(s[0], s[1]);
        if (n >= 1) return is_whitespace_relaxed(s[0]);
        return 0;
    }

    /**
     * Checks whether the specified UTF-8 string ends with whitespace and, if yes,
     * returns the number of UTF-8 characters (code units) of the corresponding code
     * point.
     */
    size_t is_whitespace_suffix(u8string_view s)
    {
        const size_t n = s.size();
        size_t k;
        // NOLINTBEGIN(bugprone-assignment-in-if-condition)
        if (n >= 1 && (k = is_whitespace_strict(s[n - 1])) > 0) return k;
        if (n >= 2 && (k = is_whitespace_strict(s[n - 2], s[n - 1])) > 0) return k;
        if (n >= 3 && (k = is_whitespace_strict(s[n - 3], s[n - 2], s[n - 1])) > 0) return k;
        // NOLINTEND(bugprone-assignment-in-if-condition)
        return 0;
    }



} // namespace cl7::text::inspect

#include "codepoint.h"



namespace cl7::strings {



    /**
     * Checks whether this code point represents a valid ASCII character with a
     * value of up to 0x7F.
     */
    bool codepoint::is_valid_ascii() const
    {
        return value <= max_ascii;
    }

    /**
     * Checks whether this is a valid Unicode code point with a maximum value of
     * U+10FFFF and not within the surrogate range U+D800 through U+DFFF.
     */
    bool codepoint::is_valid_unicode() const
    {
        return value <= max_unicode && !is_surrogate();
    }

    /**
     * Checks whether this is a high-surrogate code point in Unicode, falling within
     * the range U+D800 through U+DBFF.
     */
    bool codepoint::is_high_surrogate() const
    {
        return value >= 0xd800 && value <= 0xdbff;
    }

    /**
     * Checks whether this is a low-surrogate code point in Unicode, falling within
     * the range U+DC00 through U+DFFF.
     */
    bool codepoint::is_low_surrogate() const
    {
        return value >= 0xdc00 && value <= 0xdfff;
    }

    /**
     * Checks whether this is a Unicode code point within the surrogate range U+D800
     * through U+DFFF.
     */
    bool codepoint::is_surrogate() const
    {
        return value >= 0xd800 && value <= 0xdfff;
    }



} // namespace cl7::strings

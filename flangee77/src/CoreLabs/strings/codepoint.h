#ifndef CL7_STRINGS_CODEPOINT_H
#define CL7_STRINGS_CODEPOINT_H

#include <CoreLabs/root.h>



namespace cl7::strings {



struct codepoint
{

    static constexpr unsigned max_ascii = 0x7f;
    static constexpr unsigned max_unicode = 0x10ffff;

    static constexpr unsigned replacement_ascii = 0x1a;
    static constexpr unsigned replacement_unicode = 0xfffd;



    unsigned value = 0;



    /**
     * Checks whether this code point represents a valid ASCII character with a
     * value of up to 0x7F.
     */
    bool is_valid_ascii() const;

    /**
     * Checks whether this is a valid Unicode code point with a maximum value of
     * U+10FFFF and not within the surrogate range U+D800 through U+DFFF.
     */
    bool is_valid_unicode() const;

    /**
     * Checks whether this is a high-surrogate code point in Unicode, falling within
     * the range U+D800 through U+DBFF.
     */
    bool is_high_surrogate() const;

    /**
     * Checks whether this is a low-surrogate code point in Unicode, falling within
     * the range U+DC00 through U+DFFF.
     */
    bool is_low_surrogate() const;

    /**
     * Checks whether this is a Unicode code point within the surrogate range U+D800
     * through U+DFFF.
     */
    bool is_surrogate() const;

}; // struct codepoint



} // namespace cl7::strings

#endif // CL7_STRINGS_CODEPOINT_H

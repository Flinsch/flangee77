#ifndef CL7_TEXT_CODEC_CODEPOINT_H
#define CL7_TEXT_CODEC_CODEPOINT_H

#include <CoreLabs/root.h>



namespace cl7::text::codec {



struct codepoint
{
    using value_type = unsigned;

    // At least 21 bits to cover all Unicode code points.
    // Expressed in bytes, that would be 3 (24 bits >= 21 bits).
    static_assert(sizeof(value_type) >= 3);

    static constexpr value_type max_ascii = 0x7f;
    static constexpr value_type max_unicode = 0x10ffff;

    static constexpr value_type high_surrogate_min = 0xd800;
    static constexpr value_type high_surrogate_max = 0xdbff;
    static constexpr value_type low_surrogate_min = 0xdc00;
    static constexpr value_type low_surrogate_max = 0xdfff;

    static constexpr value_type replacement_ascii = 0x1a;
    static constexpr value_type replacement_unicode = 0xfffd;



    value_type value = 0;



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



} // namespace cl7::text::codec

#endif // CL7_TEXT_CODEC_CODEPOINT_H

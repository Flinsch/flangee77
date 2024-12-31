#ifndef DL7_BASE64_H
#define DL7_BASE64_H

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>
#include <CoreLabs/string.h>



namespace dl7 {



class Base64
{

public:
    struct Options
    {
        /** The second last character used in the Base64 alphabet at position 62 (default: '+'). */
        cl7::u8char_type ch62;
        /** The very last character used in the Base64 alphabet at position 63 (default: '/'). */
        cl7::u8char_type ch63;
        /** The flag specifying whether the Base64 text is padded to always be a multiple of 4 characters (padding character: '='). */
        bool pad;
        /** The maximum line length (default: 76 characters). */
        size_t line_length;
        /** The flag specifying whether to insert line breaks if the maximum line length is reached (default: false). */
        bool insert_breaks;
        /** . */
        //bool insert_spaces;
    };

    static constexpr Options default_options = {
        .ch62=u8'+',
        .ch63=u8'/',
        .pad=true,
        .line_length=76,
        .insert_breaks=false,
        //.insert_spaces=false,
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Base64(const Options& options = default_options);



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Encodes the specified data as Base64.
     */
    cl7::u8string encode(cl7::byte_view data);

    /**
     * Decodes the specified Base64 text. If the text is invalid, an empty buffer is
     * returned.
     */
    cl7::byte_vector decode(cl7::u8string_view base64);



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the variant options.
     */
    const Options& get_options() const { return _options; }

    /**
     * Sets the variant options.
     */
    void set_options(const Options& options) { _options = options; }



    // #############################################################################
    // Static Functions
    // #############################################################################

    /**
     * Calculates the number of bytes required to store the decoded data of the
     * specified Base64 text. If the text has an invalid length, 0 is returned.
     * However, the function does not fully validate the data.
     */
    static size_t calculate_data_size(cl7::u8string_view base64);



private:

    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Returns the Base64 character of the given 6-bit character sextet.
     */
    cl7::u8char_type _encode(uint8_t sextet) const;

    /**
     * Checks the given character whether it is a valid Base64 character.
     * If it is valid, its 6-bit character sextet will be returned.
     * If it is invalid, an invalid value above 63 will be returned.
     */
    uint8_t _decode(cl7::u8char_type chr) const;

    /**
     * Extracts the first 6-bit Base64 character sextet from the first data byte.
     */
    static uint8_t _char1(uint8_t a);

    /**
     * Extracts the second 6-bit Base64 character sextet from the first two data bytes.
     */
    static uint8_t _char2(uint8_t a, uint8_t b);

    /**
     * Extracts the third 6-bit Base64 character sextet from the second and third data bytes.
     */
    static uint8_t _char3(uint8_t b, uint8_t c);

    /**
     * Extracts the fourth 6-bit Base64 character sextet from the third data byte.
     */
    static uint8_t _char4(uint8_t c);

    /**
     * Extracts the first data byte from the first and second 6-bit Base64 character sextets.
     */
    static uint8_t _byte1(uint8_t a, uint8_t b);

    /**
     * Extracts the second data byte from the second and third 6-bit Base64 character sextets.
     */
    static uint8_t _byte2(uint8_t b, uint8_t c);

    /**
     * Extracts the third data byte from the given third and fourth 6-bit Base64 character sextets.
     */
    static uint8_t _byte3(uint8_t c, uint8_t d);



    // #############################################################################
    // Attributes
    // #############################################################################

    static bool _initialized;
    static cl7::u8char_type _base64_char_table[64];
    static uint8_t _base64_sextet_lookup[128];

    /**
     * The variant options.
     */
    Options _options;

}; // class Base64



} // namespace dl7

#endif // DL7_BASE64_H

#pragma once
#ifndef CL7_BASE64_H
#define CL7_BASE64_H

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>
#include <CoreLabs/string.h>



namespace cl7 {



class Base64
{

public:
    struct Options
    {
        /** The second last character used in the Base64 alphabet at position 62 (default: '+'). */
        char ch62;
        /** The very last character used in the Base64 alphabet at position 63 (default: '/'). */
        char ch63;
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
        '+',    // ch62
        '/',    // ch63
        true,   // pad
        76,     // line_length
        false,  // insert_breaks
        //false,  // insert_spaces
    };

private:
    static bool _initialized;
    static char _base64_char_table[ 64 ];
    static uint8_t _base64_sextet_lookup[ 128 ];



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default/explicit constructor.
     */
    Base64(const Options& options = default_options);

    /**
     * Destructor.
     */
    ~Base64() = default;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The variant options.
     */
    Options _options;



    // #############################################################################
    // Properties
    // #############################################################################
private:
    /**
     * Returns the variant options.
     */
    const Options& get_options() const { return _options; }

    /**
     * Sets the variant options.
     */
    void set_options(const Options& options) { _options = options; }



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Encodes the specified data as Base64.
     */
    cl7::astring encode(cl7::byte_view data);

    /**
     * Decodes the specified Base64 text. If the text is invalid, an empty buffer is
     * returned.
     */
    cl7::byte_vector decode(cl7::astring_view base64);

    /**
     * Calculates the number of bytes required to store the decoded data of the
     * specified Base64 text. If the text has an invalid length, 0 is returned.
     * However, the function does not fully validate the data.
     */
    size_t calculate_data_size(cl7::astring_view base64);



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Extracts the first 6-bit Base64 character sextet from the first data byte.
     */
    uint8_t _char1(uint8_t a) const;

    /**
     * Extracts the second 6-bit Base64 character sextet from the first two data bytes.
     */
    uint8_t _char2(uint8_t a, uint8_t b) const;

    /**
     * Extracts the third 6-bit Base64 character sextet from the second and third data bytes.
     */
    uint8_t _char3(uint8_t b, uint8_t c) const;

    /**
     * Extracts the fourth 6-bit Base64 character sextet from the third data byte.
     */
    uint8_t _char4(uint8_t c) const;

    /**
     * Extracts the first data byte from the first and second 6-bit Base64 character sextets.
     */
    uint8_t _byte1(uint8_t a, uint8_t b) const;

    /**
     * Extracts the second data byte from the second and third 6-bit Base64 character sextets.
     */
    uint8_t _byte2(uint8_t b, uint8_t c) const;

    /**
     * Extracts the third data byte from the given third and fourth 6-bit Base64 character sextets.
     */
    uint8_t _byte3(uint8_t c, uint8_t d) const;

    /**
     * Returns the Base64 character of the given 6-bit character sextet.
     */
    cl7::achar_type _encode(uint8_t sextet) const;

    /**
     * Checks the given character whether it is a valid Base64 character.
     * If it is valid, its 6-bit character sextet will be returned.
     * If it is invalid, an invalid value above 63 will be returned.
     */
    uint8_t _decode(cl7::achar_type chr) const;

}; // class Base64



} // namespace cl7

#endif // CL7_BASE64_H

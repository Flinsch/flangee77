#pragma once
#ifndef F77_TESTS_CL7_STRINGS_H
#define F77_TESTS_CL7_STRINGS_H

#include <CoreLabs/strings.h>

#include <TestLabs/TestSuite.h>

#include <vector>



namespace cl7 {
    template <> inline
    cl7::string to_string(const byte_vector& bytes)
    {
        cl7::string s( bytes.size() * 4 + 2, TEXT(' ') );
        s[ 0 ] = TEXT('{');
        for ( size_t i = 0; i < bytes.size(); ++i )
        {
            const cl7::char_type byte = static_cast<cl7::char_type>( bytes[ i ] ) & 0xff;
            const cl7::char_type lo = byte & 0xf;
            const cl7::char_type hi = (byte >> 4) & 0xf;
            s[ i * 4 + 1 ] = TEXT('\\');
            s[ i * 4 + 2 ] = TEXT('x');
            s[ i * 4 + 3 ] = hi < 10 ? TEXT('0') + hi : TEXT('a') + hi - 10;
            s[ i * 4 + 4 ] = lo < 10 ? TEXT('0') + lo : TEXT('a') + lo - 10;
        }
        s[ bytes.size() * 4 + 1 ] = TEXT('}');
        return s;
    }
}



#define CoreLabs_strings_to_ASCII_Latin1_from_UTFx(from, type, prefix) \
TESTLABS_CASE( TEXT("CoreLabs:  strings:  to ASCII/Latin-1 from " from) ) \
{ \
    struct Entry \
    { \
        type input; \
        cl7::astring_view expected; \
        bool check_ascii; \
    } entry; \
 \
    const std::vector<Entry> container { \
        { prefix"pure ASCII", "pure ASCII", true }, \
        { prefix"ß", "ß", false }, /* valid Latin-1 character, but invalid ASCII character */ \
        { prefix"\u0100", "\x1a", true }, /* invalid Latin-1 character, but valid replacement character (even ASCII) */ \
        { prefix"aou äöü \ud7ff\ue000 123", "aou äöü \x1a\x1a 123", false }, /* just some valid and invalid code points */ \
        { prefix"\u0080", "\x80", false }, /* lowest non-ASCII character */ \
        { prefix"\u00ff", "\xff", false }, /* highest Latin-1 character */ \
    }; \
 \
    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( TEXT("from " from), container, entry, entry.input ) \
    { \
        auto actual = cl7::strings::to_ascii( entry.input ); \
        TESTLABS_CHECK_EQ( actual, entry.expected ); \
        TESTLABS_CHECK_EQ( cl7::strings::check_ascii( actual ), entry.check_ascii ); \
    } \
}

CoreLabs_strings_to_ASCII_Latin1_from_UTFx( "UTF-8", cl7::u8string_view, u8 );
CoreLabs_strings_to_ASCII_Latin1_from_UTFx( "UTF-16", cl7::u16string_view, u );
CoreLabs_strings_to_ASCII_Latin1_from_UTFx( "UTF-32", cl7::u32string_view, U );



TESTLABS_CASE( TEXT("CoreLabs:  strings:  to UTF-8 from ASCII/Latin-1") )
{
    struct Entry
    {
        cl7::astring_view input;
        cl7::u8string_view expected;
        size_t byte_count;
    } entry;

    const std::vector<Entry> container {
        { "pure ASCII", u8"pure ASCII", 10 }, // 1 byte per character
        { "\xdf", u8"ß", 2 }, // 2-byte sequence
        { "aou äöü \x1a\x1a 123", u8"aou äöü \x1a\x1a 123", 17 }, /* just some "random" characters */ \
        { "\x80", u8"\u0080", 2 }, /* lowest non-ASCII character */ \
        { "\xff", u8"\u00ff", 2 }, /* highest Latin-1 character */ \
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( TEXT("to UTF-8"), container, entry, entry.input )
    {
        auto actual = cl7::strings::to_utf8( entry.input );
        TESTLABS_CHECK_EQ( actual, entry.expected );
        TESTLABS_CHECK_EQ( actual.size(), entry.byte_count );
    }
}

#define CoreLabs_strings_to_UTFx_from_ASCII_Latin1(to, type, prefix, to_utfx) \
TESTLABS_CASE( TEXT("CoreLabs:  strings:  to " to " from ASCII/Latin-1") ) \
{ \
    struct Entry \
    { \
        cl7::astring_view input; \
        type expected; \
    } entry; \
 \
    const std::vector<Entry> container { \
        { "pure ASCII", prefix"pure ASCII" }, \
        { "\xdf", prefix"ß" }, \
        { "\x80", prefix"\u0080" }, /* lowest non-ASCII character */ \
        { "\xff", prefix"\u00ff" }, /* highest Latin-1 character */ \
    }; \
 \
    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( TEXT("to " to), container, entry, entry.input ) \
    { \
        auto actual = cl7::strings::to_utfx( entry.input ); \
        TESTLABS_CHECK_EQ( actual, entry.expected ); \
    } \
}

CoreLabs_strings_to_UTFx_from_ASCII_Latin1( "UTF-16", cl7::u16string_view, u, to_utf16 );
CoreLabs_strings_to_UTFx_from_ASCII_Latin1( "UTF-32", cl7::u32string_view, U, to_utf32 );



#define CoreLabs_strings_to_UTF8_from_UTFx(from, type, prefix) \
TESTLABS_CASE( TEXT("CoreLabs:  strings:  to UTF-8 from " from) ) \
{ \
    struct Entry \
    { \
        type input; \
        cl7::u8string_view expected; \
        size_t byte_count; \
    } entry; \
 \
    const std::vector<Entry> container { \
        { prefix"pure ASCII", u8"pure ASCII", 10 }, /* 1 byte per character */ \
        { prefix"\u0080", u8"\u0080", 2 }, /* "first" 2-byte sequence */ \
        { prefix"\u07ff", u8"\u07ff", 2 }, /* "last" 2-byte sequence */ \
        { prefix"\u0800", u8"\u0800", 3 }, /* "first" 3-byte sequence */ \
        { prefix"\uffff", u8"\uffff", 3 }, /* "last" 3-byte sequence */ \
        { prefix"\U00010000", u8"\U00010000", 4 }, /* "first" 3-byte sequence */ \
        { prefix"\U0010ffff", u8"\U0010ffff", 4 }, /* "last" 3-byte sequence */ \
    }; \
 \
    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( TEXT("from " from), container, entry, entry.input ) \
    { \
        auto actual = cl7::strings::to_utf8( entry.input ); \
        TESTLABS_CHECK_EQ( actual, entry.expected ); \
        TESTLABS_CHECK_EQ( actual.size(), entry.byte_count ); \
    } \
}

CoreLabs_strings_to_UTF8_from_UTFx( "UTF-16", cl7::u16string_view, u );
CoreLabs_strings_to_UTF8_from_UTFx( "UTF-32", cl7::u32string_view, U );



#define CoreLabs_strings_to_UTFx_from_UTF8(to, type, prefix, to_utfx) \
TESTLABS_CASE( TEXT("CoreLabs:  strings:  to " to " from UTF-8") ) \
{ \
    struct Entry \
    { \
        cl7::u8string_view input; \
        type expected; \
    } entry; \
 \
    const std::vector<Entry> container { \
        { u8"pure ASCII", prefix"pure ASCII" }, /* 1 byte per character */ \
        { u8"\u0080", prefix"\u0080" }, /* "first" 2-byte sequence */ \
        { u8"\u07ff", prefix"\u07ff" }, /* "last" 2-byte sequence */ \
        { u8"\u0800", prefix"\u0800" }, /* "first" 3-byte sequence */ \
        { u8"\uffff", prefix"\uffff" }, /* "last" 3-byte sequence */ \
        { u8"\U00010000", prefix"\U00010000" }, /* "first" 4-byte sequence */ \
        { u8"\U0010ffff", prefix"\U0010ffff" }, /* "last" 4-byte sequence */ \
    }; \
 \
    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( TEXT("to " to), container, entry, entry.input ) \
    { \
        auto actual = cl7::strings::to_utfx( entry.input ); \
        TESTLABS_CHECK_EQ( actual, entry.expected ); \
    } \
}

CoreLabs_strings_to_UTFx_from_UTF8( "UTF-16", cl7::u16string_view, u, to_utf16 );
CoreLabs_strings_to_UTFx_from_UTF8( "UTF-32", cl7::u32string_view, U, to_utf32 );



TESTLABS_CASE( TEXT("CoreLabs:  strings:  between UTF-16 and UTF-32") )
{
    struct Entry
    {
        std::vector<unsigned short> u16d;
        cl7::u32string_view u32s;
        cl7::astring comment;
    } entry;

    const std::vector<Entry> container {
        { { 0x70, 0x75, 0x72, 0x65, 0x20, 0x41, 0x53, 0x43, 0x49, 0x49 }, U"pure ASCII", "pure ASCII" },
        { { 0xd7ff }, U"\ud7ff", "u+df77" },
        { { 0xe000 }, U"\ue000", "u+e000" },
        { { 0xffff }, U"\uffff", "u+ffff" },
        { { 0xd800, 0xdc00 }, U"\U00010000", "u+d800 u+dc00  <=>  u+00010000" },
        { { 0xd800, 0xdc01 }, U"\U00010001", "u+d800 u+dc01  <=>  u+00010001" },
        { { 0xd800, 0xdfff }, U"\U000103ff", "u+d800 u+dfff  <=>  u+000103ff" },
        { { 0xd801, 0xdc00 }, U"\U00010400", "u+d801 u+dc00  <=>  u+00010400" },
        { { 0xd801, 0xdc01 }, U"\U00010401", "u+d801 u+dc01  <=>  u+00010401" },
        { { 0xd801, 0xdfff }, U"\U000107ff", "u+d801 u+dfff  <=>  u+000107ff" },
        { { 0xdbff, 0xdc00 }, U"\U0010fc00", "u+dbff u+dc00  <=>  u+0010fc00" },
        { { 0xdbff, 0xdc01 }, U"\U0010fc01", "u+dbff u+dc01  <=>  u+0010fc01" },
        { { 0xdbff, 0xdfff }, U"\U0010ffff", "u+dbff u+dfff  <=>  u+0010ffff" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( TEXT("between UTF-16 and UTF-32"), container, entry, entry.comment )
    {
        cl7::u16string entry_u16s( entry.u16d.size(), u' ' );
        for ( size_t i = 0; i < entry.u16d.size(); ++i )
            entry_u16s[ i ] = static_cast<cl7::u16char_type>( entry.u16d[ i ] );

        auto actual_u16s = cl7::strings::to_utf16( entry.u32s );
        auto actual_u32s = cl7::strings::to_utf32( entry_u16s );

        const auto& expected_u16s = entry_u16s;
        const auto& expected_u32s = entry.u32s;

        TESTLABS_CHECK_EQ( actual_u16s, expected_u16s );
        TESTLABS_CHECK_EQ( actual_u32s, expected_u32s );
    }
}



// Functions to be tested explicitly:
// 
// astring to_latin1(const u32string_view& u32s)
// u8string to_utf8(const u32string_view& u32s)
// u16string to_utf16(const u32string_view& u32s)
// 
// astring to_latin1(const byte_span& bys)
// u8string to_utf8_unchecked(const byte_span& bys)
// u16string to_utf16_unchecked(const byte_span& bys)
// u32string to_utf32_unchecked(const byte_span& bys)
// 
// byte_vector to_bytes(const astring_view& as)
// byte_vector to_bytes(const u8string_view& u8s, bool add_bom)
// byte_vector to_bytes(const u16string_view& u16s, bool add_bom, std::endian endian)
// byte_vector to_bytes(const u32string_view& u32s, bool add_bom, std::endian endian)
// 
// bool check_ascii(const astring_view& as, bool log_warning)
// bool parse_utf8(const u8string_view& u8s, u32string& u32s, bool log_warning)
// bool parse_utf16(const u16string_view& u16s, u32string& u32s, bool log_warning)
// bool check_utf32(const u32string_view& u32s, bool log_warning)
// 
// size_t utf8_length(const u8string_view& u8s)
// size_t utf16_length(const u16string_view& u16s)



TESTLABS_CASE( TEXT("CoreLabs:  strings::to_latin1(const u32string_view&)") )
{
    for ( cl7::u32char_type u32c = 0; u32c <= 0xff; ++u32c )
    {
        const cl7::achar_type ac = static_cast<cl7::achar_type>( u32c );
        const cl7::astring as( 1, ac );
        const cl7::u32string u32s( 1, u32c );
        TESTLABS_CHECK_EQ( cl7::strings::to_latin1( u32s ), as );
    }

    for ( cl7::u32char_type u32c = 0x0100; ; u32c *= 2 )
    {
        const cl7::achar_type ac = 0x1a;
        const cl7::astring as( 1, ac );
        const cl7::u32string u32s( 1, u32c );
        TESTLABS_CHECK_EQ( cl7::strings::to_latin1( u32s ), as );

        if ( u32c > 0x10ffff ) // Deliberately include also an invalid code point (above 0x10ffff).
            break;
    }
}

TESTLABS_CASE( TEXT("CoreLabs:  strings::to_utf8(const u32string_view&)") )
{
    struct Entry
    {
        std::vector<unsigned long> u32d;
        std::vector<unsigned char> u8d;
        cl7::astring comment;
    } entry;

    const std::vector<Entry> container {
        { { 0 }, { 0 }, "" },
        { { 0x53, 0x74, 0x65, 0x66, 0x61, 0x6e, 0x20, 0x46, 0x6c, 0x65, 0x69, 0x73, 0x63, 0x68, 0x65, 0x72, 0 }, { 0x53, 0x74, 0x65, 0x66, 0x61, 0x6e, 0x20, 0x46, 0x6c, 0x65, 0x69, 0x73, 0x63, 0x68, 0x65, 0x72, 0 }, "Stefan Fleischer" },
        { { 0x47, 0xe4, 0x6e, 0x73, 0x65, 0x66, 0xfc, 0xdf, 0x63, 0x68, 0x65, 0x6e, 0 }, { 0x47, 0xc3,0xa4, 0x6e, 0x73, 0x65, 0x66, 0xc3,0xbc, 0xc3,0x9f, 0x63, 0x68, 0x65, 0x6e, 0 }, "Gänsefüßchen" },
        { { 0x20, 0 }, { 0x20, 0 }, "u+0020  =>  \\x20" },
        { { 0x7f, 0 }, { 0x7f, 0 }, "u+007f  =>  \\x7f" },
        { { 0x80, 0 }, { 0xc2,0x80, 0 }, "u+0080  =>  \\xc2\\x80" },
        { { 0xff, 0 }, { 0xc3,0xbf, 0 }, "u+00ff  =>  \\xc3\\xbf" },
        { { 0x07ff, 0 }, { 0xdf,0xbf, 0 }, "u+07ff  =>  \\xdf\\xbf" },
        { { 0x0800, 0 }, { 0xe0,0xa0,0x80, 0 }, "u+0800  =>  \\xe0\\xa0\\x80" },
        { { 0xd7ff, 0 }, { 0xed,0x9f,0xbf, 0 }, "u+d7ff  =>  \\xed\\x9f\\xbf" },
        { { 0xd800, 0 }, { 0xed,0xa0,0x80, 0 }, "u+d800  =>  \\xed\\xa0\\x80  (invalid code point, but codeable)" },
        { { 0xdbff, 0 }, { 0xed,0xaf,0xbf, 0 }, "u+dbff  =>  \\xed\\xaf\\xbf  (invalid code point, but codeable)" },
        { { 0xdc00, 0 }, { 0xed,0xb0,0x80, 0 }, "u+dc00  =>  \\xed\\xb0\\x80  (invalid code point, but codeable)" },
        { { 0xdfff, 0 }, { 0xed,0xbf,0xbf, 0 }, "u+dfff  =>  \\xed\\xbf\\xbf  (invalid code point, but codeable)" },
        { { 0xe000, 0 }, { 0xee,0x80,0x80, 0 }, "u+e000  =>  \\xee\\x80\\x80" },
        { { 0xffff, 0 }, { 0xef,0xbf,0xbf, 0 }, "u+ffff  =>  \\xef\\xbf\\xbf" },
        { { 0x0001'0000, 0 }, { 0xf0,0x90,0x80,0x80, 0 }, "u+00010000  =>  \\xf0\\x90\\x80\\x80" },
        { { 0x0010'ffff, 0 }, { 0xf4,0x8f,0xbf,0xbf, 0 }, "u+0010ffff  =>  \\xf4\\x8f\\xbf\\xbf" },
        { { 0x0011'0000, 0 }, { 0xef,0xbf,0xbd, 0 }, "u+00110000  =>  \\xef\\xbf\\xbd  (invalid code point)" },
        { { 0x20, 0x7f, 0x80, 0xff, 0x07ff, 0x0800, 0xd7ff, 0xd800, 0xdbff, 0xdc00, 0xdfff, 0xe000, 0xffff, 0x0001'0000, 0x0010'ffff, 0x0011'0000, 0 }, { 0x20, 0x7f, 0xc2,0x80, 0xc3,0xbf, 0xdf,0xbf, 0xe0,0xa0,0x80, 0xed,0x9f,0xbf, 0xed,0xa0,0x80, 0xed,0xaf,0xbf, 0xed,0xb0,0x80, 0xed,0xbf,0xbf, 0xee,0x80,0x80, 0xef,0xbf,0xbf, 0xf0,0x90,0x80,0x80, 0xf4,0x8f,0xbf,0xbf, 0xef,0xbf,0xbd, 0 }, "..." },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( TEXT(""), container, entry, entry.comment )
    {
        TESTLABS_ASSERT( entry.u32d.size() >= 1 && entry.u32d.back() == 0 );
        TESTLABS_ASSERT( entry.u8d.size() >= 1 && entry.u8d.back() == 0 );

        const cl7::u32string_view u32s{ reinterpret_cast<const cl7::u32char_type*>( &entry.u32d[0] ) };
        const cl7::u8string_view u8s{ reinterpret_cast<const cl7::u8char_type*>( &entry.u8d[0] ) };

        TESTLABS_CHECK_EQ( cl7::strings::to_utf8( u32s ), u8s );
    }
}

TESTLABS_CASE( TEXT("CoreLabs:  strings::to_utf16(const u32string_view&)") )
{
    struct Entry
    {
        std::vector<unsigned long> u32d;
        std::vector<unsigned short> u16d;
        cl7::astring comment;
    } entry;

    const std::vector<Entry> container {
        { { 0 }, { 0 }, "" },
        { { 0x53, 0x74, 0x65, 0x66, 0x61, 0x6e, 0x20, 0x46, 0x6c, 0x65, 0x69, 0x73, 0x63, 0x68, 0x65, 0x72, 0 }, { 0x53, 0x74, 0x65, 0x66, 0x61, 0x6e, 0x20, 0x46, 0x6c, 0x65, 0x69, 0x73, 0x63, 0x68, 0x65, 0x72, 0 }, "Stefan Fleischer" },
        { { 0x47, 0xe4, 0x6e, 0x73, 0x65, 0x66, 0xfc, 0xdf, 0x63, 0x68, 0x65, 0x6e, 0 }, { 0x47, 0xe4, 0x6e, 0x73, 0x65, 0x66, 0xfc, 0xdf, 0x63, 0x68, 0x65, 0x6e, 0 }, "Gänsefüßchen" },
        { { 0x20, 0 }, { 0x20, 0 }, "u+0020  =>  u+0020" },
        { { 0x7f, 0 }, { 0x7f, 0 }, "u+007f  =>  u+007f" },
        { { 0x80, 0 }, { 0x80, 0 }, "u+0080  =>  u+0080" },
        { { 0xff, 0 }, { 0xff, 0 }, "u+00ff  =>  u+00ff" },
        { { 0x07ff, 0 }, { 0x07ff, 0 }, "u+07ff  =>  u+07ff" },
        { { 0x0800, 0 }, { 0x0800, 0 }, "u+0800  =>  u+0800" },
        { { 0xd7ff, 0 }, { 0xd7ff, 0 }, "u+d7ff  =>  u+d7ff" },
        { { 0xd800, 0 }, { 0xd800, 0 }, "u+d800  =>  u+d800  (invalid code point, but codeable)" },
        { { 0xdbff, 0 }, { 0xdbff, 0 }, "u+dbff  =>  u+dbff  (invalid code point, but codeable)" },
        { { 0xdc00, 0 }, { 0xdc00, 0 }, "u+dc00  =>  u+dc00  (invalid code point, but codeable)" },
        { { 0xdfff, 0 }, { 0xdfff, 0 }, "u+dfff  =>  u+dfff  (invalid code point, but codeable)" },
        { { 0xe000, 0 }, { 0xe000, 0 }, "u+e000  =>  u+e000" },
        { { 0xffff, 0 }, { 0xffff, 0 }, "u+ffff  =>  u+ffff" },
        { { 0x0001'0000, 0 }, { 0xd800,0xdc00, 0 }, "u+00010000  =>  u+d800 u+dc00" },
        { { 0x0010'ffff, 0 }, { 0xdbff,0xdfff, 0 }, "u+0010ffff  =>  u+dbff u+dfff" },
        { { 0x0011'0000, 0 }, { 0xfffd, 0 }, "u+00110000  =>  u+fffd  (invalid code point)" },
        { { 0x20, 0x7f, 0x80, 0xff, 0x07ff, 0x0800, 0xd7ff, 0xd800, 0xdbff, 0xdc00, 0xdfff, 0xe000, 0xffff, 0x0001'0000, 0x0010'ffff, 0x0011'0000, 0 }, { 0x20, 0x7f, 0x80, 0xff, 0x07ff, 0x0800, 0xd7ff, 0xd800, 0xdbff, 0xdc00, 0xdfff, 0xe000, 0xffff, 0xd800,0xdc00, 0xdbff,0xdfff, 0xfffd, 0 }, "..." },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( TEXT(""), container, entry, entry.comment )
    {
        TESTLABS_ASSERT( entry.u32d.size() >= 1 && entry.u32d.back() == 0 );
        TESTLABS_ASSERT( entry.u16d.size() >= 1 && entry.u16d.back() == 0 );

        const cl7::u32string_view u32s{ reinterpret_cast<const cl7::u32char_type*>( &entry.u32d[0] ) };
        const cl7::u16string_view u16s{ reinterpret_cast<const cl7::u16char_type*>( &entry.u16d[0] ) };

        TESTLABS_CHECK_EQ( cl7::strings::to_utf16( u32s ), u16s );
    }
}



TESTLABS_CASE( TEXT("CoreLabs:  strings::to_latin1(const byte_span&)") )
{
    struct Entry
    {
        cl7::byte_vector input;
        cl7::astring_view expected;
        bool check_ascii;
    } entry;

    const std::vector<Entry> container {
        { cl7::make_bytes( 0x70, 0x75, 0x72, 0x65, 0x20, 0x41, 0x53, 0x43, 0x49, 0x49 ), "pure ASCII", true },
        { cl7::make_bytes( 0xdf ), "ß", false }, /* valid Latin-1 character, but invalid ASCII character */
        { cl7::make_bytes( 0x61,0x6f,0x75, 0x20, 0xe4,0xf6,0xfc, 0x20, 0x1a,0x1a, 0x20, 0x31,0x32,0x33 ), "aou äöü \x1a\x1a 123", false }, /* just some "random" characters */
        { cl7::make_bytes( 0x80 ), "\x80", false }, /* lowest non-ASCII character */
        { cl7::make_bytes( 0xff ), "\xff", false }, /* highest Latin-1 character */
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( TEXT(""), container, entry, entry.expected )
    {
        TESTLABS_CHECK_EQ( cl7::strings::to_latin1( entry.input ), entry.expected );
    }
}

TESTLABS_CASE( TEXT("CoreLabs:  strings::to_utf8_unchecked(const byte_span&)") )
{
    struct Entry
    {
        cl7::byte_vector input;
        cl7::u8string_view expected;
        size_t byte_count;
    } entry;

    const std::vector<Entry> container {
        { cl7::make_bytes( 0x70, 0x75, 0x72, 0x65, 0x20, 0x41, 0x53, 0x43, 0x49, 0x49 ), u8"pure ASCII", 10 },
        { cl7::make_bytes( 0xc3,0x9f ), u8"ß", 2 }, /* valid Latin-1 character, but invalid ASCII character */
        { cl7::make_bytes( 0x61,0x6f,0x75, 0x20, 0xc3,0xa4,0xc3,0xb6,0xc3,0xbc, 0x20, 0x1a,0x1a, 0x20, 0x31,0x32,0x33 ), u8"aou äöü \x1a\x1a 123", 17 }, /* just some "random" characters */
        { cl7::make_bytes( 0xc2,0x80 ), u8"\u0080", 2 }, /* lowest non-ASCII character */
        { cl7::make_bytes( 0xc3,0xbf ), u8"\u00ff", 2 }, /* highest Latin-1 character */
        { cl7::make_bytes( 0x80,0xff,0xc2,0xc3 ), (const cl7::u8char_type*)"\x80\xff\xc2\xc3", 4 }, /* Invalid character sequence (should stay the same) */
        { cl7::make_bytes( 0xef,0xbb,0xbf ), u8"", 0 }, /* BOM only */
        { cl7::make_bytes( 0xef,0xbb,0xbf, 0x20 ), u8" ", 1 }, /* BOM with "something" */
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( TEXT(""), container, entry, entry.expected )
    {
        auto actual = cl7::strings::to_utf8_unchecked( entry.input );
        TESTLABS_CHECK_EQ( actual, entry.expected );
        TESTLABS_CHECK_EQ( actual.size(), entry.byte_count );
    }
}

TESTLABS_CASE( TEXT("CoreLabs:  strings::to_utf16_unchecked(const byte_span&)") )
{
    struct Entry
    {
        cl7::byte_vector input;
        cl7::u16string_view expected;
        size_t unit_count;
    } entry;

#pragma warning( push ) // Oh dear, what's next?
#pragma warning( disable: 6276 ) // I hate myself! But we "need" this in order to cast between semantically different string types (with different character sizes).
    const std::vector<Entry> container {
#if ('AB') == 0x4142 // => little endian
        { cl7::make_bytes( 0x70,0x00, 0x75,0x00, 0x72,0x00, 0x65,0x00, 0x20,0x00, 0x41,0x00, 0x53,0x00, 0x43,0x00, 0x49,0x00, 0x49,0x00 ), u"pure ASCII", 10 },
        { cl7::make_bytes( 0xdf,0x00 ), u"ß", 1 }, /* valid Latin-1 character, but invalid ASCII character */
        { cl7::make_bytes( 0x61,0x00,0x6f,0x00,0x75,0x00, 0x20,0x00, 0xe4,0x00,0xf6,0x00,0xfc,0x00, 0x20,0x00, 0x1a,0x00,0x1a,0x00, 0x20,0x00, 0x31,0x00,0x32,0x00,0x33,0x00 ), u"aou äöü \x1a\x1a 123", 14 }, /* just some "random" characters */
        { cl7::make_bytes( 0x80,0x00 ), u"\u0080", 1 }, /* lowest non-ASCII character */
        { cl7::make_bytes( 0xff,0x00 ), u"\u00ff", 1 }, /* highest Latin-1 character */
        { cl7::make_bytes( 0x00,0xd8, 0xff,0xdb, 0x00,0xdc, 0xff,0xdf ), (const cl7::u16char_type*)"\x00\xd8\xff\xdb\x00\xdc\xff\xdf", 4 }, /* Invalid character sequence (should stay the same) */
#endif
#if ('AB') == 0x4241 // => big endian
        { cl7::make_bytes( 0x00,0x70, 0x00,0x75, 0x00,0x72, 0x00,0x65, 0x00,0x20, 0x00,0x41, 0x00,0x53, 0x00,0x43, 0x00,0x49, 0x00,0x49 ), u"pure ASCII", 10 },
        { cl7::make_bytes( 0x00,0xdf ), u"ß", 1 }, /* valid Latin-1 character, but invalid ASCII character */
        { cl7::make_bytes( 0x00,0x61,0x00,0x6f,0x00,0x75, 0x00,0x20, 0x00,0xe4,0x00,0xf6,0x00,0xfc, 0x00,0x20, 0x00,0x1a,0x00,0x1a, 0x00,0x20, 0x00,0x31,0x00,0x32,0x00,0x33 ), u"aou äöü \x1a\x1a 123", 14 }, /* just some "random" characters */
        { cl7::make_bytes( 0x00,0x80 ), u"\u0080", 1 }, /* lowest non-ASCII character */
        { cl7::make_bytes( 0x00,0xff ), u"\u00ff", 1 }, /* highest Latin-1 character */
        { cl7::make_bytes( 0xd8,0x00, 0xdb,0xff, 0xdc,0x00, 0xdf,0xff ), (const cl7::u16char_type*)"\xd8\x00\xdb\xff\xdc\x00\xdf\xff", 4 }, /* Invalid character sequence (should stay the same) */
#endif
        { cl7::make_bytes( 0xff,0xfe ), u"", 0 }, /* BOM only (little endian) */
        { cl7::make_bytes( 0xfe,0xff ), u"", 0 }, /* BOM only (big endian) */
        { cl7::make_bytes( 0xff,0xfe, 0x20,0x00 ), u" ", 1 }, /* BOM (little endian) with "something" */
        { cl7::make_bytes( 0xfe,0xff, 0x00,0x20 ), u" ", 1 }, /* BOM (big endian) with "something" */
    };
#pragma warning( pop ) // Finally!

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( TEXT(""), container, entry, entry.expected )
    {
        auto actual = cl7::strings::to_utf16_unchecked( entry.input );
        TESTLABS_CHECK_EQ( actual, entry.expected );
        TESTLABS_CHECK_EQ( actual.size(), entry.unit_count );
    }
}

TESTLABS_CASE( TEXT("CoreLabs:  strings::to_utf32_unchecked(const byte_span&)") )
{
    struct Entry
    {
        cl7::byte_vector input;
        cl7::u32string_view expected;
        size_t unit_count;
    } entry;

#pragma warning( push ) // Oh dear, what's next?
#pragma warning( disable: 6276 ) // I hate myself! But we "need" this in order to cast between semantically different string types (with different character sizes).
    const std::vector<Entry> container {
#if ('AB') == 0x4142 // => little endian
        { cl7::make_bytes( 0x70,0x00,0x00,0x00, 0x75,0x00,0x00,0x00, 0x72,0x00,0x00,0x00, 0x65,0x00,0x00,0x00, 0x20,0x00,0x00,0x00, 0x41,0x00,0x00,0x00, 0x53,0x00,0x00,0x00, 0x43,0x00,0x00,0x00, 0x49,0x00,0x00,0x00, 0x49,0x00,0x00,0x00 ), U"pure ASCII", 10 },
        { cl7::make_bytes( 0xdf,0x00,0x00,0x00 ), U"ß", 1 }, /* valid Latin-1 character, but invalid ASCII character */
        { cl7::make_bytes( 0x61,0x00,0x00,0x00,0x6f,0x00,0x00,0x00,0x75,0x00,0x00,0x00, 0x20,0x00,0x00,0x00, 0xe4,0x00,0x00,0x00,0xf6,0x00,0x00,0x00,0xfc,0x00,0x00,0x00, 0x20,0x00,0x00,0x00, 0x1a,0x00,0x00,0x00,0x1a,0x00,0x00,0x00, 0x20,0x00,0x00,0x00, 0x31,0x00,0x00,0x00,0x32,0x00,0x00,0x00,0x33,0x00,0x00,0x00 ), U"aou äöü \x1a\x1a 123", 14 }, /* just some "random" characters */
        { cl7::make_bytes( 0x80,0x00,0x00,0x00 ), U"\U00000080", 1 }, /* lowest non-ASCII character */
        { cl7::make_bytes( 0xff,0x00,0x00,0x00 ), U"\U000000ff", 1 }, /* highest Latin-1 character */
        { cl7::make_bytes( 0x00,0xd8,0x00,0x00, 0xff,0xdb,0x00,0x00, 0x00,0xdc,0x00,0x00, 0xff,0xdf,0x00,0x00 ), (const cl7::u32char_type*)"\x00\xd8\x00\x00\xff\xdb\x00\x00\x00\xdc\x00\x00\xff\xdf\x00\x00", 4 }, /* Invalid character sequence (should stay the same) */
#endif
#if ('AB') == 0x4241 // => big endian
        { cl7::make_bytes( 0x00,0x00,0x00,0x70, 0x00,0x00,0x00,0x75, 0x00,0x00,0x00,0x72, 0x00,0x00,0x00,0x65, 0x00,0x00,0x00,0x20, 0x00,0x00,0x00,0x41, 0x00,0x00,0x00,0x53, 0x00,0x00,0x00,0x43, 0x00,0x00,0x00,0x49, 0x00,0x00,0x00,0x49 ), U"pure ASCII", 10 },
        { cl7::make_bytes( 0x00,0x00,0x00,0xdf ), U"ß", 1 }, /* valid Latin-1 character, but invalid ASCII character */
        { cl7::make_bytes( 0x00,0x00,0x00,0x61,0x00,0x00,0x00,0x6f,0x00,0x00,0x00,0x75, 0x00,0x00,0x00,0x20, 0x00,0x00,0x00,0xe4,0x00,0x00,0x00,0xf6,0x00,0x00,0x00,0xfc, 0x00,0x00,0x00,0x20, 0x00,0x00,0x00,0x1a,0x00,0x00,0x00,0x1a, 0x00,0x00,0x00,0x20, 0x00,0x00,0x00,0x31,0x00,0x00,0x00,0x32,0x00,0x00,0x00,0x33 ), U"aou äöü \x1a\x1a 123", 14 }, /* just some "random" characters */
        { cl7::make_bytes( 0x00,0x00,0x00,0x80 ), U"\U00000080", 1 }, /* lowest non-ASCII character */
        { cl7::make_bytes( 0x00,0x00,0x00,0xff ), U"\U000000ff", 1 }, /* highest Latin-1 character */
        { cl7::make_bytes( 0x00,0x00,0xd8,0x00, 0x00,0x00,0xdb,0xff, 0x00,0x00,0xdc,0x00, 0x00,0x00,0xdf,0xff ), (const cl7::u32char_type*)"\x00\x00\xd8\x00\x00\x00\xdb\xff\x00\x00\xdc\x00\x00\x00\xdf\xff", 4 }, /* Invalid character sequence (should stay the same) */
#endif
        { cl7::make_bytes( 0xff,0xfe,0x00,0x00 ), U"", 0 }, /* BOM only (little endian) */
        { cl7::make_bytes( 0x00,0x00,0xfe,0xff ), U"", 0 }, /* BOM only (big endian) */
        { cl7::make_bytes( 0xff,0xfe,0x00,0x00, 0x20,0x00,0x00,0x00 ), U" ", 1 }, /* BOM (little endian) with "something" */
        { cl7::make_bytes( 0x00,0x00,0xfe,0xff, 0x00,0x00,0x00,0x20 ), U" ", 1 }, /* BOM (big endian) with "something" */
    };
#pragma warning( pop ) // Finally!

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( TEXT(""), container, entry, entry.expected )
    {
        auto actual = cl7::strings::to_utf32_unchecked( entry.input );
        TESTLABS_CHECK_EQ( actual, entry.expected );
        TESTLABS_CHECK_EQ( actual.size(), entry.unit_count );
    }
}



TESTLABS_CASE( TEXT("CoreLabs:  strings::to_bytes(const astring_view&)") )
{
    struct Entry
    {
        cl7::astring_view input;
        cl7::byte_vector expected;
    } entry;

    const std::vector<Entry> container {
        { "pure ASCII", cl7::make_bytes( 0x70, 0x75, 0x72, 0x65, 0x20, 0x41, 0x53, 0x43, 0x49, 0x49 ) },
        { "ß", cl7::make_bytes( 0xdf ) }, /* valid Latin-1 character, but invalid ASCII character */
        { "aou äöü \x1a\x1a 123", cl7::make_bytes( 0x61,0x6f,0x75, 0x20, 0xe4,0xf6,0xfc, 0x20, 0x1a,0x1a, 0x20, 0x31,0x32,0x33 ), }, /* just some "random" characters */
        { "\x80", cl7::make_bytes( 0x80 ) }, /* lowest non-ASCII character */
        { "\xff", cl7::make_bytes( 0xff ) }, /* highest Latin-1 character */
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( TEXT(""), container, entry, entry.input )
    {
        auto actual = cl7::strings::to_bytes( entry.input );
        TESTLABS_CHECK_EQ( actual, entry.expected );
    }
}

TESTLABS_CASE( TEXT("CoreLabs:  strings::to_bytes(const u8string_view&, bool)") )
{
    struct Entry
    {
        cl7::u8string_view input;
        bool add_bom;
        cl7::byte_vector expected;
    } entry;

    const std::vector<Entry> container {
        { u8"pure ASCII", false, cl7::make_bytes( 0x70, 0x75, 0x72, 0x65, 0x20, 0x41, 0x53, 0x43, 0x49, 0x49 ) },
        { u8"ß", false, cl7::make_bytes( 0xc3,0x9f ) }, /* valid Latin-1 character, but invalid ASCII character */
        { u8"aou äöü \x1a\x1a 123", false, cl7::make_bytes( 0x61,0x6f,0x75, 0x20, 0xc3,0xa4,0xc3,0xb6,0xc3,0xbc, 0x20, 0x1a,0x1a, 0x20, 0x31,0x32,0x33 ) }, /* just some "random" characters */
        { u8"\u0080", false, cl7::make_bytes( 0xc2,0x80 ) }, /* lowest non-ASCII character */
        { u8"\u00ff", false, cl7::make_bytes( 0xc3,0xbf ) }, /* highest Latin-1 character */
        { u8"", true, cl7::make_bytes( 0xef,0xbb,0xbf ) }, /* BOM only */
        { u8" ", true, cl7::make_bytes( 0xef,0xbb,0xbf, 0x20 ) }, /* BOM with "something" */
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( TEXT(""), container, entry, entry.input )
    {
        auto actual = cl7::strings::to_bytes( entry.input, entry.add_bom );
        TESTLABS_CHECK_EQ( actual, entry.expected );
    }
}

TESTLABS_CASE( TEXT("CoreLabs:  strings::to_bytes(const u16string_view&, bool, std::endian)") )
{
    struct Entry
    {
        cl7::u16string_view input;
        bool add_bom;
        std::endian endian;
        cl7::byte_vector expected;
    } entry;

    const std::vector<Entry> container {
        { u"pure ASCII", false, std::endian::little, cl7::make_bytes( 0x70,0x00, 0x75,0x00, 0x72,0x00, 0x65,0x00, 0x20,0x00, 0x41,0x00, 0x53,0x00, 0x43,0x00, 0x49,0x00, 0x49,0x00 ) },
        { u"ß", false, std::endian::little, cl7::make_bytes( 0xdf,0x00 ) }, /* valid Latin-1 character, but invalid ASCII character */
        { u"aou äöü \x1a\x1a 123", false, std::endian::little, cl7::make_bytes( 0x61,0x00,0x6f,0x00,0x75,0x00, 0x20,0x00, 0xe4,0x00,0xf6,0x00,0xfc,0x00, 0x20,0x00, 0x1a,0x00,0x1a,0x00, 0x20,0x00, 0x31,0x00,0x32,0x00,0x33,0x00 ) }, /* just some "random" characters */
        { u"\u0080", false, std::endian::little, cl7::make_bytes( 0x80,0x00 ) }, /* lowest non-ASCII character */
        { u"\u00ff", false, std::endian::little, cl7::make_bytes( 0xff,0x00 ) }, /* highest Latin-1 character */

        { u"pure ASCII", false, std::endian::big, cl7::make_bytes( 0x00,0x70, 0x00,0x75, 0x00,0x72, 0x00,0x65, 0x00,0x20, 0x00,0x41, 0x00,0x53, 0x00,0x43, 0x00,0x49, 0x00,0x49 ) },
        { u"ß", false, std::endian::big, cl7::make_bytes( 0x00,0xdf ) }, /* valid Latin-1 character, but invalid ASCII character */
        { u"aou äöü \x1a\x1a 123", false, std::endian::big, cl7::make_bytes( 0x00,0x61,0x00,0x6f,0x00,0x75, 0x00,0x20, 0x00,0xe4,0x00,0xf6,0x00,0xfc, 0x00,0x20, 0x00,0x1a,0x00,0x1a, 0x00,0x20, 0x00,0x31,0x00,0x32,0x00,0x33 ) }, /* just some "random" characters */
        { u"\u0080", false, std::endian::big, cl7::make_bytes( 0x00,0x80 ) }, /* lowest non-ASCII character */
        { u"\u00ff", false, std::endian::big, cl7::make_bytes( 0x00,0xff ) }, /* highest Latin-1 character */

        { u"", true, std::endian::little, cl7::make_bytes( 0xff,0xfe ) }, /* BOM only (little endian) */
        { u"", true, std::endian::big, cl7::make_bytes( 0xfe,0xff ) }, /* BOM only (big endian) */
        { u" ", true, std::endian::little, cl7::make_bytes( 0xff,0xfe, 0x20,0x00 ) }, /* BOM (little endian) with "something" */
        { u" ", true, std::endian::big, cl7::make_bytes( 0xfe,0xff, 0x00,0x20 ) }, /* BOM (big endian) with "something" */
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( TEXT(""), container, entry, entry.input )
    {
        auto actual = cl7::strings::to_bytes( entry.input, entry.add_bom, entry.endian );
        TESTLABS_CHECK_EQ( actual, entry.expected );
    }
}

TESTLABS_CASE( TEXT("CoreLabs:  strings::to_bytes(const u32string_view&, bool, std::endian)") )
{
    struct Entry
    {
        cl7::u32string_view input;
        bool add_bom;
        std::endian endian;
        cl7::byte_vector expected;
    } entry;

    const std::vector<Entry> container {
        { U"pure ASCII", false, std::endian::little, cl7::make_bytes( 0x70,0x00,0x00,0x00, 0x75,0x00,0x00,0x00, 0x72,0x00,0x00,0x00, 0x65,0x00,0x00,0x00, 0x20,0x00,0x00,0x00, 0x41,0x00,0x00,0x00, 0x53,0x00,0x00,0x00, 0x43,0x00,0x00,0x00, 0x49,0x00,0x00,0x00, 0x49,0x00,0x00,0x00 ) },
        { U"ß", false, std::endian::little, cl7::make_bytes( 0xdf,0x00,0x00,0x00 ) }, /* valid Latin-1 character, but invalid ASCII character */
        { U"aou äöü \x1a\x1a 123", false, std::endian::little, cl7::make_bytes( 0x61,0x00,0x00,0x00,0x6f,0x00,0x00,0x00,0x75,0x00,0x00,0x00, 0x20,0x00,0x00,0x00, 0xe4,0x00,0x00,0x00,0xf6,0x00,0x00,0x00,0xfc,0x00,0x00,0x00, 0x20,0x00,0x00,0x00, 0x1a,0x00,0x00,0x00,0x1a,0x00,0x00,0x00, 0x20,0x00,0x00,0x00, 0x31,0x00,0x00,0x00,0x32,0x00,0x00,0x00,0x33,0x00,0x00,0x00 ) }, /* just some "random" characters */
        { U"\U00000080", false, std::endian::little, cl7::make_bytes( 0x80,0x00,0x00,0x00 ) }, /* lowest non-ASCII character */
        { U"\U000000ff", false, std::endian::little, cl7::make_bytes( 0xff,0x00,0x00,0x00 ) }, /* highest Latin-1 character */

        { U"pure ASCII", false, std::endian::big, cl7::make_bytes( 0x00,0x00,0x00,0x70, 0x00,0x00,0x00,0x75, 0x00,0x00,0x00,0x72, 0x00,0x00,0x00,0x65, 0x00,0x00,0x00,0x20, 0x00,0x00,0x00,0x41, 0x00,0x00,0x00,0x53, 0x00,0x00,0x00,0x43, 0x00,0x00,0x00,0x49, 0x00,0x00,0x00,0x49 ) },
        { U"ß", false, std::endian::big, cl7::make_bytes( 0x00,0x00,0x00,0xdf ) }, /* valid Latin-1 character, but invalid ASCII character */
        { U"aou äöü \x1a\x1a 123", false, std::endian::big, cl7::make_bytes( 0x00,0x00,0x00,0x61,0x00,0x00,0x00,0x6f,0x00,0x00,0x00,0x75, 0x00,0x00,0x00,0x20, 0x00,0x00,0x00,0xe4,0x00,0x00,0x00,0xf6,0x00,0x00,0x00,0xfc, 0x00,0x00,0x00,0x20, 0x00,0x00,0x00,0x1a,0x00,0x00,0x00,0x1a, 0x00,0x00,0x00,0x20, 0x00,0x00,0x00,0x31,0x00,0x00,0x00,0x32,0x00,0x00,0x00,0x33 ) }, /* just some "random" characters */
        { U"\U00000080", false, std::endian::big, cl7::make_bytes( 0x00,0x00,0x00,0x80 ) }, /* lowest non-ASCII character */
        { U"\U000000ff", false, std::endian::big, cl7::make_bytes( 0x00,0x00,0x00,0xff ) }, /* highest Latin-1 character */

        { U"", true, std::endian::little, cl7::make_bytes( 0xff,0xfe,0x00,0x00 ) }, /* BOM only (little endian) */
        { U"", true, std::endian::big, cl7::make_bytes( 0x00,0x00,0xfe,0xff ) }, /* BOM only (big endian) */
        { U" ", true, std::endian::little, cl7::make_bytes( 0xff,0xfe,0x00,0x00, 0x20,0x00,0x00,0x00 ) }, /* BOM (little endian) with "something" */
        { U" ", true, std::endian::big, cl7::make_bytes( 0x00,0x00,0xfe,0xff, 0x00,0x00,0x00,0x20 ) }, /* BOM (big endian) with "something" */
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( TEXT(""), container, entry, entry.input )
    {
        auto actual = cl7::strings::to_bytes( entry.input, entry.add_bom, entry.endian );
        TESTLABS_CHECK_EQ( actual, entry.expected );
    }
}



#endif // F77_TESTS_CL7_STRINGS_H

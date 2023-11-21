#pragma once
#ifndef F77_TESTS_CL7_STRINGS_H
#define F77_TESTS_CL7_STRINGS_H

#include <CoreLabs/strings.h>

#include <TestLabs/TestSuite.h>

#include <vector>



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
        { prefix"ί", "ί", false }, /* valid Latin-1 character, but invalid ASCII character */ \
        { prefix"\u0100", "\x1a", true }, /* invalid Latin-1 character, but valid replacement character (even ASCII) */ \
        { prefix"aou aφό \ud7ff\ue000 123", "aou aφό \x1a\x1a 123", false }, /* just some valid and invalid code points */ \
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
        { "\xdf", u8"ί", 2 }, // 2-byte sequence
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
        { "\xdf", prefix"ί" }, \
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
        cl7::string comment;
    } entry;

    const std::vector<Entry> container {
        { { 0x70, 0x75, 0x72, 0x65, 0x20, 0x41, 0x53, 0x43, 0x49, 0x49 }, U"pure ASCII", TEXT("pure ASCII") },
        { { 0xd7ff }, U"\ud7ff", TEXT("0xdf77") },
        { { 0xe000 }, U"\ue000", TEXT("0xe000") },
        { { 0xffff }, U"\uffff", TEXT("0xffff") },
        { { 0xd800, 0xdc00 }, U"\U00010000", TEXT("0xd800 0xdc00 <=> 0x10000") },
        { { 0xd800, 0xdc01 }, U"\U00010001", TEXT("0xd800 0xdc01 <=> 0x10001") },
        { { 0xd800, 0xdfff }, U"\U000103ff", TEXT("0xd800 0xdfff <=> 0x103ff") },
        { { 0xd801, 0xdc00 }, U"\U00010400", TEXT("0xd801 0xdc00 <=> 0x10400") },
        { { 0xd801, 0xdc01 }, U"\U00010401", TEXT("0xd801 0xdc01 <=> 0x10401") },
        { { 0xd801, 0xdfff }, U"\U000107ff", TEXT("0xd801 0xdfff <=> 0x107ff") },
        { { 0xdbff, 0xdc00 }, U"\U0010fc00", TEXT("0xdbff 0xdc00 <=> 0x10fc00") },
        { { 0xdbff, 0xdc01 }, U"\U0010fc01", TEXT("0xdbff 0xdc01 <=> 0x10fc01") },
        { { 0xdbff, 0xdfff }, U"\U0010ffff", TEXT("0xdbff 0xdfff <=> 0x10ffff") },
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



#endif // F77_TESTS_CL7_STRINGS_H

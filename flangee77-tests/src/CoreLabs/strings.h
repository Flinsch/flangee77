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



#endif // F77_TESTS_CL7_STRINGS_H

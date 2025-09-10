#ifndef F77_TESTS_CL7_STRINGS_TRANSFORM_H
#define F77_TESTS_CL7_STRINGS_TRANSFORM_H

#include <CoreLabs/strings/transform.h>

#include <TestLabs/TestSuite.h>

#include "../../shared.h"

#include <vector>



TESTLABS_CASE( u8"CoreLabs:  strings:  transform:  trimming left" )
{
    struct Entry
    {
        cl7::u8string input;
        cl7::u8string expected;
    } entry;

    const std::vector<Entry> container {
        { u8"", u8"" },
        { u8" ", u8"" },
        { u8"  ", u8"" },
        { u8"  x", u8"x" },
        { u8"x  ", u8"x  " },
        { u8"  x  ", u8"x  " },
        { u8"\n\tx\n\t", u8"x\n\t" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"<string>", container, entry, entry.input )
    {
        const auto& input = entry.input;
        const auto& expected = entry.expected;

        auto actual = entry.input;
        cl7::strings::transform::trim_left( actual );

        TESTLABS_CHECK_EQ( actual, expected );
        TESTLABS_CHECK_EQ( cl7::strings::transform::trimmed_left( input ), expected );
    }

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"<string_view>", container, entry, entry.input )
    {
        const auto input_view = cl7::make_string_view( entry.input );
        const auto expected_view = cl7::make_string_view( entry.expected );

        auto actual_view = cl7::make_string_view( entry.input );
        cl7::strings::transform::trim_left( actual_view );

        TESTLABS_CHECK_EQ( actual_view, expected_view );
        TESTLABS_CHECK_EQ( cl7::strings::transform::trimmed_left( input_view ), expected_view );
    }
}

TESTLABS_CASE( u8"CoreLabs:  strings:  transform:  trimming right" )
{
    struct Entry
    {
        cl7::u8string input;
        cl7::u8string expected;
    } entry;

    const std::vector<Entry> container {
        { u8"", u8"" },
        { u8" ", u8"" },
        { u8"  ", u8"" },
        { u8"  x", u8"  x" },
        { u8"x  ", u8"x" },
        { u8"  x  ", u8"  x" },
        { u8"\n\tx\n\t", u8"\n\tx" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"<string>", container, entry, entry.input )
    {
        const auto& input = entry.input;
        const auto& expected = entry.expected;

        auto actual = entry.input;
        cl7::strings::transform::trim_right( actual );

        TESTLABS_CHECK_EQ( actual, expected );
        TESTLABS_CHECK_EQ( cl7::strings::transform::trimmed_right( input ), expected );
    }

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"<string_view>", container, entry, entry.input )
    {
        const auto input_view = cl7::make_string_view( entry.input );
        const auto expected_view = cl7::make_string_view( entry.expected );

        auto actual_view = cl7::make_string_view( entry.input );
        cl7::strings::transform::trim_right( actual_view );

        TESTLABS_CHECK_EQ( actual_view, expected_view );
        TESTLABS_CHECK_EQ( cl7::strings::transform::trimmed_right( input_view ), expected_view );
    }
}

TESTLABS_CASE( u8"CoreLabs:  strings:  transform:  trimming both" )
{
    struct Entry
    {
        cl7::u8string input;
        cl7::u8string expected;
    } entry;

    const std::vector<Entry> container {
        { u8"", u8"" },
        { u8" ", u8"" },
        { u8"  ", u8"" },
        { u8"  x", u8"x" },
        { u8"x  ", u8"x" },
        { u8"  x  ", u8"x" },
        { u8"\n\tx\n\t", u8"x" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"<string>", container, entry, entry.input )
    {
        const auto& input = entry.input;
        const auto& expected = entry.expected;

        auto actual = entry.input;
        cl7::strings::transform::trim( actual );

        TESTLABS_CHECK_EQ( actual, expected );
        TESTLABS_CHECK_EQ( cl7::strings::transform::trimmed( input ), expected );
    }

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"<string_view>", container, entry, entry.input )
    {
        const auto input_view = cl7::make_string_view( entry.input );
        const auto expected_view = cl7::make_string_view( entry.expected );

        auto actual_view = cl7::make_string_view( entry.input );
        cl7::strings::transform::trim( actual_view );

        TESTLABS_CHECK_EQ( actual_view, expected_view );
        TESTLABS_CHECK_EQ( cl7::strings::transform::trimmed( input_view ), expected_view );
    }
}



TESTLABS_CASE( u8"CoreLabs:  strings:  transform:  padding left" )
{
    struct Entry
    {
        cl7::u8string input;
        size_t len;
        cl7::u8char_t chr;
        cl7::u8string expected;
    } entry;

    const std::vector<Entry> container {
        { u8"", 0, u8' ', u8"" },
        { u8"", 1, u8' ', u8" " },
        { u8"", 2, u8' ', u8"  " },
        { u8"xx", 2, u8' ', u8"xx" },
        { u8"xx", 3, u8' ', u8" xx" },
        { u8"xx", 4, u8' ', u8"  xx" },
        { u8"x", 2, u8'x', u8"xx" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.input )
    {
        const auto& input = entry.input;
        const auto len = entry.len;
        const auto chr = entry.chr;
        const auto& expected = entry.expected;

        auto actual = entry.input;
        cl7::strings::transform::pad_left( actual, len, chr );

        TESTLABS_CHECK_EQ( actual, expected );
        TESTLABS_CHECK_EQ( cl7::strings::transform::padded_left( input, len, chr ), expected );
    }
}

TESTLABS_CASE( u8"CoreLabs:  strings:  transform:  padding right" )
{
    struct Entry
    {
        cl7::u8string input;
        size_t len;
        cl7::u8char_t chr;
        cl7::u8string expected;
    } entry;

    const std::vector<Entry> container {
        { u8"", 0, u8' ', u8"" },
        { u8"", 1, u8' ', u8" " },
        { u8"", 2, u8' ', u8"  " },
        { u8"xx", 2, u8' ', u8"xx" },
        { u8"xx", 3, u8' ', u8"xx " },
        { u8"xx", 4, u8' ', u8"xx  " },
        { u8"x", 2, u8'x', u8"xx" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.input )
    {
        const auto& input = entry.input;
        const auto len = entry.len;
        const auto chr = entry.chr;
        const auto& expected = entry.expected;

        auto actual = entry.input;
        cl7::strings::transform::pad_right( actual, len, chr );

        TESTLABS_CHECK_EQ( actual, expected );
        TESTLABS_CHECK_EQ( cl7::strings::transform::padded_right( input, len, chr ), expected );
    }
}



TESTLABS_CASE( u8"CoreLabs:  strings:  transform:  to lower ASCII character")
{
    struct Entry
    {
        cl7::u8char_t input;
        cl7::u8char_t expected;
    } entry;

    std::vector<Entry> container;
    for (unsigned i = 0x20; i <= 0x7f; ++i)
        container.push_back( { cl7::u8char_t(i), cl7::u8char_t(i >= 0x41 && i <= 0x5a ? i + 0x20 : i) } );

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.expected )
    {
        TESTLABS_CHECK_EQ( cl7::strings::transform::to_lower_ascii( entry.input ), entry.expected );
    }
}

TESTLABS_CASE( u8"CoreLabs:  strings:  transform:  to upper ASCII character")
{
    struct Entry
    {
        cl7::u8char_t input;
        cl7::u8char_t expected;
    } entry;

    std::vector<Entry> container;
    for (unsigned i = 0x20; i <= 0x7f; ++i)
        container.push_back( { cl7::u8char_t(i), cl7::u8char_t(i >= 0x61 && i <= 0x7a ? i - 0x20 : i) } );

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.expected )
    {
        TESTLABS_CHECK_EQ( cl7::strings::transform::to_upper_ascii( entry.input ), entry.expected );
    }
}

TESTLABS_CASE( u8"CoreLabs:  strings:  transform:  to lower ASCII string")
{
    struct Entry
    {
        cl7::u8string input;
        cl7::u8string expected;
    } entry;

    const std::vector<Entry> container {
        { u8"\n", u8"\n" },
        { u8" ", u8" " },
        { u8"1", u8"1" },
        { u8"9", u8"9" },
        { u8"A", u8"a" },
        { u8"a", u8"a" },
        { u8"Z", u8"z" },
        { u8"z", u8"z" },
        { u8"ThE qUiCk BrOwN fOx JuMpS oVeR tHe LaZy DoG", u8"the quick brown fox jumps over the lazy dog" },
        { u8"tHe QuIcK bRoWn FoX jUmPs OvEr ThE lAzY dOg", u8"the quick brown fox jumps over the lazy dog" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.expected )
    {
        TESTLABS_CHECK_EQ( cl7::strings::transform::to_lower_ascii( entry.input ), entry.expected );
    }
}

TESTLABS_CASE( u8"CoreLabs:  strings:  transform:  to upper ASCII string")
{
    struct Entry
    {
        cl7::u8string input;
        cl7::u8string expected;
    } entry;

    const std::vector<Entry> container {
        { u8"\n", u8"\n" },
        { u8" ", u8" " },
        { u8"1", u8"1" },
        { u8"9", u8"9" },
        { u8"A", u8"A" },
        { u8"a", u8"A" },
        { u8"Z", u8"Z" },
        { u8"z", u8"Z" },
        { u8"ThE qUiCk BrOwN fOx JuMpS oVeR tHe LaZy DoG", u8"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG" },
        { u8"tHe QuIcK bRoWn FoX jUmPs OvEr ThE lAzY dOg", u8"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.expected )
    {
        TESTLABS_CHECK_EQ( cl7::strings::transform::to_upper_ascii( entry.input ), entry.expected );
    }
}



#endif // F77_TESTS_CL7_STRINGS_TRANSFORM_H

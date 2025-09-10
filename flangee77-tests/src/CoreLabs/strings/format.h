#ifndef F77_TESTS_CL7_STRINGS_FORMAT_H
#define F77_TESTS_CL7_STRINGS_FORMAT_H

#include <CoreLabs/strings/format.h>

#include <TestLabs/TestSuite.h>

#include "../../shared.h"

#include <vector>



TESTLABS_CASE( u8"CoreLabs:  strings:  format:  to hex" )
{
    struct Entry
    {
        unsigned val;
        unsigned pad;
        cl7::u8char_t chA;
        cl7::u8string expected;
    } entry;

    const std::vector<Entry> container {
        { 0x00000000, 0, u8'a', u8"" },
        { 0x00000000, 2, u8'a', u8"00" },
        { 0x00000000, 8, u8'a', u8"00000000" },
        { 0x0000bf00, 0, u8'a', u8"bf00" },
        { 0x0000bf00, 8, u8'a', u8"0000bf00" },
        { 0x12345678, 0, u8'a', u8"12345678" },
        { 0xffffffff, 0, u8'a', u8"ffffffff" },
        { 0xffffffff, 0, u8'A', u8"FFFFFFFF" },
        { 0xffffffff, 9, u8'A', u8"0FFFFFFFF" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.expected )
    {
        const auto val = entry.val;
        const auto pad = entry.pad;
        const auto chA = entry.chA;
        const auto& expected = entry.expected;

        TESTLABS_CHECK_EQ( cl7::strings::format::to_hex( val, pad, chA ), expected );
        TESTLABS_CHECK_EQ( cl7::strings::format::to_0xhex( val, pad, chA ), u8"0x" + expected );
        TESTLABS_CHECK_EQ( cl7::strings::format::to_0xhex_lc( val, pad ), u8"0x" + cl7::strings::transform::to_lower_ascii( expected ) );
        TESTLABS_CHECK_EQ( cl7::strings::format::to_0xhex_uc( val, pad ), u8"0x" + cl7::strings::transform::to_upper_ascii( expected ) );
    }
}

TESTLABS_CASE( u8"CoreLabs:  strings:  format:  parse hex" )
{
    struct Entry
    {
        cl7::u8string input;
        unsigned expected;
    } entry;

    const std::vector<Entry> container {
        { u8"", 0x00000000 },
        { u8"00", 0x00000000 },
        { u8"00000000", 0x00000000 },
        { u8"bf00", 0x0000bf00 },
        { u8"0000bf00", 0x0000bf00 },
        { u8"12345678", 0x12345678 },
        { u8"ffffffff", 0xffffffff },
        { u8"FFFFFFFF", 0xffffffff },
        { u8"0FFFFFFFF", 0xffffffff },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.input )
    {
        const auto& input = entry.input;
        const auto expected = entry.expected;

        TESTLABS_CHECK_EQ( cl7::strings::format::parse_hex<>( input ), expected );
        TESTLABS_CHECK_EQ( cl7::strings::format::parse_hex<>( u8"  " + input ), expected );
        TESTLABS_CHECK_EQ( cl7::strings::format::parse_hex<>( u8"0x" + input ), cl7::strings::transform::to_lower_ascii( expected ) );
        TESTLABS_CHECK_EQ( cl7::strings::format::parse_hex<>( u8"  0x" + input ), cl7::strings::transform::to_upper_ascii( expected ) );
        TESTLABS_CHECK_EQ( cl7::strings::format::parse_hex<>( input + u8"XX" ), expected );
        TESTLABS_CHECK_EQ( cl7::strings::format::parse_hex<>( u8"  0x" + input + u8"  " ), expected );
    }
}



#endif // F77_TESTS_CL7_STRINGS_FORMAT_H

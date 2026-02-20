
#include <TestLabs/TestSuite.h>

#include <CoreLabs/text/format.h>

#include "../../shared.h"

#include <vector>



TESTLABS_CASE( u8"CoreLabs:  text:  format:  adjust fixed precision" )
{
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"0", 0 ), u8"0" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"-1", 0 ), u8"-1" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"+1", 0 ), u8"+1" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"1.0", 0 ), u8"1.0" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"-1.0", 0 ), u8"-1.0" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"+1.0", 0 ), u8"+1.0" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"0.", 0 ), u8"0" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"-0.", 0 ), u8"-0" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"+0.", 0 ), u8"+0" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8".0", 0 ), u8"0.0" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"-.0", 0 ), u8"-0.0" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"+.0", 0 ), u8"+0.0" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"123.456", 0 ), u8"123.456" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"-123.456", 0 ), u8"-123.456" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"+123.456", 0 ), u8"+123.456" );

    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"0", 1 ), u8"0.0" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"0.", 1 ), u8"0.0" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8".0", 1 ), u8"0.0" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"1.0", 1 ), u8"1.0" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"123.456", 1 ), u8"123.456" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"-123.456", 1 ), u8"-123.456" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"+123.456", 1 ), u8"+123.456" );

    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"0", 3 ), u8"0.000" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"1.0", 3 ), u8"1.000" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"123.456", 3 ), u8"123.456" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"-123.456", 3 ), u8"-123.456" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"+123.456", 3 ), u8"+123.456" );

    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"0", 4 ), u8"0.0000" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"1.0", 4 ), u8"1.0000" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"123.456", 4 ), u8"123.4560" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"-123.456", 4 ), u8"-123.4560" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"+123.456", 4 ), u8"+123.4560" );

    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"1.0", 0, 0 ), u8"1" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"1.0", 0, 1 ), u8"1.0" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"1.0", 4, 2 ), u8"1.00" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"-123.456", 0, 0 ), u8"-123" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"+123.456", 0, 0 ), u8"+123" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"-123.456", 0, 1 ), u8"-123.5" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"+123.456", 0, 1 ), u8"+123.5" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"-123.789", 0, 1 ), u8"-123.8" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"+123.789", 0, 1 ), u8"+123.8" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"-123.789", 0, 0 ), u8"-124" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"+123.789", 0, 0 ), u8"+124" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8".999", 0, 2 ), u8"1.00" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"-.999", 0, 2 ), u8"-1.00" );
    TESTLABS_CHECK_EQ( cl7::text::format::adjust_fixed_precision( u8"+.999", 0, 2 ), u8"+1.00" );
}



TESTLABS_CASE( u8"CoreLabs:  text:  format:  to string (float)" )
{
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.0f, {} ), u8"0" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.123f, {} ), u8"0.123" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.789f, {} ), u8"0.789" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 12345.0f, {} ), u8"12345" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 123456789.0, {} ), u8"1.23456789e+08" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( -0.012345f, {} ), u8"-0.012345" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.000000789, {} ), u8"7.89e-07" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.000000999, {} ), u8"9.99e-07" );

    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.0f, {.format = cl7::text::format::FloatFormat::Fixed} ), u8"0" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.123f, {.format = cl7::text::format::FloatFormat::Fixed} ), u8"0.123" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.789f, {.format = cl7::text::format::FloatFormat::Fixed} ), u8"0.789" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 12345.0f, {.format = cl7::text::format::FloatFormat::Fixed} ), u8"12345" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 123456789.0, {.format = cl7::text::format::FloatFormat::Fixed} ), u8"123456789" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( -0.012345f, {.format = cl7::text::format::FloatFormat::Fixed} ), u8"-0.012345" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.000000789, {.format = cl7::text::format::FloatFormat::Fixed} ), u8"0.000000789" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.000000999, {.format = cl7::text::format::FloatFormat::Fixed} ), u8"0.000000999" );

    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.0f, {.format = cl7::text::format::FloatFormat::Scientific} ), u8"0e+00" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.123f, {.format = cl7::text::format::FloatFormat::Scientific} ), u8"1.23e-01" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.789f, {.format = cl7::text::format::FloatFormat::Scientific} ), u8"7.89e-01" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 12345.0f, {.format = cl7::text::format::FloatFormat::Scientific} ), u8"1.2345e+04" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 123456789.0, {.format = cl7::text::format::FloatFormat::Scientific} ), u8"1.23456789e+08" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( -0.012345f, {.format = cl7::text::format::FloatFormat::Scientific} ), u8"-1.2345e-02" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.000000789, {.format = cl7::text::format::FloatFormat::Scientific} ), u8"7.89e-07" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.000000999, {.format = cl7::text::format::FloatFormat::Scientific} ), u8"9.99e-07" );

    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.0f, {.min_precision = 4} ), u8"0.0000" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.123f, {.min_precision = 4} ), u8"0.1230" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.789f, {.min_precision = 4} ), u8"0.7890" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 12345.0f, {.min_precision = 4} ), u8"12345.0000" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 123456789.0, {.min_precision = 4} ), u8"1.23456789e+08" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( -0.012345f, {.min_precision = 4} ), u8"-0.012345" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.000000789, {.min_precision = 4} ), u8"7.8900e-07" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.000000999, {.min_precision = 4} ), u8"9.9900e-07" );

    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.0f, {.max_precision = 0} ), u8"0" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.123f, {.max_precision = 0} ), u8"0" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.789f, {.max_precision = 0} ), u8"1" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 12345.0f, {.max_precision = 0} ), u8"12345" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 123456789.0, {.max_precision = 0} ), u8"1e+08" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( -0.012345f, {.max_precision = 0} ), u8"-0" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.000000789, {.max_precision = 0} ), u8"8e-07" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.000000999, {.max_precision = 0} ), u8"1e-06" );

    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.0f, {.max_precision = 1} ), u8"0" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.123f, {.max_precision = 1} ), u8"0.1" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.789f, {.max_precision = 1} ), u8"0.8" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 12345.0f, {.max_precision = 1} ), u8"12345" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 123456789.0, {.max_precision = 1} ), u8"1.2e+08" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( -0.012345f, {.max_precision = 1} ), u8"-0.0" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.000000789, {.max_precision = 1} ), u8"7.9e-07" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.000000999, {.max_precision = 1} ), u8"1.0e-06" );

    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.0f, {.min_precision = 1, .max_precision = 1} ), u8"0.0" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.123f, {.min_precision = 1, .max_precision = 1} ), u8"0.1" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.789f, {.min_precision = 1, .max_precision = 1} ), u8"0.8" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 12345.0f, {.min_precision = 1, .max_precision = 1} ), u8"12345.0" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 123456789.0, {.min_precision = 1, .max_precision = 1} ), u8"1.2e+08" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( -0.012345f, {.min_precision = 1, .max_precision = 1} ), u8"-0.0" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.000000789, {.min_precision = 1, .max_precision = 1} ), u8"7.9e-07" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 0.000000999, {.min_precision = 1, .max_precision = 1} ), u8"1.0e-06" );

    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 123456789.45, {.force_sign = true, .format = cl7::text::format::FloatFormat::Fixed, .min_precision = 1, .max_precision = 1} ), u8"+123456789.5" );
    TESTLABS_CHECK_EQ( cl7::text::format::to_string( 9.99, {.force_sign = true, .format = cl7::text::format::FloatFormat::Scientific, .min_precision = 1, .max_precision = 1} ), u8"+1.0e+01" );
}

TESTLABS_CASE( u8"CoreLabs:  text:  format:  to string (integral)" )
{
    struct Entry
    {
        unsigned val;
        unsigned pad;
        cl7::u8string expected;
    } entry;

    const std::vector<Entry> container {
        { 0, 0, u8"0" },
        { 0, 2, u8"00" },
        { 0, 8, u8"00000000" },
        { 48896, 0, u8"48896" },
        { 48896, 8, u8"00048896" },
        { 4294967295, 0, u8"4294967295" },
        { 4294967295, 11, u8"04294967295" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"unsigned", container, entry, entry.expected )
    {
        const auto val = entry.val;
        const auto pad = entry.pad;
        const auto& expected = entry.expected;

        TESTLABS_CHECK_EQ( cl7::text::format::to_string( val, pad ), expected );
    }

    TESTLABS_SUBCASE( u8"signed" )
    {
        TESTLABS_CHECK_EQ( cl7::text::format::to_string( -77 ), u8"-77" );
        TESTLABS_CHECK_EQ( cl7::text::format::to_string( -77, 4 ), u8"-0077" );
        TESTLABS_CHECK_EQ( cl7::text::format::to_string( 77, { .force_sign = true, .zero_pad_width = 4 } ), u8"+0077" );
    }
}

TESTLABS_CASE( u8"CoreLabs:  text:  format:  to hex" )
{
    struct Entry
    {
        unsigned val;
        unsigned pad;
        cl7::text::format::HexCase hex_case;
        cl7::u8string expected;
    } entry;

    const std::vector<Entry> container {
        { 0x00000000, 0, cl7::text::format::HexCase::Lower, u8"0" },
        { 0x00000000, 2, cl7::text::format::HexCase::Lower, u8"00" },
        { 0x00000000, 8, cl7::text::format::HexCase::Lower, u8"00000000" },
        { 0x0000bf00, 0, cl7::text::format::HexCase::Lower, u8"bf00" },
        { 0x0000bf00, 8, cl7::text::format::HexCase::Lower, u8"0000bf00" },
        { 0x12345678, 0, cl7::text::format::HexCase::Lower, u8"12345678" },
        { 0xffffffff, 0, cl7::text::format::HexCase::Lower, u8"ffffffff" },
        { 0xffffffff, 0, cl7::text::format::HexCase::Upper, u8"FFFFFFFF" },
        { 0xffffffff, 9, cl7::text::format::HexCase::Upper, u8"0FFFFFFFF" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"unsigned", container, entry, entry.expected )
    {
        const auto val = entry.val;
        const auto pad = entry.pad;
        const auto hex_case = entry.hex_case;
        const auto& expected = entry.expected;

        TESTLABS_CHECK_EQ( cl7::text::format::to_hex( val, pad, hex_case ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::to_0x_hex( val, pad, hex_case ), u8"0x" + expected );
        TESTLABS_CHECK_EQ( cl7::text::format::to_0x_hex_lc( val, pad ), u8"0x" + cl7::text::transform::to_lower_ascii( expected ) );
        TESTLABS_CHECK_EQ( cl7::text::format::to_0x_hex_uc( val, pad ), u8"0x" + cl7::text::transform::to_upper_ascii( expected ) );
    }

    TESTLABS_SUBCASE( u8"signed" )
    {
        TESTLABS_CHECK_EQ( cl7::text::format::to_hex( -77 ), u8"-4D" );
        TESTLABS_CHECK_EQ( cl7::text::format::to_hex( -77, 4 ), u8"-004D" );
        TESTLABS_CHECK_EQ( cl7::text::format::to_0x_hex( -77 ), u8"-0x4D" );
        TESTLABS_CHECK_EQ( cl7::text::format::to_0x_hex( -77, 4 ), u8"-0x004D" );
        TESTLABS_CHECK_EQ( cl7::text::format::to_hex( 77, { .force_sign = true, .with_prefix = true, .zero_pad_width = 4 } ), u8"+0x004D" );
    }
}

TESTLABS_CASE( u8"CoreLabs:  text:  format:  to octal" )
{
    struct Entry
    {
        unsigned val;
        unsigned pad;
        cl7::u8string expected;
    } entry;

    const std::vector<Entry> container {
        { 00, 0, u8"0" },
        { 00, 2, u8"00" },
        { 00, 8, u8"00000000" },
        { 0137400, 0, u8"137400" },
        { 0137400, 8, u8"00137400" },
        { 01234567, 0, u8"1234567" },
        { 037777777777, 0, u8"37777777777" },
        { 037777777777, 12, u8"037777777777" },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"unsigned", container, entry, entry.expected )
    {
        const auto val = entry.val;
        const auto pad = entry.pad;
        const auto& expected = entry.expected;

        TESTLABS_CHECK_EQ( cl7::text::format::to_octal( val, pad ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::to_0o_octal( val, pad ), u8"0o" + expected );
    }

    TESTLABS_SUBCASE( u8"signed" )
    {
        TESTLABS_CHECK_EQ( cl7::text::format::to_octal( -77 ), u8"-115" );
        TESTLABS_CHECK_EQ( cl7::text::format::to_octal( -77, 4 ), u8"-0115" );
        TESTLABS_CHECK_EQ( cl7::text::format::to_0o_octal( -77 ), u8"-0o115" );
        TESTLABS_CHECK_EQ( cl7::text::format::to_0o_octal( -77, 4 ), u8"-0o0115" );
        TESTLABS_CHECK_EQ( cl7::text::format::to_octal( 77, { .force_sign = true, .with_prefix = true, .zero_pad_width = 4 } ), u8"+0o0115" );
    }
}

TESTLABS_CASE( u8"CoreLabs:  text:  format:  to binary" )
{
    struct Entry
    {
        unsigned val;
        unsigned pad;
        cl7::u8string expected;
    } entry;

    const std::vector<Entry> container {
            { 0b0, 0, u8"0" },
            { 0b0, 2, u8"00" },
            { 0b0, 8, u8"00000000" },
            { 0b1011111100000000, 0, u8"1011111100000000" },
            { 0b1011111100000000, 18, u8"001011111100000000" },
            { 0b100101101011010000111, 0, u8"100101101011010000111" },
            { 0b11111111111111111111111111111111, 0, u8"11111111111111111111111111111111" },
            { 0b11111111111111111111111111111111, 40, u8"0000000011111111111111111111111111111111" },
        };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"unsigned", container, entry, entry.expected )
    {
        const auto val = entry.val;
        const auto pad = entry.pad;
        const auto& expected = entry.expected;

        TESTLABS_CHECK_EQ( cl7::text::format::to_binary( val, pad ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::to_0b_binary( val, pad ), u8"0b" + expected );
    }

    TESTLABS_SUBCASE( u8"signed" )
    {
        TESTLABS_CHECK_EQ( cl7::text::format::to_binary( -77 ), u8"-1001101" );
        TESTLABS_CHECK_EQ( cl7::text::format::to_binary( -77, 8 ), u8"-01001101" );
        TESTLABS_CHECK_EQ( cl7::text::format::to_0b_binary( -77 ), u8"-0b1001101" );
        TESTLABS_CHECK_EQ( cl7::text::format::to_0b_binary( -77, 8 ), u8"-0b01001101" );
        TESTLABS_CHECK_EQ( cl7::text::format::to_binary( 77, { .force_sign = true, .with_prefix = true, .zero_pad_width = 8 } ), u8"+0b01001101" );
    }
}



TESTLABS_CASE( u8"CoreLabs:  text:  format:  parse float" )
{
    struct Entry
    {
        cl7::u8string input;
        float expected;
    } entry;

    const std::vector<Entry> container {
        { u8"", 0.0f },
        { u8"00", 0.0f },
        { u8"00000000", 0.0f },
        { u8"77", 77.0f },
        { u8"-77", -77.0f },
        { u8"+77", +77.0f },
        { u8"12345678", 12345678.0f },
        { u8"1234.5678", 1234.5678f },
        { u8"2e2", 2e2f },
        { u8"-2e-2", -2e-2f },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"signed", container, entry, entry.input )
    {
        const auto& input = entry.input;
        const auto expected = entry.expected;

        TESTLABS_CHECK_EQ( cl7::text::format::parse_float<>( input ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_float<>( u8"  " + input ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_float<>( input + u8"XX" ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_float<>( u8"  " + input + u8"  " ), expected );
    }

    TESTLABS_SUBCASE( u8"ASCII" )
    {
        TESTLABS_CHECK_EQ( cl7::text::format::parse_float<>( cl7::astring_view("-2e-2") ), -2e-2f );
    }

    TESTLABS_SUBCASE( u8"UTF-8" )
    {
        TESTLABS_CHECK_EQ( cl7::text::format::parse_float<>( cl7::u8string_view(u8"-2e-2") ), -2e-2f );
    }

    TESTLABS_SUBCASE( u8"UTF-16" )
    {
        TESTLABS_CHECK_EQ( cl7::text::format::parse_float<>( cl7::u16string_view(u"-2e-2") ), -2e-2f );
    }

    TESTLABS_SUBCASE( u8"UTF-32" )
    {
        TESTLABS_CHECK_EQ( cl7::text::format::parse_float<>( cl7::u32string_view(U"-2e-2") ), -2e-2f );
    }
}

TESTLABS_CASE( u8"CoreLabs:  text:  format:  parse int" )
{
    struct Entry
    {
        cl7::u8string input;
        signed expected;
    } entry;

    const std::vector<Entry> container {
        { u8"", 0 },
        { u8"00", 0 },
        { u8"00000000", 0 },
        { u8"0001", 1 },
        { u8"77", 77 },
        { u8"-77", -77 },
        { u8"+77", +77 },
        { u8"12345678", 12345678 },

        { u8"0x4d", 77 },
        { u8"-0x4d", -77 },
        { u8"+0x4d", +77 },
        { u8"0o115", 77 },
        { u8"-0o115", -77 },
        { u8"+0o115", +77 },
        { u8"0b1001101", 77 },
        { u8"-0b1001101", -77 },
        { u8"+0b1001101", +77 },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"signed", container, entry, entry.input )
    {
        const auto& input = entry.input;
        const auto expected = entry.expected;

        TESTLABS_CHECK_EQ( cl7::text::format::parse_int<>( input ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_int<>( u8"  " + input ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_int<>( input + u8"XX" ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_int<>( u8"  " + input + u8"  " ), expected );
    }
}

TESTLABS_CASE( u8"CoreLabs:  text:  format:  parse hex" )
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
        { u8"0001", 0x00000001 },
        { u8"bf00", 0x0000bf00 },
        { u8"0000bf00", 0x0000bf00 },
        { u8"12345678", 0x12345678 },
        { u8"ffffffff", 0xffffffff },
        { u8"FFFFFFFF", 0xffffffff },
        { u8"0FFFFFFFF", 0xffffffff },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"unsigned", container, entry, entry.input )
    {
        const auto& input = entry.input;
        const auto expected = entry.expected;

        TESTLABS_CHECK_EQ( cl7::text::format::parse_hex<>( input ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_hex<>( u8"  " + input ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_hex<>( u8"0x" + input ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_hex<>( u8"  0x" + input ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_hex<>( input + u8"XX" ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_hex<>( u8"  0x" + input + u8"  " ), expected );
    }

    TESTLABS_SUBCASE( u8"signed" )
    {
        TESTLABS_CHECK_EQ( cl7::text::format::parse_hex<signed>( cl7::u8string_view(u8"-4d") ), -77 );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_hex<signed>( cl7::u8string_view(u8"-004D") ), -77 );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_hex<signed>( cl7::u8string_view(u8"-0x4d") ), -77 );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_hex<signed>( cl7::u8string_view(u8"+0x4d") ), +77 );
    }
}

TESTLABS_CASE( u8"CoreLabs:  text:  format:  parse octal" )
{
    struct Entry
    {
        cl7::u8string input;
        unsigned expected;
    } entry;

    const std::vector<Entry> container {
        { u8"", 0 },
        { u8"00", 0 },
        { u8"00000000", 0 },
        { u8"0001", 1 },
        { u8"0115", 77 },
        { u8"115", 77 },
        { u8"01234567", 01234567 },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"unsigned", container, entry, entry.input )
    {
        const auto& input = entry.input;
        const auto expected = entry.expected;

        TESTLABS_CHECK_EQ( cl7::text::format::parse_octal<>( input ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_octal<>( u8"  " + input ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_octal<>( u8"0o" + input ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_octal<>( u8"  0o" + input ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_octal<>( input + u8"XX" ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_octal<>( u8"  0o" + input + u8"  " ), expected );
    }

    TESTLABS_SUBCASE( u8"signed" )
    {
        TESTLABS_CHECK_EQ( cl7::text::format::parse_octal<signed>( cl7::u8string_view(u8"-115") ), -77 );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_octal<signed>( cl7::u8string_view(u8"-0115") ), -77 );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_octal<signed>( cl7::u8string_view(u8"-0o115") ), -77 );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_octal<signed>( cl7::u8string_view(u8"+0o115") ), +77 );
    }
}

TESTLABS_CASE( u8"CoreLabs:  text:  format:  parse binary" )
{
    struct Entry
    {
        cl7::u8string input;
        unsigned expected;
    } entry;

    const std::vector<Entry> container {
        { u8"", 0 },
        { u8"00", 0 },
        { u8"00000000", 0 },
        { u8"0001", 1 },
        { u8"01001101", 77 },
        { u8"1001101", 77 },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"unsigned", container, entry, entry.input )
    {
        const auto& input = entry.input;
        const auto expected = entry.expected;

        TESTLABS_CHECK_EQ( cl7::text::format::parse_binary<>( input ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_binary<>( u8"  " + input ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_binary<>( u8"0b" + input ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_binary<>( u8"  0b" + input ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_binary<>( input + u8"XX" ), expected );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_binary<>( u8"  0b" + input + u8"  " ), expected );
    }

    TESTLABS_SUBCASE( u8"signed" )
    {
        TESTLABS_CHECK_EQ( cl7::text::format::parse_binary<signed>( cl7::u8string_view(u8"-1001101") ), -77 );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_binary<signed>( cl7::u8string_view(u8"-01001101") ), -77 );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_binary<signed>( cl7::u8string_view(u8"-0b1001101") ), -77 );
        TESTLABS_CHECK_EQ( cl7::text::format::parse_binary<signed>( cl7::u8string_view(u8"+0b1001101") ), +77 );
    }
}

#pragma once
#ifndef F77_TESTS_ML7_UTILITIES_H
#define F77_TESTS_ML7_UTILITIES_H

#include <MathLabs/utilities.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



TESTLABS_CASE( TEXT("MathLabs:  utilities:  min2") )
{
    struct Entry
    {
        float a, b;
        float expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f, 0.0f },
        { 1.0f, 2.0f, 1.0f },
        { 1.0f, -1.0f, -1.0f },
    };

    TESTLABS_SUBCASE_BATCH( TEXT("min2<float>"), container, entry )
    {
        auto a = entry.a;
        auto b = entry.b;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::utilities::min2( a, b ), expected );
    }
}

TESTLABS_CASE( TEXT("MathLabs:  utilities:  max2") )
{
    struct Entry
    {
        float a, b;
        float expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f, 0.0f },
        { 1.0f, 2.0f, 2.0f },
        { 1.0f, -1.0f, 1.0f },
    };

    TESTLABS_SUBCASE_BATCH( TEXT("max2<float>"), container, entry )
    {
        auto a = entry.a;
        auto b = entry.b;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::utilities::max2( a, b ), expected );
    }
}


TESTLABS_CASE( TEXT("MathLabs:  utilities:  min3") )
{
    struct Entry
    {
        float a, b, c;
        float expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f, 0.0f, 0.0f },
        { 1.0f, 2.0f, 3.0f, 1.0f },
        { 1.0f, -1.0f, 0.5f, -1.0f },
        { 1.0f, 1.5f, -1.0f, -1.0f },
    };

    TESTLABS_SUBCASE_BATCH( TEXT("min3<float>"), container, entry )
    {
        auto a = entry.a;
        auto b = entry.b;
        auto c = entry.c;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::utilities::min3( a, b, c ), expected );
    }
}

TESTLABS_CASE( TEXT("MathLabs:  utilities:  max3") )
{
    struct Entry
    {
        float a, b, c;
        float expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f, 0.0f, 0.0f },
        { 1.0f, 2.0f, 3.0f, 3.0f },
        { 1.0f, -1.0f, 0.5f, 1.0f },
        { 1.0f, 1.5f, -1.0f, 1.5f },
    };

    TESTLABS_SUBCASE_BATCH( TEXT("max3<float>"), container, entry )
    {
        auto a = entry.a;
        auto b = entry.b;
        auto c = entry.c;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::utilities::max3( a, b, c ), expected );
    }
}



TESTLABS_CASE( TEXT("MathLabs:  utilities:  clamp") )
{
    struct Entry
    {
        float x, min, max;
        float expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f, 0.0f, 0.0f },
        { 1.0f, 2.0f, 3.0f, 2.0f },
        { 1.0f, -1.0f, 0.5f, 0.5f },
        { 1.0f, 1.5f, -1.0f, 1.5f },
    };

    TESTLABS_SUBCASE_BATCH( TEXT("clamp<float>"), container, entry )
    {
        auto x = entry.x;
        auto min = entry.min;
        auto max = entry.max;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::utilities::clamp( x, min, max ), expected );
    }
}

TESTLABS_CASE( TEXT("MathLabs:  utilities:  round") )
{
    TESTLABS_SUBCASE( TEXT("round<float>") )
    {
        TESTLABS_CHECK_EQ( ml7::utilities::round( 0.0f ), 0.0f );
        TESTLABS_CHECK_EQ( ml7::utilities::round( 1.0f ), 1.0f );

        TESTLABS_CHECK_EQ( ml7::utilities::round( 0.1f ), 0.0f );
        TESTLABS_CHECK_EQ( ml7::utilities::round( 0.49f ), 0.0f );
        TESTLABS_CHECK_EQ( ml7::utilities::round( 0.5f ), 1.0f );
        TESTLABS_CHECK_EQ( ml7::utilities::round( 0.51f ), 1.0f );
        TESTLABS_CHECK_EQ( ml7::utilities::round( 0.9f ), 1.0f );
        TESTLABS_CHECK_EQ( ml7::utilities::round( 1.5f ), 2.0f );
        TESTLABS_CHECK_EQ( ml7::utilities::round( 1.51f ), 2.0f );

        TESTLABS_CHECK_EQ( ml7::utilities::round( -0.1f ), -0.0f );
        TESTLABS_CHECK_EQ( ml7::utilities::round( -0.49f ), -0.0f );
        TESTLABS_CHECK_EQ( ml7::utilities::round( -0.5f ), -0.0f );
        TESTLABS_CHECK_EQ( ml7::utilities::round( -0.51f ), -1.0f );
        TESTLABS_CHECK_EQ( ml7::utilities::round( -0.9f ), -1.0f );
        TESTLABS_CHECK_EQ( ml7::utilities::round( -1.5f ), -1.0f );
        TESTLABS_CHECK_EQ( ml7::utilities::round( -1.51f ), -2.0f );

        TESTLABS_CHECK_EQ( ml7::utilities::round( 1.234f, 1 ), 1.2f );
        TESTLABS_CHECK_EQ( ml7::utilities::round( 3.456f, 1 ), 3.5f );
        TESTLABS_CHECK_EQ( ml7::utilities::round( -1.234f, 1 ), -1.2f );
        TESTLABS_CHECK_EQ( ml7::utilities::round( -3.456f, 1 ), -3.5f );
    }

    TESTLABS_SUBCASE( TEXT("round<double>") )
    {
        TESTLABS_CHECK_EQ( ml7::utilities::round( 0.0 ), 0.0 );
        TESTLABS_CHECK_EQ( ml7::utilities::round( 1.0 ), 1.0 );

        TESTLABS_CHECK_EQ( ml7::utilities::round( 0.1 ), 0.0 );
        TESTLABS_CHECK_EQ( ml7::utilities::round( 0.49 ), 0.0 );
        TESTLABS_CHECK_EQ( ml7::utilities::round( 0.5 ), 1.0 );
        TESTLABS_CHECK_EQ( ml7::utilities::round( 0.51 ), 1.0 );
        TESTLABS_CHECK_EQ( ml7::utilities::round( 0.9 ), 1.0 );
        TESTLABS_CHECK_EQ( ml7::utilities::round( 1.5 ), 2.0 );
        TESTLABS_CHECK_EQ( ml7::utilities::round( 1.51 ), 2.0 );

        TESTLABS_CHECK_EQ( ml7::utilities::round( -0.1 ), -0.0 );
        TESTLABS_CHECK_EQ( ml7::utilities::round( -0.49 ), -0.0 );
        TESTLABS_CHECK_EQ( ml7::utilities::round( -0.5 ), -0.0 );
        TESTLABS_CHECK_EQ( ml7::utilities::round( -0.51 ), -1.0 );
        TESTLABS_CHECK_EQ( ml7::utilities::round( -0.9 ), -1.0 );
        TESTLABS_CHECK_EQ( ml7::utilities::round( -1.5 ), -1.0 );
        TESTLABS_CHECK_EQ( ml7::utilities::round( -1.51 ), -2.0 );

        TESTLABS_CHECK_EQ( ml7::utilities::round( 1.234, 1 ), 1.2 );
        TESTLABS_CHECK_EQ( ml7::utilities::round( 3.456, 1 ), 3.5 );
        TESTLABS_CHECK_EQ( ml7::utilities::round( -1.234, 1 ), -1.2 );
        TESTLABS_CHECK_EQ( ml7::utilities::round( -3.456, 1 ), -3.5 );
    }
}



TESTLABS_CASE( TEXT("MathLabs:  utilities:  is_power_of_2") )
{
    for ( unsigned x = 0; x < 9999; ++x )
    {
        bool expected = false;
        switch ( x )
        {
        case (1u <<  0):
        case (1u <<  1):
        case (1u <<  2):
        case (1u <<  3):
        case (1u <<  4):
        case (1u <<  5):
        case (1u <<  6):
        case (1u <<  7):
        case (1u <<  8):
        case (1u <<  9):
        case (1u << 10):
        case (1u << 11):
        case (1u << 12):
        case (1u << 13):
        case (1u << 14):
        case (1u << 15):
        case (1u << 16):
        case (1u << 17):
        case (1u << 18):
        case (1u << 19):
        case (1u << 20):
        case (1u << 21):
        case (1u << 22):
        case (1u << 23):
        case (1u << 24):
        case (1u << 25):
        case (1u << 26):
        case (1u << 27):
        case (1u << 28):
        case (1u << 29):
        case (1u << 30):
        case (1u << 31):
            expected = true;
            break;
        }

        TESTLABS_CHECK_EQ( ml7::utilities::is_power_of_2( x ), expected );
    }
}

TESTLABS_CASE( TEXT("MathLabs:  utilities:  prev_power_of_2") )
{
    struct Entry
    {
        unsigned x;
        unsigned expected;
    } entry;

    const std::vector<Entry> container {
        { 0, 0 },
        { 1, 1 },
        { 2, 2 },
        { 3, 2 },
        { 4, 4 },
        { 5, 4 },
        { 6, 4 },
        { 7, 4 },
        { 8, 8 },
        { 9, 8 },
        { 10, 8 },
        { 11, 8 },
        { 12, 8 },
        { 13, 8 },
        { 14, 8 },
        { 15, 8 },
        { 16, 16 },
        { 17, 16 },
        { 18, 16 },
        { 19, 16 },
        { 20, 16 },
        { 0x7fffffff, 0x40000000 },
        { 0x80000000, 0x80000000 },
        { 0x80000001, 0x80000000 },
        { 0xffffffff, 0x80000000 },
    };

    TESTLABS_SUBCASE_BATCH( TEXT("prev_power_of_2"), container, entry )
    {
        auto x = entry.x;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::utilities::prev_power_of_2( x ), expected );
    }
}

TESTLABS_CASE( TEXT("MathLabs:  utilities:  next_power_of_2") )
{
    struct Entry
    {
        unsigned x;
        unsigned expected;
    } entry;

    const std::vector<Entry> container {
        { 0, 1 },
        { 1, 1 },
        { 2, 2 },
        { 3, 4 },
        { 4, 4 },
        { 5, 8 },
        { 6, 8 },
        { 7, 8 },
        { 8, 8 },
        { 9, 16 },
        { 10, 16 },
        { 11, 16 },
        { 12, 16 },
        { 13, 16 },
        { 14, 16 },
        { 15, 16 },
        { 16, 16 },
        { 17, 32 },
        { 18, 32 },
        { 19, 32 },
        { 20, 32 },
        { 0x7fffffff, 0x80000000 },
        { 0x80000000, 0x80000000 },
        { 0x80000001, 0x00000000 },
        { 0xffffffff, 0x00000000 },
    };

    TESTLABS_SUBCASE_BATCH( TEXT("next_power_of_2"), container, entry )
    {
        auto x = entry.x;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::utilities::next_power_of_2( x ), expected );
    }
}



#endif // F77_TESTS_ML7_UTILITIES_H

#pragma once
#ifndef F77_TESTS_ML7_UTILITIES_H
#define F77_TESTS_ML7_UTILITIES_H

#include <MathLabs/utilities.h>

#include <TestLabs/TestSuite.h>



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



#endif // F77_TESTS_ML7_UTILITIES_H

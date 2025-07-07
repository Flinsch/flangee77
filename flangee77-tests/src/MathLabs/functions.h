#ifndef F77_TESTS_ML7_FUNCTIONS_H
#define F77_TESTS_ML7_FUNCTIONS_H

#include <MathLabs/functions.h>

#include <TestLabs/TestSuite.h>

#include "./shared.h"



TESTLABS_CASE( u8"MathLabs:  functions:  pow(T, T)" )
{
    TESTLABS_CHECK_EQ( ml7::pow( 0.0f, 0.0f ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::pow( 5.5f, 0.0f ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::pow( 3.0f, 1.0f ), 3.0f );
    TESTLABS_CHECK_EQ( ml7::pow( 2.0f, 2.0f ), 4.0f );
    TESTLABS_CHECK_EQ( ml7::pow( 2.0f, 3.0f ), 8.0f );
    TESTLABS_CHECK_EQ( ml7::pow( 1.5f, 4.0f ), 5.0625f );
    TESTLABS_CHECK_EQ( ml7::pow( -1.0f, 5.0f ), -1.0f );
    TESTLABS_CHECK_EQ( ml7::pow( -1.0f, 4.0f ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::pow( 0.0f, 3.0f ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::pow( 1.0f, 0.0f ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::pow( 2.0f, -1.0f ), 0.5f );
    TESTLABS_CHECK_EQ( ml7::pow( 4.0f, 0.5f ), 2.0f );
    TESTLABS_CHECK_EQ( ml7::pow( 4.0f, -0.5f ), 0.5f );
}

TESTLABS_CASE( u8"MathLabs:  functions:  pow(T, int)" )
{
    TESTLABS_CHECK_EQ( ml7::pow( 0.0f, 0 ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::pow( 5.5f, 0 ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::pow( 3.0f, 1 ), 3.0f );
    TESTLABS_CHECK_EQ( ml7::pow( 2.0f, 2 ), 4.0f );
    TESTLABS_CHECK_EQ( ml7::pow( 2.0f, 3 ), 8.0f );
    TESTLABS_CHECK_EQ( ml7::pow( 1.5f, 4 ), 5.0625f );
    TESTLABS_CHECK_EQ( ml7::pow( -1.0f, 5 ), -1.0f );
    TESTLABS_CHECK_EQ( ml7::pow( -1.0f, 4 ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::pow( 0.0f, 3 ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::pow( 1.0f, 0 ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::pow( 2.0f, -1 ), 0.5f );
}

TESTLABS_CASE( u8"MathLabs:  functions:  pow<unsigned>(T)" )
{
    TESTLABS_CHECK_EQ( ml7::pow<0>( 0.0f ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::pow<0>( 5.5f ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::pow<1>( 3.0f ), 3.0f );
    TESTLABS_CHECK_EQ( ml7::pow<2>( 2.0f ), 4.0f );
    TESTLABS_CHECK_EQ( ml7::pow<3>( 2.0f ), 8.0f );
    TESTLABS_CHECK_EQ( ml7::pow<4>( 1.5f ), 5.0625f );
    TESTLABS_CHECK_EQ( ml7::pow<5>( -1.0f ), -1.0f );
    TESTLABS_CHECK_EQ( ml7::pow<4>( -1.0f ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::pow<3>( 0.0f ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::pow<0>( 1.0f ), 1.0f );
}

TESTLABS_CASE( u8"MathLabs:  functions:  sqr" )
{
    TESTLABS_CHECK_EQ( ml7::sqr( 0.0f ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::sqr( 1.0f ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::sqr( 2.0f ), 4.0f );
    TESTLABS_CHECK_EQ( ml7::sqr( 3.0f ), 9.0f );
    TESTLABS_CHECK_EQ( ml7::sqr( -1.0f ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::sqr( -2.0f ), 4.0f );
}

TESTLABS_CASE( u8"MathLabs:  functions:  sqrt" )
{
    TESTLABS_CHECK_EQ( ml7::sqrt( 0.0f ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::sqrt( 1.0f ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::sqrt( 4.0f ), 2.0f );
    TESTLABS_CHECK_EQ( ml7::sqrt( 9.0f ), 3.0f );
    TESTLABS_CHECK( std::isnan( ml7::sqrt( -1.0f ) ) );
    TESTLABS_CHECK( std::isnan( ml7::sqrt( -2.0f ) ) );
}

TESTLABS_CASE( u8"MathLabs:  functions:  ssqrt" )
{
    TESTLABS_CHECK_EQ( ml7::sqrt( 0.0f ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::sqrt( 1.0f ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::sqrt( 4.0f ), 2.0f );
    TESTLABS_CHECK_EQ( ml7::sqrt( 9.0f ), 3.0f );
    TESTLABS_CHECK_EQ( ml7::ssqrt( -1.0f ), -1.0f );
    TESTLABS_CHECK_EQ( ml7::ssqrt( -4.0f ), -2.0f );
}



TESTLABS_CASE( u8"MathLabs:  functions:  abs" )
{
    struct Entry
    {
        float x;
        float expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f },
        { 1.0f, 1.0f },
        { -1.0f, 1.0f },
        { 0.5f, 0.5f },
        { -0.5f, 0.5f },
        { 2.0f, 2.0f },
        { -2.0f, 2.0f },
    };

    TESTLABS_SUBCASE_BATCH( u8"abs<float>", container, entry )
    {
        auto x = entry.x;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::abs( x ), expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  sgn" )
{
    struct Entry
    {
        float x;
        float expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f },
        { 1.0f, +1.0f },
        { -1.0f, -1.0f },
        { 0.5f, +1.0f },
        { -0.5f, -1.0f },
        { 2.0f, +1.0f },
        { -2.0f, -1.0f },
    };

    TESTLABS_SUBCASE_BATCH( u8"sgn<float>", container, entry )
    {
        auto x = entry.x;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::sgn( x ), expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  step(x, edge)" )
{
    struct Entry
    {
        float x;
        float edge;
        float expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f, 1.0f },
        { 1.0f, 0.0f, 1.0f },
        { -1.0f, 0.0f, 0.0f },
        { 0.5f, 0.0f, 1.0f },
        { -0.5f, 0.0f, 0.0f },
        { 2.0f, 0.0f, 1.0f },
        { -2.0f, 0.0f, 0.0f },
        { 0.0f, 0.5f, 0.0f },
        { 1.0f, 0.5f, 1.0f },
        { -1.0f, 0.5f, 0.0f },
        { 0.5f, 0.5f, 1.0f },
        { -0.5f, 0.5f, 0.0f },
        { -0.5f, -0.5f, 1.0f },
        { 2.0f, 0.5f, 1.0f },
        { -2.0f, 0.5f, 0.0f },
    };

    TESTLABS_SUBCASE_BATCH( u8"step<float>(x, edge)", container, entry )
    {
        auto x = entry.x;
        auto edge = entry.edge;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::step( x, edge ), expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  step(x)" )
{
    struct Entry
    {
        float x;
        float expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { -1.0f, 0.0f },
        { 0.5f, 1.0f },
        { -0.5f, 0.0f },
        { 2.0f, 1.0f },
        { -2.0f, 0.0f },
    };

    TESTLABS_SUBCASE_BATCH( u8"step<float>(x)", container, entry )
    {
        auto x = entry.x;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::step( x ), expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  smoothstep(x, min, max)" )
{
    struct Entry
    {
        float x;
        float min, max;
        float expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { 1.0f, 0.0f, 1.0f, 1.0f },
        { -1.0f, 0.0f, 1.0f, 0.0f },
        { 0.5f, 0.0f, 1.0f, 0.5f },
        { -0.5f, 0.0f, 1.0f, 0.0f },
        { 2.0f, 0.0f, 1.0f, 1.0f },
        { -2.0f, 0.0f, 1.0f, 0.0f },
        { 0.0f, -1.0f, +1.0f, 0.5f },
        { 1.0f, -1.0f, +1.0f, 1.0f },
        { -1.0f, -1.0f, +1.0f, 0.0f },
        { 0.5f, -1.0f, +1.0f, 0.84375f },
        { -0.5f, -1.0f, +1.0f, 0.15625f },
        { 2.0f, -1.0f, +1.0f, 1.0f },
        { -2.0f, -1.0f, +1.0f, 0.0f },
    };

    TESTLABS_SUBCASE_BATCH( u8"smoothstep<float>(x, min, max)", container, entry )
    {
        auto x = entry.x;
        auto min = entry.min;
        auto max = entry.max;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::smoothstep( x, min, max ), expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  smoothstep(x)" )
{
    struct Entry
    {
        float x;
        float expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f },
        { 1.0f, 1.0f },
        { -1.0f, 0.0f },
        { 0.5f, 0.5f },
        { -0.5f, 0.0f },
        { 2.0f, 1.0f },
        { -2.0f, 0.0f },
    };

    TESTLABS_SUBCASE_BATCH( u8"smoothstep<float>(x)", container, entry )
    {
        auto x = entry.x;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::smoothstep( x ), expected );
    }
}


TESTLABS_CASE( u8"MathLabs:  functions:  min2" )
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

    TESTLABS_SUBCASE_BATCH( u8"min2<float>", container, entry )
    {
        auto a = entry.a;
        auto b = entry.b;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::min2( a, b ), expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  max2" )
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

    TESTLABS_SUBCASE_BATCH( u8"max2<float>", container, entry )
    {
        auto a = entry.a;
        auto b = entry.b;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::max2( a, b ), expected );
    }
}


TESTLABS_CASE( u8"MathLabs:  functions:  min3" )
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

    TESTLABS_SUBCASE_BATCH( u8"min3<float>", container, entry )
    {
        auto a = entry.a;
        auto b = entry.b;
        auto c = entry.c;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::min3( a, b, c ), expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  max3" )
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

    TESTLABS_SUBCASE_BATCH( u8"max3<float>", container, entry )
    {
        auto a = entry.a;
        auto b = entry.b;
        auto c = entry.c;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::max3( a, b, c ), expected );
    }
}



TESTLABS_CASE( u8"MathLabs:  functions:  clamp" )
{
    TESTLABS_CHECK_EQ( ml7::clamp( 0.0f, 0.0f, 0.0f ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::clamp( 1.0f, 2.0f, 3.0f ), 2.0f );
    TESTLABS_CHECK_EQ( ml7::clamp( 1.0f, -1.0f, 0.5f ), 0.5f );
    TESTLABS_CHECK_EQ( ml7::clamp( 1.0f, 1.5f, -1.0f ), 1.5f );

    TESTLABS_CHECK_EQ( (ml7::clamp<0.0f, 0.0f>( 0.0f )), 0.0f );
    TESTLABS_CHECK_EQ( (ml7::clamp<2.0f, 3.0f>( 1.0f )), 2.0f );
    TESTLABS_CHECK_EQ( (ml7::clamp<-1.0f, 0.5f>( 1.0f )), 0.5f );
    TESTLABS_CHECK_EQ( (ml7::clamp<1.5f, -1.0f>( 1.0f )), 1.5f );
}

TESTLABS_CASE( u8"MathLabs:  functions:  clamp01" )
{
    TESTLABS_CHECK_EQ( ml7::clamp01( -0.5f ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::clamp01( 0.0f ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::clamp01( 0.5f ), 0.5f );
    TESTLABS_CHECK_EQ( ml7::clamp01( 1.0f ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::clamp01( 1.5f ), 1.0f );
}



TESTLABS_CASE( u8"MathLabs:  functions:  round" )
{
    TESTLABS_SUBCASE( u8"round<float>" )
    {
        TESTLABS_CHECK_EQ( ml7::round( 0.0f ), 0.0f );
        TESTLABS_CHECK_EQ( ml7::round( 1.0f ), 1.0f );

        TESTLABS_CHECK_EQ( ml7::round( 0.1f ), 0.0f );
        TESTLABS_CHECK_EQ( ml7::round( 0.49f ), 0.0f );
        TESTLABS_CHECK_EQ( ml7::round( 0.5f ), 1.0f );
        TESTLABS_CHECK_EQ( ml7::round( 0.51f ), 1.0f );
        TESTLABS_CHECK_EQ( ml7::round( 0.9f ), 1.0f );
        TESTLABS_CHECK_EQ( ml7::round( 1.5f ), 2.0f );
        TESTLABS_CHECK_EQ( ml7::round( 1.51f ), 2.0f );

        TESTLABS_CHECK_EQ( ml7::round( -0.1f ), -0.0f );
        TESTLABS_CHECK_EQ( ml7::round( -0.49f ), -0.0f );
        TESTLABS_CHECK_EQ( ml7::round( -0.5f ), -0.0f );
        TESTLABS_CHECK_EQ( ml7::round( -0.51f ), -1.0f );
        TESTLABS_CHECK_EQ( ml7::round( -0.9f ), -1.0f );
        TESTLABS_CHECK_EQ( ml7::round( -1.5f ), -1.0f );
        TESTLABS_CHECK_EQ( ml7::round( -1.51f ), -2.0f );

        TESTLABS_CHECK_EQ( ml7::round( 1.234f, 1 ), 1.2f );
        TESTLABS_CHECK_EQ( ml7::round( 3.456f, 1 ), 3.5f );
        TESTLABS_CHECK_EQ( ml7::round( -1.234f, 1 ), -1.2f );
        TESTLABS_CHECK_EQ( ml7::round( -3.456f, 1 ), -3.5f );
    }

    TESTLABS_SUBCASE( u8"round<double>" )
    {
        TESTLABS_CHECK_EQ( ml7::round( 0.0 ), 0.0 );
        TESTLABS_CHECK_EQ( ml7::round( 1.0 ), 1.0 );

        TESTLABS_CHECK_EQ( ml7::round( 0.1 ), 0.0 );
        TESTLABS_CHECK_EQ( ml7::round( 0.49 ), 0.0 );
        TESTLABS_CHECK_EQ( ml7::round( 0.5 ), 1.0 );
        TESTLABS_CHECK_EQ( ml7::round( 0.51 ), 1.0 );
        TESTLABS_CHECK_EQ( ml7::round( 0.9 ), 1.0 );
        TESTLABS_CHECK_EQ( ml7::round( 1.5 ), 2.0 );
        TESTLABS_CHECK_EQ( ml7::round( 1.51 ), 2.0 );

        TESTLABS_CHECK_EQ( ml7::round( -0.1 ), -0.0 );
        TESTLABS_CHECK_EQ( ml7::round( -0.49 ), -0.0 );
        TESTLABS_CHECK_EQ( ml7::round( -0.5 ), -0.0 );
        TESTLABS_CHECK_EQ( ml7::round( -0.51 ), -1.0 );
        TESTLABS_CHECK_EQ( ml7::round( -0.9 ), -1.0 );
        TESTLABS_CHECK_EQ( ml7::round( -1.5 ), -1.0 );
        TESTLABS_CHECK_EQ( ml7::round( -1.51 ), -2.0 );

        TESTLABS_CHECK_EQ( ml7::round( 1.234, 1 ), 1.2 );
        TESTLABS_CHECK_EQ( ml7::round( 3.456, 1 ), 3.5 );
        TESTLABS_CHECK_EQ( ml7::round( -1.234, 1 ), -1.2 );
        TESTLABS_CHECK_EQ( ml7::round( -3.456, 1 ), -3.5 );
    }
}



TESTLABS_CASE( u8"MathLabs:  functions:  is_zero" )
{
    struct Entry
    {
        float input;
        float epsilon;
        bool expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, FLT_EPSILON, true },
        { -1.0f, FLT_EPSILON, false },
        { +1.0f, FLT_EPSILON, false },
        { -FLT_EPSILON, FLT_EPSILON, true },
        { +FLT_EPSILON, FLT_EPSILON, true },
        { -2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { +2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { 0.0f, 1.0f, true },
        { -1.0f, 1.0f, true },
        { +1.0f, 1.0f, true },
        { -1.0f - FLT_EPSILON, 1.0f, false },
        { +1.0f + FLT_EPSILON, 1.0f, false },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"is_zero<float>", container, entry, entry.input )
    {
        TESTLABS_CHECK_EQ( ml7::is_zero( entry.input, entry.epsilon ), entry.expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  is_one" )
{
    struct Entry
    {
        float input;
        float epsilon;
        bool expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, FLT_EPSILON, false },
        { -1.0f, FLT_EPSILON, false },
        { +1.0f, FLT_EPSILON, true },
        { -FLT_EPSILON, FLT_EPSILON, false },
        { +FLT_EPSILON, FLT_EPSILON, false },
        { -1.1f * FLT_EPSILON, FLT_EPSILON, false },
        { +1.1f * FLT_EPSILON, FLT_EPSILON, false },
        { -1.0f - FLT_EPSILON, FLT_EPSILON, false },
        { -1.0f + FLT_EPSILON, FLT_EPSILON, false },
        { +1.0f - FLT_EPSILON, FLT_EPSILON, true },
        { +1.0f + FLT_EPSILON, FLT_EPSILON, true },
        { +1.0f - 2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { +1.0f + 2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { 1.0f, 1.0f, true },
        { -0.0f, 1.0f, true },
        { +2.0f, 1.0f, true },
        { -0.0f - 2.0f * FLT_EPSILON, 1.0f, false },
        { +2.0f + 2.0f * FLT_EPSILON, 1.0f, false },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"is_one<float>", container, entry, entry.input )
    {
        TESTLABS_CHECK_EQ( ml7::is_one( entry.input, entry.epsilon ), entry.expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  is_one_sqr" )
{
    struct Entry
    {
        float input;
        float epsilon;
        bool expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, FLT_EPSILON, false },
        { -1.0f, FLT_EPSILON, false },
        { +1.0f, FLT_EPSILON, true },
        { -FLT_EPSILON, FLT_EPSILON, false },
        { +FLT_EPSILON, FLT_EPSILON, false },
        { -1.1f * FLT_EPSILON, FLT_EPSILON, false },
        { +1.1f * FLT_EPSILON, FLT_EPSILON, false },
        { -1.0f - FLT_EPSILON, FLT_EPSILON, false },
        { -1.0f + FLT_EPSILON, FLT_EPSILON, false },
        { +1.0f - FLT_EPSILON, FLT_EPSILON, true },
        { +1.0f + FLT_EPSILON, FLT_EPSILON, true },
        { +1.0f - 2.0f * FLT_EPSILON, FLT_EPSILON, true },
        { +1.0f + 2.0f * FLT_EPSILON, FLT_EPSILON, true },
        { +1.0f - 3.0f * FLT_EPSILON, FLT_EPSILON, false },
        { +1.0f + 3.0f * FLT_EPSILON, FLT_EPSILON, false },
        { 1.0f, 1.0f, true },
        { -0.0f, 1.0f, true },
        { +2.0f, 1.0f, true },
        { -1.0f, 1.0f, true },
        { +3.0f, 1.0f, true },
        { -1.0f - 2.0f * FLT_EPSILON, 1.0f, false },
        { +3.0f + 2.0f * FLT_EPSILON, 1.0f, false },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"is_one_sqr<float>", container, entry, entry.input )
    {
        TESTLABS_CHECK_EQ( ml7::is_one_sqr( entry.input, entry.epsilon ), entry.expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  is_equal" )
{
    struct Entry
    {
        float a, b;
        float epsilon;
        bool expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f, FLT_EPSILON, true },
        { -1.0f, 0.0f, FLT_EPSILON, false },
        { +1.0f, 0.0f, FLT_EPSILON, false },
        { 0.0f, -1.0f, FLT_EPSILON, false },
        { -1.0f, -1.0f, FLT_EPSILON, true },
        { +1.0f, -1.0f, FLT_EPSILON, false },
        { 0.0f, +1.0f, FLT_EPSILON, false },
        { -1.0f, +1.0f, FLT_EPSILON, false },
        { +1.0f, +1.0f, FLT_EPSILON, true },
        { 0.0f, -FLT_EPSILON, FLT_EPSILON, true },
        { 0.0f, +FLT_EPSILON, FLT_EPSILON, true },
        { 0.0f, -2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { 0.0f, +2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { -1.0f, -1.0f - FLT_EPSILON, FLT_EPSILON, true },
        { -1.0f, -1.0f + FLT_EPSILON, FLT_EPSILON, true },
        { -1.0f, -1.0f - 2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { -1.0f, -1.0f + 2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { +1.0f, +1.0f - FLT_EPSILON, FLT_EPSILON, true },
        { +1.0f, +1.0f + FLT_EPSILON, FLT_EPSILON, true },
        { +1.0f, +1.0f - 2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { +1.0f, +1.0f + 2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { 1.0f, 1.0f, 1.0f, true },
        { 1.0f, -0.0f, 1.0f, true },
        { 1.0f, +2.0f, 1.0f, true },
        { 1.0f, -0.0f - 2.0f * FLT_EPSILON, 1.0f, false },
        { 1.0f, +2.0f + 2.0f * FLT_EPSILON, 1.0f, false },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"is_equal<float>", container, entry, cl7::to_string( entry.a ) + u8" = " + cl7::to_string( entry.b ) )
    {
        TESTLABS_CHECK_EQ( ml7::is_equal( entry.a, entry.b, entry.epsilon ), entry.expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  is_less" )
{
    struct Entry
    {
        float a, b;
        float epsilon;
        bool expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f, FLT_EPSILON, false },
        { -1.0f, 0.0f, FLT_EPSILON, true },
        { +1.0f, 0.0f, FLT_EPSILON, false },
        { 0.0f, -1.0f, FLT_EPSILON, false },
        { -1.0f, -1.0f, FLT_EPSILON, false },
        { +1.0f, -1.0f, FLT_EPSILON, false },
        { 0.0f, +1.0f, FLT_EPSILON, true },
        { -1.0f, +1.0f, FLT_EPSILON, true },
        { +1.0f, +1.0f, FLT_EPSILON, false },
        { 0.0f, -FLT_EPSILON, FLT_EPSILON, false },
        { 0.0f, +FLT_EPSILON, FLT_EPSILON, false },
        { 0.0f, -2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { 0.0f, +2.0f * FLT_EPSILON, FLT_EPSILON, true },
        { -1.0f, -1.0f - FLT_EPSILON, FLT_EPSILON, false },
        { -1.0f, -1.0f + FLT_EPSILON, FLT_EPSILON, false },
        { -1.0f, -1.0f - 2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { -1.0f, -1.0f + 2.0f * FLT_EPSILON, FLT_EPSILON, true },
        { +1.0f, +1.0f - FLT_EPSILON, FLT_EPSILON, false },
        { +1.0f, +1.0f + FLT_EPSILON, FLT_EPSILON, false },
        { +1.0f, +1.0f - 2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { +1.0f, +1.0f + 2.0f * FLT_EPSILON, FLT_EPSILON, true },
        { 1.0f, 1.0f, 1.0f, false },
        { 1.0f, -0.0f, 1.0f, false },
        { 1.0f, +2.0f, 1.0f, false },
        { 1.0f, -0.0f - 2.0f * FLT_EPSILON, 1.0f, false },
        { 1.0f, +2.0f + 2.0f * FLT_EPSILON, 1.0f, true },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"is_less<float>", container, entry, cl7::to_string( entry.a ) + u8" = " + cl7::to_string( entry.b ) )
    {
        TESTLABS_CHECK_EQ( ml7::is_less( entry.a, entry.b, entry.epsilon ), entry.expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  is_less_equal" )
{
    struct Entry
    {
        float a, b;
        float epsilon;
        bool expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f, FLT_EPSILON, true },
        { -1.0f, 0.0f, FLT_EPSILON, true },
        { +1.0f, 0.0f, FLT_EPSILON, false },
        { 0.0f, -1.0f, FLT_EPSILON, false },
        { -1.0f, -1.0f, FLT_EPSILON, true },
        { +1.0f, -1.0f, FLT_EPSILON, false },
        { 0.0f, +1.0f, FLT_EPSILON, true },
        { -1.0f, +1.0f, FLT_EPSILON, true },
        { +1.0f, +1.0f, FLT_EPSILON, true },
        { 0.0f, -FLT_EPSILON, FLT_EPSILON, true },
        { 0.0f, +FLT_EPSILON, FLT_EPSILON, true },
        { 0.0f, -2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { 0.0f, +2.0f * FLT_EPSILON, FLT_EPSILON, true },
        { -1.0f, -1.0f - FLT_EPSILON, FLT_EPSILON, true },
        { -1.0f, -1.0f + FLT_EPSILON, FLT_EPSILON, true },
        { -1.0f, -1.0f - 2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { -1.0f, -1.0f + 2.0f * FLT_EPSILON, FLT_EPSILON, true },
        { +1.0f, +1.0f - FLT_EPSILON, FLT_EPSILON, true },
        { +1.0f, +1.0f + FLT_EPSILON, FLT_EPSILON, true },
        { +1.0f, +1.0f - 2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { +1.0f, +1.0f + 2.0f * FLT_EPSILON, FLT_EPSILON, true },
        { 1.0f, 1.0f, 1.0f, true },
        { 1.0f, -0.0f, 1.0f, true },
        { 1.0f, +2.0f, 1.0f, true },
        { 1.0f, -0.0f - 2.0f * FLT_EPSILON, 1.0f, false },
        { 1.0f, +2.0f + 2.0f * FLT_EPSILON, 1.0f, true },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"is_less_equal<float>", container, entry, cl7::to_string( entry.a ) + u8" = " + cl7::to_string( entry.b ) )
    {
        TESTLABS_CHECK_EQ( ml7::is_less_equal( entry.a, entry.b, entry.epsilon ), entry.expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  is_greater" )
{
    struct Entry
    {
        float a, b;
        float epsilon;
        bool expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f, FLT_EPSILON, false },
        { -1.0f, 0.0f, FLT_EPSILON, false },
        { +1.0f, 0.0f, FLT_EPSILON, true },
        { 0.0f, -1.0f, FLT_EPSILON, true },
        { -1.0f, -1.0f, FLT_EPSILON, false },
        { +1.0f, -1.0f, FLT_EPSILON, true },
        { 0.0f, +1.0f, FLT_EPSILON, false },
        { -1.0f, +1.0f, FLT_EPSILON, false },
        { +1.0f, +1.0f, FLT_EPSILON, false },
        { 0.0f, -FLT_EPSILON, FLT_EPSILON, false },
        { 0.0f, +FLT_EPSILON, FLT_EPSILON, false },
        { 0.0f, -2.0f * FLT_EPSILON, FLT_EPSILON, true },
        { 0.0f, +2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { -1.0f, -1.0f - FLT_EPSILON, FLT_EPSILON, false },
        { -1.0f, -1.0f + FLT_EPSILON, FLT_EPSILON, false },
        { -1.0f, -1.0f - 2.0f * FLT_EPSILON, FLT_EPSILON, true },
        { -1.0f, -1.0f + 2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { +1.0f, +1.0f - FLT_EPSILON, FLT_EPSILON, false },
        { +1.0f, +1.0f + FLT_EPSILON, FLT_EPSILON, false },
        { +1.0f, +1.0f - 2.0f * FLT_EPSILON, FLT_EPSILON, true },
        { +1.0f, +1.0f + 2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { 1.0f, 1.0f, 1.0f, false },
        { 1.0f, -0.0f, 1.0f, false },
        { 1.0f, +2.0f, 1.0f, false },
        { 1.0f, -0.0f - 2.0f * FLT_EPSILON, 1.0f, true },
        { 1.0f, +2.0f + 2.0f * FLT_EPSILON, 1.0f, false },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"is_greater<float>", container, entry, cl7::to_string( entry.a ) + u8" = " + cl7::to_string( entry.b ) )
    {
        TESTLABS_CHECK_EQ( ml7::is_greater( entry.a, entry.b, entry.epsilon ), entry.expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  is_greater_equal" )
{
    struct Entry
    {
        float a, b;
        float epsilon;
        bool expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f, FLT_EPSILON, true },
        { -1.0f, 0.0f, FLT_EPSILON, false },
        { +1.0f, 0.0f, FLT_EPSILON, true },
        { 0.0f, -1.0f, FLT_EPSILON, true },
        { -1.0f, -1.0f, FLT_EPSILON, true },
        { +1.0f, -1.0f, FLT_EPSILON, true },
        { 0.0f, +1.0f, FLT_EPSILON, false },
        { -1.0f, +1.0f, FLT_EPSILON, false },
        { +1.0f, +1.0f, FLT_EPSILON, true },
        { 0.0f, -FLT_EPSILON, FLT_EPSILON, true },
        { 0.0f, +FLT_EPSILON, FLT_EPSILON, true },
        { 0.0f, -2.0f * FLT_EPSILON, FLT_EPSILON, true },
        { 0.0f, +2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { -1.0f, -1.0f - FLT_EPSILON, FLT_EPSILON, true },
        { -1.0f, -1.0f + FLT_EPSILON, FLT_EPSILON, true },
        { -1.0f, -1.0f - 2.0f * FLT_EPSILON, FLT_EPSILON, true },
        { -1.0f, -1.0f + 2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { +1.0f, +1.0f - FLT_EPSILON, FLT_EPSILON, true },
        { +1.0f, +1.0f + FLT_EPSILON, FLT_EPSILON, true },
        { +1.0f, +1.0f - 2.0f * FLT_EPSILON, FLT_EPSILON, true },
        { +1.0f, +1.0f + 2.0f * FLT_EPSILON, FLT_EPSILON, false },
        { 1.0f, 1.0f, 1.0f, true },
        { 1.0f, -0.0f, 1.0f, true },
        { 1.0f, +2.0f, 1.0f, true },
        { 1.0f, -0.0f - 2.0f * FLT_EPSILON, 1.0f, true },
        { 1.0f, +2.0f + 2.0f * FLT_EPSILON, 1.0f, false },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"is_greater_equal<float>", container, entry, cl7::to_string( entry.a ) + u8" = " + cl7::to_string( entry.b ) )
    {
        TESTLABS_CHECK_EQ( ml7::is_greater_equal( entry.a, entry.b, entry.epsilon ), entry.expected );
    }
}



TESTLABS_CASE( u8"MathLabs:  functions:  is_equal_ulps" )
{
    struct Entry
    {
        float a, b;
        int ulps;
        bool expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f, 1000, true },
        { -1.0f, 0.0f, 1000, false },
        { +1.0f, 0.0f, 1000, false },
        { 0.0f, -1.0f, 1000, false },
        { -1.0f, -1.0f, 1000, true },
        { +1.0f, -1.0f, 1000, false },
        { 0.0f, +1.0f, 1000, false },
        { -1.0f, +1.0f, 1000, false },
        { +1.0f, +1.0f, 1000, true },
        { -0.0f, +0.0f, 1000, true },
        { +0.0f, -0.0f, 1000, true },
        { -1.0f, -0.99999988079071044921875f, 2, true },
        { -1.0f, -1.0000002384185791015625f, 2, true },
        { +1.0f, +0.99999988079071044921875f, 2, true },
        { +1.0f, +1.0000002384185791015625f, 2, true },
        { -1.0f, -0.99999988079071044921875f, 1, false },
        { -1.0f, -1.0000002384185791015625f, 1, false },
        { +1.0f, +0.99999988079071044921875f, 1, false },
        { +1.0f, +1.0000002384185791015625f, 1, false },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"is_equal_ulps<float>", container, entry, cl7::to_string( entry.a ) + u8" = " + cl7::to_string( entry.b ) )
    {
        TESTLABS_CHECK_EQ( ml7::is_equal_ulps( entry.a, entry.b, entry.ulps ), entry.expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  is_less_ulps" )
{
    struct Entry
    {
        float a, b;
        int ulps;
        bool expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f, 1000, false },
        { -1.0f, 0.0f, 1000, true },
        { +1.0f, 0.0f, 1000, false },
        { 0.0f, -1.0f, 1000, false },
        { -1.0f, -1.0f, 1000, false },
        { +1.0f, -1.0f, 1000, false },
        { 0.0f, +1.0f, 1000, true },
        { -1.0f, +1.0f, 1000, true },
        { +1.0f, +1.0f, 1000, false },
        { -0.0f, +0.0f, 1000, false },
        { +0.0f, -0.0f, 1000, false },
        { -1.0f, -0.99999988079071044921875f, 2, false },
        { -1.0f, -1.0000002384185791015625f, 2, false },
        { +1.0f, +0.99999988079071044921875f, 2, false },
        { +1.0f, +1.0000002384185791015625f, 2, false },
        { -1.0f, -0.99999988079071044921875f, 1, true },
        { -1.0f, -1.0000002384185791015625f, 1, false },
        { +1.0f, +0.99999988079071044921875f, 1, false },
        { +1.0f, +1.0000002384185791015625f, 1, true },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"is_less_ulps<float>", container, entry, cl7::to_string( entry.a ) + u8" = " + cl7::to_string( entry.b ) )
    {
        TESTLABS_CHECK_EQ( ml7::is_less_ulps( entry.a, entry.b, entry.ulps ), entry.expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  is_less_equal_ulps" )
{
    struct Entry
    {
        float a, b;
        int ulps;
        bool expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f, 1000, true },
        { -1.0f, 0.0f, 1000, true },
        { +1.0f, 0.0f, 1000, false },
        { 0.0f, -1.0f, 1000, false },
        { -1.0f, -1.0f, 1000, true },
        { +1.0f, -1.0f, 1000, false },
        { 0.0f, +1.0f, 1000, true },
        { -1.0f, +1.0f, 1000, true },
        { +1.0f, +1.0f, 1000, true },
        { -0.0f, +0.0f, 1000, true },
        { +0.0f, -0.0f, 1000, true },
        { -1.0f, -0.99999988079071044921875f, 2, true },
        { -1.0f, -1.0000002384185791015625f, 2, true },
        { +1.0f, +0.99999988079071044921875f, 2, true },
        { +1.0f, +1.0000002384185791015625f, 2, true },
        { -1.0f, -0.99999988079071044921875f, 1, true },
        { -1.0f, -1.0000002384185791015625f, 1, false },
        { +1.0f, +0.99999988079071044921875f, 1, false },
        { +1.0f, +1.0000002384185791015625f, 1, true },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"is_less_equal_ulps<float>", container, entry, cl7::to_string( entry.a ) + u8" = " + cl7::to_string( entry.b ) )
    {
        TESTLABS_CHECK_EQ( ml7::is_less_equal_ulps( entry.a, entry.b, entry.ulps ), entry.expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  is_greater_ulps" )
{
    struct Entry
    {
        float a, b;
        int ulps;
        bool expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f, 1000, false },
        { -1.0f, 0.0f, 1000, false },
        { +1.0f, 0.0f, 1000, true },
        { 0.0f, -1.0f, 1000, true },
        { -1.0f, -1.0f, 1000, false },
        { +1.0f, -1.0f, 1000, true },
        { 0.0f, +1.0f, 1000, false },
        { -1.0f, +1.0f, 1000, false },
        { +1.0f, +1.0f, 1000, false },
        { -0.0f, +0.0f, 1000, false },
        { +0.0f, -0.0f, 1000, false },
        { -1.0f, -0.99999988079071044921875f, 2, false },
        { -1.0f, -1.0000002384185791015625f, 2, false },
        { +1.0f, +0.99999988079071044921875f, 2, false },
        { +1.0f, +1.0000002384185791015625f, 2, false },
        { -1.0f, -0.99999988079071044921875f, 1, false },
        { -1.0f, -1.0000002384185791015625f, 1, true },
        { +1.0f, +0.99999988079071044921875f, 1, true },
        { +1.0f, +1.0000002384185791015625f, 1, false },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"is_greater_ulps<float>", container, entry, cl7::to_string( entry.a ) + u8" = " + cl7::to_string( entry.b ) )
    {
        TESTLABS_CHECK_EQ( ml7::is_greater_ulps( entry.a, entry.b, entry.ulps ), entry.expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  is_greater_equal_ulps" )
{
    struct Entry
    {
        float a, b;
        int ulps;
        bool expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f, 1000, true },
        { -1.0f, 0.0f, 1000, false },
        { +1.0f, 0.0f, 1000, true },
        { 0.0f, -1.0f, 1000, true },
        { -1.0f, -1.0f, 1000, true },
        { +1.0f, -1.0f, 1000, true },
        { 0.0f, +1.0f, 1000, false },
        { -1.0f, +1.0f, 1000, false },
        { +1.0f, +1.0f, 1000, true },
        { -0.0f, +0.0f, 1000, true },
        { +0.0f, -0.0f, 1000, true },
        { -1.0f, -0.99999988079071044921875f, 2, true },
        { -1.0f, -1.0000002384185791015625f, 2, true },
        { +1.0f, +0.99999988079071044921875f, 2, true },
        { +1.0f, +1.0000002384185791015625f, 2, true },
        { -1.0f, -0.99999988079071044921875f, 1, false },
        { -1.0f, -1.0000002384185791015625f, 1, true },
        { +1.0f, +0.99999988079071044921875f, 1, true },
        { +1.0f, +1.0000002384185791015625f, 1, false },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"is_greater_equal_ulps<float>", container, entry, cl7::to_string( entry.a ) + u8" = " + cl7::to_string( entry.b ) )
    {
        TESTLABS_CHECK_EQ( ml7::is_greater_equal_ulps( entry.a, entry.b, entry.ulps ), entry.expected );
    }
}



TESTLABS_CASE( u8"MathLabs:  functions:  deg_to_rad" )
{
    TESTLABS_SUBCASE( u8"deg_to_rad<float>" )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::deg_to_rad( 1.0f ), 5 ), ml7::round( 0.0174533f, 5 ) );
    }

    TESTLABS_SUBCASE( u8"deg_to_rad<double>" )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::deg_to_rad( 1.0 ), 5 ), ml7::round( 0.0174533, 5 ) );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  rad_to_deg" )
{
    TESTLABS_SUBCASE( u8"rad_to_deg<float>" )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::rad_to_deg( 1.0f ), 4 ), ml7::round( 57.2958f, 4 ) );
    }

    TESTLABS_SUBCASE( u8"rad_to_deg<double>" )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::rad_to_deg( 1.0 ), 4 ), ml7::round( 57.2958, 4 ) );
    }
}



TESTLABS_CASE( u8"MathLabs:  functions:  is_power_of_2" )
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

        TESTLABS_CHECK_EQ( ml7::is_power_of_2( x ), expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  prev_power_of_2" )
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

    TESTLABS_SUBCASE_BATCH( u8"prev_power_of_2", container, entry )
    {
        auto x = entry.x;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::prev_power_of_2( x ), expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  next_power_of_2" )
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

    TESTLABS_SUBCASE_BATCH( u8"next_power_of_2", container, entry )
    {
        auto x = entry.x;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::next_power_of_2( x ), expected );
    }
}

TESTLABS_CASE( u8"MathLabs:  functions:  decimal_digits" )
{
    struct Entry
    {
        unsigned x;
        unsigned expected;
    } entry;

    const std::vector<Entry> container {
        { 0, 1 },
        { 1, 1 },
        { 2, 1 },
        { 3, 1 },
        { 4, 1 },
        { 5, 1 },
        { 6, 1 },
        { 7, 1 },
        { 8, 1 },
        { 9, 1 },
        { 10, 2 },
        { 11, 2 },
        { 12, 2 },
        { 13, 2 },
        { 14, 2 },
        { 15, 2 },
        { 16, 2 },
        { 17, 2 },
        { 18, 2 },
        { 19, 2 },
        { 20, 2 },
        { 99, 2 },
        { 100, 3 },
        { 999, 3 },
        { 1000, 4 },
        { 9999, 4 },
        { 10000, 5 },
        { 99999, 5 },
        { 100000, 6 },
        { 999999, 6 },
        { 1000000, 7 },
        { 9999999, 7 },
        { 10000000, 8 },
        { 99999999, 8 },
        { 100000000, 9 },
        { 999999999, 9 },
        { 1000000000, 10 },
        { 0x7fffffff, 10 },
        { 0x80000000, 10 },
        { 0x80000001, 10 },
        { 0xffffffff, 10 },
    };

    TESTLABS_SUBCASE_BATCH( u8"decimal_digits", container, entry )
    {
        auto x = entry.x;
        auto expected = entry.expected;
        TESTLABS_CHECK_EQ( ml7::decimal_digits( x ), expected );
    }
}



#endif // F77_TESTS_ML7_FUNCTIONS_H

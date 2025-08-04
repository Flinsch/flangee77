#ifndef F77_TESTS_ML7_ALGORITHM_H
#define F77_TESTS_ML7_ALGORITHM_H

#include <MathLabs/algorithm/find_quadratic_roots.h>
#include <MathLabs/algorithm/find_cubic_roots.h>

#include <TestLabs/TestSuite.h>

#include "./shared.h"



TESTLABS_CASE( u8"MathLabs:  algorithm:  find_quadratic_roots" )
{
    struct Entry
    {
        float a, b, c;
        cl7::static_vector<float, 2> expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f, 0.0f, {} },
        { 1.0f, 0.0f, 0.0f, { 0.0f } },
        { 0.0f, 1.0f, 0.0f, { 0.0f } },
        { 1.0f, 0.0f, -1.0f, { -1.0f, 1.0f } },
        { -1.0f, 0.0f, 1.0f, { -1.0f, 1.0f } },
        { 1.0f, 3.0f, -3.0f, { -3.791288f, 0.791288f } },
        { -4.0f, 2.0f, 1.0f, { -0.309017f, 0.809017f } },
        { -1.0f, 4.0f, 4.0f, { -0.828427f, 4.828427f } },
        { -1.0f, 4.0f, 0.0f, { 0.0f, 4.0f } },
        { 3.0f, -3.0f, -1.0f, { -0.263763f, 1.263763f } },
        { -1.0f, 2.0f, -4.0f, {} },
        { -1.0f, 2.0f, -1.0f, { 1.0f } },
        { 1.0f, 3.0f, -1.0f, { -3.302776f, 0.302776f } },
        { -2.0f, 3.0f, 1.0f, { -0.280776f, 1.780776f } },
    };

    TESTLABS_SUBCASE_BATCH( u8"", container, entry )
    {
        const auto actual = ml7::algorithm::find_quadratic_roots( entry.a, entry.b, entry.c );

        TESTLABS_CHECK_EQ( actual.size(), entry.expected.size() );
        for ( size_t i = 0; i < (std::min)(actual.size(), entry.expected.size()); ++i )
        {
            TESTLABS_CHECK_EQ_FLT( ml7::round( actual[i], 5 ), ml7::round( entry.expected[i], 5 ) );
            TESTLABS_CHECK_EQ_FLT( ml7::round( entry.a * actual[i]*actual[i] + entry.b * actual[i] + entry.c, 4 ), 0.0f );
        }
    }
}



TESTLABS_CASE( u8"MathLabs:  algorithm:  find_cubic_roots" )
{
    struct Entry
    {
        float a, b, c, d;
        cl7::static_vector<float, 3> expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f, 0.0f, 0.0f, 0.0f, {} },
        { 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f } },
        { 0.0f, 1.0f, 0.0f, 0.0f, { 0.0f } },
        { 0.0f, 0.0f, 1.0f, 0.0f, { 0.0f } },
        { 1.0f, 1.0f, 0.0f, 0.0f, { -1.0f, 0.0f } },
        { 1.0f, -1.0f, 0.0f, 0.0f, { 0.0f, 1.0f } },
        { 4.0f, -2.0f, 3.0f, -1.0f, { 0.357613f } },
        { 1.0f, 0.0f, 2.0f, 2.0f, { -0.770917f } },
        { -4.0f, -2.0f, -3.0f, 1.0f, { 0.262979f } },
        { 3.0f, 3.0f, 4.0f, 0.0f, { 0.0f } },
        { 0.0f, -4.0f, 4.0f, 0.0f, { 0.0f, 1.0f } },
        { -0.7f, 3.0f, -1.0f, -3.0f, { -0.78895f, 1.53441f, 3.54025f } },
        { 1.0f, -2.0f, 0.0f, 1.0f, { -0.61803f, 1.0f, 1.61803f } },
        { -0.5f, 1.0f, 1.0f, -1.0f, { -1.17009f, 0.68889f, 2.48119f } },
    };

    TESTLABS_SUBCASE_BATCH( u8"", container, entry )
    {
        const auto actual = ml7::algorithm::find_cubic_roots( entry.a, entry.b, entry.c, entry.d );

        TESTLABS_CHECK_EQ( actual.size(), entry.expected.size() );
        for ( size_t i = 0; i < (std::min)(actual.size(), entry.expected.size()); ++i )
        {
            TESTLABS_CHECK_EQ_FLT( ml7::round( actual[i], 5 ), ml7::round( entry.expected[i], 5 ) );
            TESTLABS_CHECK_EQ_FLT( ml7::round( entry.a * actual[i]*actual[i]*actual[i] + entry.b * actual[i]*actual[i] + entry.c * actual[i] + entry.d, 4 ), 0.0f );
        }
    }
}



#endif // F77_TESTS_ML7_ALGORITHM_H

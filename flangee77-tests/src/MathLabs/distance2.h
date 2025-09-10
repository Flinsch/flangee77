#ifndef F77_TESTS_ML7_DISTANCE2_H
#define F77_TESTS_ML7_DISTANCE2_H

#include <MathLabs/distance2.h>

#include <TestLabs/TestSuite.h>

#include "./shared.h"



TESTLABS_CASE( u8"MathLabs:  distance2:  point_ray" )
{
    struct Entry
    {
        ml7::Vector2 point;
        ml7::Vector2 origin;
        ml7::Vector2 direction;
        float expected_t;
    } entry;

    const std::vector<Entry> container {
        { { 4.0f, -2.0f }, { 1.0f, 2.0f }, { -3.0f, 4.0f }, 0.0f },
        { { 1.0f, 2.0f }, { 1.0f, 2.0f }, { -3.0f, 4.0f }, 0.0f },
        { { -0.5f, 4.0f }, { 1.0f, 2.0f }, { -3.0f, 4.0f }, 0.5f },
        { { -2.0f, 6.0f }, { 1.0f, 2.0f }, { -3.0f, 4.0f }, 1.0f },
        { { -5.0f, 10.0f }, { 1.0f, 2.0f }, { -3.0f, 4.0f }, 2.0f },
        { { 0.0f, -5.0f }, { 1.0f, 2.0f }, { -3.0f, 4.0f }, 0.0f },
        { { -3.0f, -1.0f }, { 1.0f, 2.0f }, { -3.0f, 4.0f }, 0.0f },
        { { -4.5f, 1.0f }, { 1.0f, 2.0f }, { -3.0f, 4.0f }, 0.5f },
        { { -6.0f, 3.0f }, { 1.0f, 2.0f }, { -3.0f, 4.0f }, 1.0f },
        { { -9.0f, 7.0f }, { 1.0f, 2.0f }, { -3.0f, 4.0f }, 2.0f },
        { { 8.0f, 1.0f }, { 1.0f, 2.0f }, { -3.0f, 4.0f }, 0.0f },
        { { 5.0f, 5.0f }, { 1.0f, 2.0f }, { -3.0f, 4.0f }, 0.0f },
        { { 3.5f, 7.0f }, { 1.0f, 2.0f }, { -3.0f, 4.0f }, 0.5f },
        { { 2.0f, 9.0f }, { 1.0f, 2.0f }, { -3.0f, 4.0f }, 1.0f },
        { { -1.0f, 13.0f }, { 1.0f, 2.0f }, { -3.0f, 4.0f }, 2.0f },
    };

    TESTLABS_SUBCASE_BATCH( u8"", container, entry )
    {
        const auto& point = entry.point;
        const auto ray = ml7::Ray2{ entry.origin, entry.direction };

        ml7::distance2::PointResult expected;
        expected.t = entry.expected_t;
        expected.point = ray.point_at( expected.t );
        expected.distance_squared = (expected.point - entry.point).length_squared();

        const auto actual = ml7::distance2::point_ray( point, ray );

        TESTLABS_CHECK_EQ( ml7::round( actual.distance_squared, 4 ), ml7::round( expected.distance_squared, 4 ) );
        TESTLABS_CHECK_EQ( ml7::round( actual.distance(), 4 ), ml7::round( expected.distance(), 4 ) );
        TESTLABS_CHECK_EQ( ml7::round( actual.point, 4 ), ml7::round( expected.point, 4 ) );
        TESTLABS_CHECK_EQ( ml7::round( actual.t, 4 ), ml7::round( expected.t, 4 ) );
    }
}



#endif // F77_TESTS_ML7_DISTANCE2_H

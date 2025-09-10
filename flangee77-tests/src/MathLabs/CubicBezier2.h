#ifndef F77_TESTS_ML7_CUBICBEZIER2_H
#define F77_TESTS_ML7_CUBICBEZIER2_H

#include <MathLabs/CubicBezier2.h>

#include <TestLabs/TestSuite.h>

#include "./shared.h"



namespace tl7::internals {
    inline
    cl7::u8string to_string(const ml7::CubicBezier2& bezier) { return u8"{ " + to_string(bezier.p0) + u8", " + to_string(bezier.p1) + u8", " + to_string(bezier.p2) + u8", " + to_string(bezier.p3) + u8" }"; }
}



TESTLABS_CASE( u8"CoreLabs:  CubicBezier2:  " )
{
    TESTLABS_CHECK_EQ( ml7::CubicBezier2(), ml7::CubicBezier2( ml7::Vector2( 0.0f, 0.0f ), ml7::Vector2( 0.0f, 0.0f ), ml7::Vector2( 0.0f, 0.0f ), ml7::Vector2( 0.0f, 0.0f ) ) );

    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).control_points().size(), 4 );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).control_points()[0], ml7::Vector2( -1.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).control_points()[1], ml7::Vector2( 0.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).control_points()[2], ml7::Vector2( 0.0f, -2.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).control_points()[3], ml7::Vector2( 1.0f, 1.0f ) );

    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).point_at( -1.0f ), ml7::Vector2( -9.0f, -45.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).point_at( 0.0f ), ml7::Vector2( -1.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).point_at( 0.5f ), ml7::Vector2( 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).point_at( 1.0f ), ml7::Vector2( 1.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).point_at( 2.0f ), ml7::Vector2( 9.0f, 45.0f ) );

    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).tangent_at( -1.0f ), ml7::Vector2( 15.0f, 93.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).tangent_at( 0.0f ), ml7::Vector2( 3.0f, 9.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).tangent_at( 0.5f ), ml7::Vector2( 1.5f, -1.5f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).tangent_at( 1.0f ), ml7::Vector2( 3.0f, 9.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).tangent_at( 2.0f ), ml7::Vector2( 15.0f, 93.0f ) );

    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).second_derivative_at( -1.0f ), ml7::Vector2( -18.0f, -126.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).second_derivative_at( 0.0f ), ml7::Vector2( -6.0f, -42.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).second_derivative_at( 0.5f ), ml7::Vector2( 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).second_derivative_at( 1.0f ), ml7::Vector2( 6.0f, 42.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).second_derivative_at( 2.0f ), ml7::Vector2( 18.0f, 126.0f ) );

    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).parameter( ml7::Vector2( -9.0f, -45.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).parameter( ml7::Vector2( -1.0f, -1.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).parameter( ml7::Vector2( 0.0f, 0.0f ) ), 0.5f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).parameter( ml7::Vector2( 1.0f, 1.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).parameter( ml7::Vector2( 9.0f, 45.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).parameter( ml7::Vector2( -10.0f, -44.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).parameter( ml7::Vector2( -1.75f, -0.75f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 4, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).parameter( ml7::Vector2( 0.1f, 0.1f ) ), 0.5f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).parameter( ml7::Vector2( 0.5f, 1.25f ) ), 1.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).parameter( ml7::Vector2( 8.0f, 46.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).parameter( ml7::Vector2( -8.0f, -46.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).parameter( ml7::Vector2( -0.5f, -1.25f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 4, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).parameter( ml7::Vector2( -0.1f, -0.1f ) ), 0.5f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).parameter( ml7::Vector2( 1.75f, 0.75f ) ), 1.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).parameter( ml7::Vector2( 10.0f, 44.0f ) ), 1.0f );

    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).closest_point( ml7::Vector2( -9.0f, -45.0f ) ), ml7::Vector2( -1.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).closest_point( ml7::Vector2( -1.0f, -1.0f ) ), ml7::Vector2( -1.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).closest_point( ml7::Vector2( 0.0f, 0.0f ) ), ml7::Vector2( 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).closest_point( ml7::Vector2( 1.0f, 1.0f ) ), ml7::Vector2( 1.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).closest_point( ml7::Vector2( 9.0f, 45.0f ) ), ml7::Vector2( 1.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).closest_point( ml7::Vector2( -10.0f, -44.0f ) ), ml7::Vector2( -1.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).closest_point( ml7::Vector2( -1.75f, -0.75f ) ), ml7::Vector2( -1.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).closest_point( ml7::Vector2( 0.1f, 0.1f ) ), 4 ), ml7::Vector2( 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).closest_point( ml7::Vector2( 0.5f, 1.25f ) ), ml7::Vector2( 1.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).closest_point( ml7::Vector2( 8.0f, 46.0f ) ), ml7::Vector2( 1.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).closest_point( ml7::Vector2( -8.0f, -46.0f ) ), ml7::Vector2( -1.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).closest_point( ml7::Vector2( -0.5f, -1.25f ) ), ml7::Vector2( -1.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).closest_point( ml7::Vector2( -0.1f, -0.1f ) ), 4 ), ml7::Vector2( 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).closest_point( ml7::Vector2( 1.75f, 0.75f ) ), ml7::Vector2( 1.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).closest_point( ml7::Vector2( 10.0f, 44.0f ) ), ml7::Vector2( 1.0f, 1.0f ) );

    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance( ml7::Vector2( -9.0f, -45.0f ) ), 44.72136f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance( ml7::Vector2( -1.0f, -1.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance( ml7::Vector2( 0.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance( ml7::Vector2( 1.0f, 1.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance( ml7::Vector2( 9.0f, 45.0f ) ), 44.72136f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance( ml7::Vector2( -10.0f, -44.0f ) ), 43.93177f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance( ml7::Vector2( -1.75f, -0.75f ) ), 0.7905694f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance( ml7::Vector2( 0.1f, 0.1f ) ), 0.1414214f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance( ml7::Vector2( 0.5f, 1.25f ) ), 0.559017f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance( ml7::Vector2( 8.0f, 46.0f ) ), 45.54119f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance( ml7::Vector2( -8.0f, -46.0f ) ), 45.54119f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance( ml7::Vector2( -0.5f, -1.25f ) ), 0.559017f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance( ml7::Vector2( -0.1f, -0.1f ) ), 0.1414214f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance( ml7::Vector2( 1.75f, 0.75f ) ), 0.7905694f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance( ml7::Vector2( 10.0f, 44.0f ) ), 43.93177f );

    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance_squared( ml7::Vector2( -9.0f, -45.0f ) ), 2000.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance_squared( ml7::Vector2( -1.0f, -1.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance_squared( ml7::Vector2( 0.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance_squared( ml7::Vector2( 1.0f, 1.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance_squared( ml7::Vector2( 9.0f, 45.0f ) ), 2000.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance_squared( ml7::Vector2( -10.0f, -44.0f ) ), 1930.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance_squared( ml7::Vector2( -1.75f, -0.75f ) ), 0.625f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance_squared( ml7::Vector2( 0.1f, 0.1f ) ), 0.02f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance_squared( ml7::Vector2( 0.5f, 1.25f ) ), 0.3125f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance_squared( ml7::Vector2( 8.0f, 46.0f ) ), 2074.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance_squared( ml7::Vector2( -8.0f, -46.0f ) ), 2074.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance_squared( ml7::Vector2( -0.5f, -1.25f ) ), 0.3125f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance_squared( ml7::Vector2( -0.1f, -0.1f ) ), 0.02f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance_squared( ml7::Vector2( 1.75f, 0.75f ) ), 0.625f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).distance_squared( ml7::Vector2( 10.0f, 44.0f ) ), 1930.0f );

    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).subdivide( 0.0f ).first, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( -1.0f, -1.0f ) ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).subdivide( 0.0f ).second, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).subdivide( 0.5f ).first, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( -0.5f, 0.5f ), ml7::Vector2( -0.25f, 0.25f ), ml7::Vector2( 0.0f, 0.0f ) ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).subdivide( 0.5f ).second, ml7::CubicBezier2( ml7::Vector2( 0.0f, 0.0f ), ml7::Vector2( 0.25f, -0.25f ), ml7::Vector2( 0.5f, -0.5f ), ml7::Vector2( 1.0f, 1.0f ) ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).subdivide( 1.0f ).first, ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier2( ml7::Vector2( -1.0f, -1.0f ), ml7::Vector2( 0.0f, 2.0f ), ml7::Vector2( 0.0f, -2.0f ), ml7::Vector2( 1.0f, 1.0f ) ).subdivide( 1.0f ).second, ml7::CubicBezier2( ml7::Vector2( 1.0f, 1.0f ), ml7::Vector2( 1.0f, 1.0f ), ml7::Vector2( 1.0f, 1.0f ), ml7::Vector2( 1.0f, 1.0f ) ) );

    TESTLABS_CHECK( ml7::CubicBezier2( ml7::Vector2( 1.0f, -2.0f ), ml7::Vector2( -3.0f, 4.0f ), ml7::Vector2( 5.0f, -6.0f ), ml7::Vector2( -7.0f, 8.0f ) ) == ml7::CubicBezier2( ml7::Vector2( 1.0f, -2.0f ), ml7::Vector2( -3.0f, 4.0f ), ml7::Vector2( 5.0f, -6.0f ), ml7::Vector2( -7.0f, 8.0f ) ) );
    TESTLABS_CHECK( !(ml7::CubicBezier2( ml7::Vector2( 1.0f, -2.0f ), ml7::Vector2( -3.0f, 4.0f ), ml7::Vector2( 5.0f, -6.0f ), ml7::Vector2( -7.0f, 8.0f ) ) == ml7::CubicBezier2( ml7::Vector2( -1.0f, 2.0f ), ml7::Vector2( 3.0f, -4.0f ), ml7::Vector2( -5.0f, 6.0f ), ml7::Vector2( 7.0f, -8.0f ) )) );

    TESTLABS_CHECK( ml7::CubicBezier2( ml7::Vector2( 1.0f, -2.0f ), ml7::Vector2( -3.0f, 4.0f ), ml7::Vector2( 5.0f, -6.0f ), ml7::Vector2( -7.0f, 8.0f ) ) != ml7::CubicBezier2( ml7::Vector2( -1.0f, 2.0f ), ml7::Vector2( 3.0f, -4.0f ), ml7::Vector2( -5.0f, 6.0f ), ml7::Vector2( 7.0f, -8.0f ) ) );
    TESTLABS_CHECK( !(ml7::CubicBezier2( ml7::Vector2( 1.0f, -2.0f ), ml7::Vector2( -3.0f, 4.0f ), ml7::Vector2( 5.0f, -6.0f ), ml7::Vector2( -7.0f, 8.0f ) ) != ml7::CubicBezier2( ml7::Vector2( 1.0f, -2.0f ), ml7::Vector2( -3.0f, 4.0f ), ml7::Vector2( 5.0f, -6.0f ), ml7::Vector2( -7.0f, 8.0f ) )) );
}



#endif // F77_TESTS_ML7_CUBICBEZIER2_H

#ifndef F77_TESTS_ML7_CUBICBEZIER3_H
#define F77_TESTS_ML7_CUBICBEZIER3_H

#include <MathLabs/CubicBezier3.h>

#include <TestLabs/TestSuite.h>

#include "./shared.h"



namespace tl7::internals {
    inline
    cl7::u8string to_string(const ml7::CubicBezier3f& bezier) { return u8"{ " + to_string(bezier.p0) + u8", " + to_string(bezier.p1) + u8", " + to_string(bezier.p2) + u8", " + to_string(bezier.p3) + u8" }"; }
}



TESTLABS_CASE( u8"CoreLabs:  CubicBezier3:  " )
{
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f(), ml7::CubicBezier3f( ml7::Vector3f( 0.0f, 0.0f, 0.0f ), ml7::Vector3f( 0.0f, 0.0f, 0.0f ), ml7::Vector3f( 0.0f, 0.0f, 0.0f ), ml7::Vector3f( 0.0f, 0.0f, 0.0f ) ) );

    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).control_points().size(), 4 );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).control_points()[0], ml7::Vector3f( -1.0f, -1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).control_points()[1], ml7::Vector3f( 0.0f, 2.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).control_points()[2], ml7::Vector3f( 0.0f, -2.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).control_points()[3], ml7::Vector3f( 1.0f, 1.0f, 0.0f ) );

    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).point_at( -1.0f ), ml7::Vector3f( -9.0f, -45.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).point_at( 0.0f ), ml7::Vector3f( -1.0f, -1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).point_at( 0.5f ), ml7::Vector3f( 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).point_at( 1.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).point_at( 2.0f ), ml7::Vector3f( 9.0f, 45.0f, 0.0f ) );

    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).tangent_at( -1.0f ), ml7::Vector3f( 15.0f, 93.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).tangent_at( 0.0f ), ml7::Vector3f( 3.0f, 9.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).tangent_at( 0.5f ), ml7::Vector3f( 1.5f, -1.5f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).tangent_at( 1.0f ), ml7::Vector3f( 3.0f, 9.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).tangent_at( 2.0f ), ml7::Vector3f( 15.0f, 93.0f, 0.0f ) );

    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).second_derivative_at( -1.0f ), ml7::Vector3f( -18.0f, -126.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).second_derivative_at( 0.0f ), ml7::Vector3f( -6.0f, -42.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).second_derivative_at( 0.5f ), ml7::Vector3f( 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).second_derivative_at( 1.0f ), ml7::Vector3f( 6.0f, 42.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).second_derivative_at( 2.0f ), ml7::Vector3f( 18.0f, 126.0f, 0.0f ) );

    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( -9.0f, -45.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( -1.0f, -1.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( 0.0f, 0.0f, 0.0f ) ), 0.5f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( 9.0f, 45.0f, 0.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( -10.0f, -44.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( -1.75f, -0.75f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 4, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( 0.1f, 0.1f, 0.0f ) ), 0.5f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( 0.5f, 1.25f, 0.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( 8.0f, 46.0f, 0.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( -8.0f, -46.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( -0.5f, -1.25f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 4, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( -0.1f, -0.1f, 0.0f ) ), 0.5f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( 1.75f, 0.75f, 0.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( 10.0f, 44.0f, 0.0f ) ), 1.0f );

    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( -9.0f, -45.0f, 0.0f ) ), ml7::Vector3f( -1.0f, -1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( -1.0f, -1.0f, 0.0f ) ), ml7::Vector3f( -1.0f, -1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( 0.0f, 0.0f, 0.0f ) ), ml7::Vector3f( 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( 9.0f, 45.0f, 0.0f ) ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( -10.0f, -44.0f, 0.0f ) ), ml7::Vector3f( -1.0f, -1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( -1.75f, -0.75f, 0.0f ) ), ml7::Vector3f( -1.0f, -1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( 0.1f, 0.1f, 0.0f ) ), 4 ), ml7::Vector3f( 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( 0.5f, 1.25f, 0.0f ) ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( 8.0f, 46.0f, 0.0f ) ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( -8.0f, -46.0f, 0.0f ) ), ml7::Vector3f( -1.0f, -1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( -0.5f, -1.25f, 0.0f ) ), ml7::Vector3f( -1.0f, -1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( -0.1f, -0.1f, 0.0f ) ), 4 ), ml7::Vector3f( 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( 1.75f, 0.75f, 0.0f ) ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( 10.0f, 44.0f, 0.0f ) ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) );

    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( -9.0f, -45.0f, 0.0f ) ), 44.72136f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( -1.0f, -1.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( 0.0f, 0.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( 9.0f, 45.0f, 0.0f ) ), 44.72136f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( -10.0f, -44.0f, 0.0f ) ), 43.93177f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( -1.75f, -0.75f, 0.0f ) ), 0.7905694f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( 0.1f, 0.1f, 0.0f ) ), 0.1414214f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( 0.5f, 1.25f, 0.0f ) ), 0.559017f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( 8.0f, 46.0f, 0.0f ) ), 45.54119f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( -8.0f, -46.0f, 0.0f ) ), 45.54119f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( -0.5f, -1.25f, 0.0f ) ), 0.559017f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( -0.1f, -0.1f, 0.0f ) ), 0.1414214f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( 1.75f, 0.75f, 0.0f ) ), 0.7905694f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( 10.0f, 44.0f, 0.0f ) ), 43.93177f );

    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( -9.0f, -45.0f, 0.0f ) ), 2000.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( -1.0f, -1.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( 0.0f, 0.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( 9.0f, 45.0f, 0.0f ) ), 2000.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( -10.0f, -44.0f, 0.0f ) ), 1930.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( -1.75f, -0.75f, 0.0f ) ), 0.625f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( 0.1f, 0.1f, 0.0f ) ), 0.02f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( 0.5f, 1.25f, 0.0f ) ), 0.3125f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( 8.0f, 46.0f, 0.0f ) ), 2074.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( -8.0f, -46.0f, 0.0f ) ), 2074.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( -0.5f, -1.25f, 0.0f ) ), 0.3125f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( -0.1f, -0.1f, 0.0f ) ), 0.02f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( 1.75f, 0.75f, 0.0f ) ), 0.625f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( 10.0f, 44.0f, 0.0f ) ), 1930.0f );

    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).subdivide( 0.0f ).first, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( -1.0f, -1.0f, 0.0f ) ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).subdivide( 0.0f ).second, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).subdivide( 0.5f ).first, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( -0.5f, 0.5f, 0.0f ), ml7::Vector3f( -0.25f, 0.25f, 0.0f ), ml7::Vector3f( 0.0f, 0.0f, 0.0f ) ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).subdivide( 0.5f ).second, ml7::CubicBezier3f( ml7::Vector3f( 0.0f, 0.0f, 0.0f ), ml7::Vector3f( 0.25f, -0.25f, 0.0f ), ml7::Vector3f( 0.5f, -0.5f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).subdivide( 1.0f ).first, ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ) );
    TESTLABS_CHECK_EQ( ml7::CubicBezier3f( ml7::Vector3f( -1.0f, -1.0f, 0.0f ), ml7::Vector3f( 0.0f, 2.0f, 0.0f ), ml7::Vector3f( 0.0f, -2.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).subdivide( 1.0f ).second, ml7::CubicBezier3f( ml7::Vector3f( 1.0f, 1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ) );

    TESTLABS_CHECK( ml7::CubicBezier3f( ml7::Vector3f( 1.0f, -2.0f, 0.0f ), ml7::Vector3f( -3.0f, 4.0f, 0.0f ), ml7::Vector3f( 5.0f, -6.0f, 0.0f ), ml7::Vector3f( -7.0f, 8.0f, 0.0f ) ) == ml7::CubicBezier3f( ml7::Vector3f( 1.0f, -2.0f, 0.0f ), ml7::Vector3f( -3.0f, 4.0f, 0.0f ), ml7::Vector3f( 5.0f, -6.0f, 0.0f ), ml7::Vector3f( -7.0f, 8.0f, 0.0f ) ) );
    TESTLABS_CHECK( !(ml7::CubicBezier3f( ml7::Vector3f( 1.0f, -2.0f, 0.0f ), ml7::Vector3f( -3.0f, 4.0f, 0.0f ), ml7::Vector3f( 5.0f, -6.0f, 0.0f ), ml7::Vector3f( -7.0f, 8.0f, 0.0f ) ) == ml7::CubicBezier3f( ml7::Vector3f( -1.0f, 2.0f, 0.0f ), ml7::Vector3f( 3.0f, -4.0f, 0.0f ), ml7::Vector3f( -5.0f, 6.0f, 0.0f ), ml7::Vector3f( 7.0f, -8.0f, 0.0f ) )) );

    TESTLABS_CHECK( ml7::CubicBezier3f( ml7::Vector3f( 1.0f, -2.0f, 0.0f ), ml7::Vector3f( -3.0f, 4.0f, 0.0f ), ml7::Vector3f( 5.0f, -6.0f, 0.0f ), ml7::Vector3f( -7.0f, 8.0f, 0.0f ) ) != ml7::CubicBezier3f( ml7::Vector3f( -1.0f, 2.0f, 0.0f ), ml7::Vector3f( 3.0f, -4.0f, 0.0f ), ml7::Vector3f( -5.0f, 6.0f, 0.0f ), ml7::Vector3f( 7.0f, -8.0f, 0.0f ) ) );
    TESTLABS_CHECK( !(ml7::CubicBezier3f( ml7::Vector3f( 1.0f, -2.0f, 0.0f ), ml7::Vector3f( -3.0f, 4.0f, 0.0f ), ml7::Vector3f( 5.0f, -6.0f, 0.0f ), ml7::Vector3f( -7.0f, 8.0f, 0.0f ) ) != ml7::CubicBezier3f( ml7::Vector3f( 1.0f, -2.0f, 0.0f ), ml7::Vector3f( -3.0f, 4.0f, 0.0f ), ml7::Vector3f( 5.0f, -6.0f, 0.0f ), ml7::Vector3f( -7.0f, 8.0f, 0.0f ) )) );
}



#endif // F77_TESTS_ML7_CUBICBEZIER3_H

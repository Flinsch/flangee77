#ifndef F77_TESTS_ML7_QUADRATICBEZIER3_H
#define F77_TESTS_ML7_QUADRATICBEZIER3_H

#include <MathLabs/QuadraticBezier3.h>

#include <TestLabs/TestSuite.h>

#include "./shared.h"



namespace tl7::internals {
    inline
    cl7::u8string to_string(const ml7::QuadraticBezier3f& bezier) { return u8"{ " + to_string(bezier.p0) + u8", " + to_string(bezier.p1) + u8", " + to_string(bezier.p2) + u8" }"; }
}



TESTLABS_CASE( u8"CoreLabs:  QuadraticBezier3:  " )
{
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f(), ml7::QuadraticBezier3f( ml7::Vector3f( 0.0f, 0.0f, 0.0f ), ml7::Vector3f( 0.0f, 0.0f, 0.0f ), ml7::Vector3f( 0.0f, 0.0f, 0.0f ) ) );

    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).control_points().size(), 3 );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).control_points()[0], ml7::Vector3f( -1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).control_points()[1], ml7::Vector3f( 0.0f, -1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).control_points()[2], ml7::Vector3f( 1.0f, 1.0f, 0.0f ) );

    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).point_at( -1.0f ), ml7::Vector3f( -3.0f, 9.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).point_at( 0.0f ), ml7::Vector3f( -1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).point_at( 0.5f ), ml7::Vector3f( 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).point_at( 1.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).point_at( 2.0f ), ml7::Vector3f( 3.0f, 9.0f, 0.0f ) );

    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).tangent_at( -1.0f ), ml7::Vector3f( 2.0f, -12.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).tangent_at( 0.0f ), ml7::Vector3f( 2.0f, -4.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).tangent_at( 0.5f ), ml7::Vector3f( 2.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).tangent_at( 1.0f ), ml7::Vector3f( 2.0f, 4.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).tangent_at( 2.0f ), ml7::Vector3f( 2.0f, 12.0f, 0.0f ) );

    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).second_derivative_at( -1.0f ), ml7::Vector3f( 0.0f, 8.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).second_derivative_at( 0.0f ), ml7::Vector3f( 0.0f, 8.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).second_derivative_at( 0.5f ), ml7::Vector3f( 0.0f, 8.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).second_derivative_at( 1.0f ), ml7::Vector3f( 0.0f, 8.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).second_derivative_at( 2.0f ), ml7::Vector3f( 0.0f, 8.0f, 0.0f ) );

    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( -3.0f, 9.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( -1.0f, 1.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( 0.0f, 0.0f, 0.0f ) ), 0.5f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( 3.0f, 9.0f, 0.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( -1.0f, 9.333333f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 3, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( -0.3333333f, 1.333333f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 3, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( 0.0f, 0.25f, 0.0f ) ), 0.5f );
    TESTLABS_CHECK_EQ_ROUND( 3, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( 0.3333333f, 1.333333f, 0.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( 1.0f, 9.333333f, 0.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( -9.0f, 8.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( -3.0f, 0.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( 0.0f, -1.0f, 0.0f ) ), 0.5f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( 3.0f, 0.0f, 0.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).parameter( ml7::Vector3f( 9.0f, 8.0f, 0.0f ) ), 1.0f );

    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( -3.0f, 9.0f, 0.0f ) ), ml7::Vector3f( -1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( -1.0f, 1.0f, 0.0f ) ), ml7::Vector3f( -1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( 0.0f, 0.0f, 0.0f ) ), ml7::Vector3f( 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( 3.0f, 9.0f, 0.0f ) ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( -1.0f, 9.333333f, 0.0f ) ), ml7::Vector3f( -1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( -0.3333333f, 1.333333f, 0.0f ) ), 2 ), ml7::Vector3f( -1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( 0.0f, 0.25f, 0.0f ) ), 2 ), ml7::Vector3f( 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( 0.3333333f, 1.333333f, 0.0f ) ), 2 ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( 1.0f, 9.333333f, 0.0f ) ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( -9.0f, 8.0f, 0.0f ) ), ml7::Vector3f( -1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( -3.0f, 0.0f, 0.0f ) ), ml7::Vector3f( -1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( 0.0f, -1.0f, 0.0f ) ), ml7::Vector3f( 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( 3.0f, 0.0f, 0.0f ) ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).closest_point( ml7::Vector3f( 9.0f, 8.0f, 0.0f ) ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) );

    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( -3.0f, 9.0f, 0.0f ) ), 8.246211f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( -1.0f, 1.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( 0.0f, 0.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( 3.0f, 9.0f, 0.0f ) ), 8.246211f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( -1.0f, 9.333333f, 0.0f ) ), 8.333333f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( -0.3333333f, 1.333333f, 0.0f ) ), 0.7453561f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( 0.0f, 0.25f, 0.0f ) ), 0.25f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( 0.3333333f, 1.333333f, 0.0f ) ), 0.7453561f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( 1.0f, 9.333333f, 0.0f ) ), 8.333333f );
    TESTLABS_CHECK_EQ_ROUND( 3, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( -9.0f, 8.0f, 0.0f ) ), 10.63015f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( -3.0f, 0.0f, 0.0f ) ), 2.236068f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( 0.0f, -1.0f, 0.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( 3.0f, 0.0f, 0.0f ) ), 2.236068f );
    TESTLABS_CHECK_EQ_ROUND( 3, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance( ml7::Vector3f( 9.0f, 8.0f, 0.0f ) ), 10.63015f );

    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( -3.0f, 9.0f, 0.0f ) ), 68.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( -1.0f, 1.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( 0.0f, 0.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( 3.0f, 9.0f, 0.0f ) ), 68.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( -1.0f, 9.333333f, 0.0f ) ), 69.44444f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( -0.3333333f, 1.333333f, 0.0f ) ), 0.5555555f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( 0.0f, 0.25f, 0.0f ) ), 0.0625f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( 0.3333333f, 1.333333f, 0.0f ) ), 0.5555555f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( 1.0f, 9.333333f, 0.0f ) ), 69.444444f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( -9.0f, 8.0f, 0.0f ) ), 113.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( -3.0f, 0.0f, 0.0f ) ), 5.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( 0.0f, -1.0f, 0.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( 3.0f, 0.0f, 0.0f ) ), 5.0f );
    TESTLABS_CHECK_EQ_ROUND( 5, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).distance_squared( ml7::Vector3f( 9.0f, 8.0f, 0.0f ) ), 113.0f );

    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).subdivide( 0.0f ).first, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( -1.0f, 1.0f, 0.0f ) ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).subdivide( 0.0f ).second, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).subdivide( 0.5f ).first, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( -0.5f, 0.0f, 0.0f ), ml7::Vector3f( 0.0f, 0.0f, 0.0f ) ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).subdivide( 0.5f ).second, ml7::QuadraticBezier3f( ml7::Vector3f( 0.0f, 0.0f, 0.0f ), ml7::Vector3f( 0.5f, 0.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).subdivide( 1.0f ).first, ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ) );
    TESTLABS_CHECK_EQ( ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ).subdivide( 1.0f ).second, ml7::QuadraticBezier3f( ml7::Vector3f( 1.0f, 1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ), ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ) );

    TESTLABS_CHECK( ml7::QuadraticBezier3f( ml7::Vector3f( 1.0f, -2.0f, 0.0f ), ml7::Vector3f( -3.0f, 4.0f, 0.0f ), ml7::Vector3f( 5.0f, -6.0f, 0.0f ) ) == ml7::QuadraticBezier3f( ml7::Vector3f( 1.0f, -2.0f, 0.0f ), ml7::Vector3f( -3.0f, 4.0f, 0.0f ), ml7::Vector3f( 5.0f, -6.0f, 0.0f ) ) );
    TESTLABS_CHECK( !(ml7::QuadraticBezier3f( ml7::Vector3f( 1.0f, -2.0f, 0.0f ), ml7::Vector3f( -3.0f, 4.0f, 0.0f ), ml7::Vector3f( 5.0f, -6.0f, 0.0f ) ) == ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 2.0f, 0.0f ), ml7::Vector3f( 3.0f, -4.0f, 0.0f ), ml7::Vector3f( -5.0f, 6.0f, 0.0f ) )) );

    TESTLABS_CHECK( ml7::QuadraticBezier3f( ml7::Vector3f( 1.0f, -2.0f, 0.0f ), ml7::Vector3f( -3.0f, 4.0f, 0.0f ), ml7::Vector3f( 5.0f, -6.0f, 0.0f ) ) != ml7::QuadraticBezier3f( ml7::Vector3f( -1.0f, 2.0f, 0.0f ), ml7::Vector3f( 3.0f, -4.0f, 0.0f ), ml7::Vector3f( -5.0f, 6.0f, 0.0f ) ) );
    TESTLABS_CHECK( !(ml7::QuadraticBezier3f( ml7::Vector3f( 1.0f, -2.0f, 0.0f ), ml7::Vector3f( -3.0f, 4.0f, 0.0f ), ml7::Vector3f( 5.0f, -6.0f, 0.0f ) ) != ml7::QuadraticBezier3f( ml7::Vector3f( 1.0f, -2.0f, 0.0f ), ml7::Vector3f( -3.0f, 4.0f, 0.0f ), ml7::Vector3f( 5.0f, -6.0f, 0.0f ) )) );
}



#endif // F77_TESTS_ML7_QUADRATICBEZIER3_H

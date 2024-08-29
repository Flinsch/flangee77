#pragma once
#ifndef F77_TESTS_ML7_MATRIX2X3_H
#define F77_TESTS_ML7_MATRIX2X3_H

#include <MathLabs/Matrix2x3.h>

#include <MathLabs/constants.h>
#include <MathLabs/utilities.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



namespace cl7 {
    template <> inline
    cl7::string to_string(const ml7::Matrix2x3& m) { return TEXT("{ ") + cl7::to_string(m.a) + TEXT(", ") + cl7::to_string(m.b) + TEXT(", ") + cl7::to_string(m.c) + TEXT("; ") + cl7::to_string(m.d) + TEXT(", ") + cl7::to_string(m.e) + TEXT(", ") + cl7::to_string(m.f) + TEXT(" }"); }
}



namespace _ {
    ml7::Matrix2x3 round(ml7::Matrix2x3 m, unsigned num_decimals) { for ( unsigned k = 0; k < 6; ++k ) m.data[k] = ml7::utilities::round(m.data[k], num_decimals); return m; }
}



TESTLABS_CASE( TEXT("CoreLabs:  Matrix2x3:  ") )
{
    TESTLABS_CHECK_EQ( ml7::Matrix2x3(), ml7::Matrix2x3( 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ) ), ml7::Matrix2x3( 1.0f, 2.0f, 0.0f, 3.0f, 4.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ), ml7::Vector2( 5.0f, 6.0f ) ), ml7::Matrix2x3( 1.0f, 2.0f, 5.0f, 3.0f, 4.0f, 6.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3::from_axes( ml7::Vector2( 0.0f, 1.0f ), ml7::Vector2( -1.0f, 0.0f ) ), ml7::Matrix2x3( 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3::from_axes( ml7::Vector2( 0.0f, 1.0f ), ml7::Vector2( -1.0f, 0.0f ) ).transform( ml7::Vector2( 1.0f, 1.0f ) ), ml7::Vector2( -1.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3::from_axes( ml7::Vector2( 0.0f, 1.0f ), ml7::Vector2( -1.0f, 0.0f ), ml7::Vector2( 2.0f, 3.0f ) ), ml7::Matrix2x3( 0.0f, -1.0f, 2.0f, 1.0f, 0.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3::from_axes( ml7::Vector2( 0.0f, 1.0f ), ml7::Vector2( -1.0f, 0.0f ), ml7::Vector2( 2.0f, 3.0f ) ).transform( ml7::Vector2( 1.0f, 1.0f ) ), ml7::Vector2( 1.0f, 4.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3::scaling( -1.0f ), ml7::Matrix2x3( -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3::scaling( ml7::Vector2( 0.5f, 2.0f ) ), ml7::Matrix2x3( 0.5f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f ) );

    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::rotation( 0.0f/4.0f * ml7::constants::pi2 ), 3 ), _::round( ml7::Matrix2x3( 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::rotation( 1.0f/4.0f * ml7::constants::pi2 ), 3 ), _::round( ml7::Matrix2x3( 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::rotation( 2.0f/4.0f * ml7::constants::pi2 ), 3 ), _::round( ml7::Matrix2x3( -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::rotation( 3.0f/4.0f * ml7::constants::pi2 ), 3 ), _::round( ml7::Matrix2x3( 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3::translation( ml7::Vector2( 5.0f, 6.0f ) ), ml7::Matrix2x3( 1.0f, 0.0f, 5.0f, 0.0f, 1.0f, 6.0f ) );

    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( -1.0f, 0.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, 6.0f ) ), 3 ), _::round( ml7::Matrix2x3( -1.0f, 0.0f, 5.0f, 0.0f, -1.0f, 6.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( -2.0f, 1.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, 6.0f ) ), 3 ), _::round( ml7::Matrix2x3( 0.0f, 2.0f, 5.0f, -2.0f, 0.0f, 6.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( -3.0f, -2.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, 6.0f ) ), 3 ), _::round( ml7::Matrix2x3( 3.0f, 0.0f, 5.0f, 0.0f, 3.0f, 6.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( -4.0f, -1.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, 6.0f ) ), 3 ), _::round( ml7::Matrix2x3( 0.0f, -4.0f, 5.0f, 4.0f, 0.0f, 6.0f ), 3 ) );

    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 1.0f, 2.0f ), 0.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, 6.0f ) ), 3 ), _::round( ml7::Matrix2x3( 1.0f, 0.0f, 5.0f, 0.0f, 2.0f, 6.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 3.0f, 4.0f ), 1.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, 6.0f ) ), 3 ), _::round( ml7::Matrix2x3( 0.0f, -4.0f, 5.0f, 3.0f, 0.0f, 6.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 5.0f, 6.0f ), -2.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, 6.0f ) ), 3 ), _::round( ml7::Matrix2x3( -5.0f, 0.0f, 5.0f, 0.0f, -6.0f, 6.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 7.0f, 8.0f ), -1.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, 6.0f ) ), 3 ), _::round( ml7::Matrix2x3( 0.0f, 8.0f, 5.0f, -7.0f, 0.0f, 6.0f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 5.0f, 3.0f, 4.0f, 6.0f ).determinant(), -2.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 2.0f, 3.0f, 5.0f, 4.0f, 1.0f, 6.0f ).determinant(), -10.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 3.0f, 4.0f, 5.0f, 1.0f, 2.0f, 6.0f ).determinant(), 2.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 4.0f, 1.0f, 5.0f, 2.0f, 3.0f, 6.0f ).determinant(), 10.0f );

    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f ).inverted(), 3 ), _::round( ml7::Matrix2x3( -5.0f/3.0f, 2.0f/3.0f, 1.0f, 4.0f/3.0f, -1.0f/3.0f, -2.0f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f ).get_column( 0 ), ml7::Vector2( 1.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f ).get_column( 1 ), ml7::Vector2( 2.0f, 5.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f ).get_column( 2 ), ml7::Vector2( 3.0f, 6.0f ) );

    ml7::Vector2 x, y, origin;
    ml7::Matrix2x3( 1.0f, 2.0f, 5.0f, 3.0f, 4.0f, 6.0f ).to_axes( x, y );
    TESTLABS_CHECK_EQ( x, ml7::Vector2( 1.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( y, ml7::Vector2( 2.0f, 4.0f ) );
    ml7::Matrix2x3( 1.0f, 2.0f, 5.0f, 3.0f, 4.0f, 6.0f ).to_axes( x, y, origin );
    TESTLABS_CHECK_EQ( x, ml7::Vector2( 1.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( y, ml7::Vector2( 2.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( origin, ml7::Vector2( 5.0f, 6.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 5.0f, 3.0f, 4.0f, 6.0f ).to_matrix2x2(), ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ) );

    ml7::Matrix2x2 m2x2;
    ml7::Vector2 translation;
    ml7::Matrix2x3( 1.0f, 2.0f, 5.0f, 3.0f, 4.0f, 6.0f ).decompose( m2x2, translation );
    TESTLABS_CHECK_EQ( m2x2, ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( translation, ml7::Vector2( 5.0f, 6.0f ) );

    ml7::Vector2 scaling;
    float theta;
    ml7::Matrix2x3( 1.0f, 0.0f, 5.0f, 0.0f, 2.0f, 6.0f ).decompose( scaling, theta, translation );
    TESTLABS_CHECK_EQ_FLT( scaling.x, 1.0f );
    TESTLABS_CHECK_EQ_FLT( scaling.y, 2.0f );
    TESTLABS_CHECK_EQ_FLT( theta, 0.0f/4.0f * ml7::constants::pi2 );
    TESTLABS_CHECK_EQ( translation, ml7::Vector2( 5.0f, 6.0f ) );
    ml7::Matrix2x3( 0.0f, -4.0f, 7.0f, 3.0f, 0.0f, 8.0f ).decompose( scaling, theta, translation );
    TESTLABS_CHECK_EQ_FLT( scaling.x, 3.0f );
    TESTLABS_CHECK_EQ_FLT( scaling.y, 4.0f );
    TESTLABS_CHECK_EQ_FLT( theta, 1.0f/4.0f * ml7::constants::pi2 );
    TESTLABS_CHECK_EQ( translation, ml7::Vector2( 7.0f, 8.0f ) );
    ml7::Matrix2x3( -5.0f, 0.0f, 9.0f, 0.0f, -6.0f, 10.0f ).decompose( scaling, theta, translation );
    TESTLABS_CHECK_EQ_FLT( scaling.x, 5.0f );
    TESTLABS_CHECK_EQ_FLT( scaling.y, 6.0f );
    TESTLABS_CHECK_EQ_FLT( theta, -2.0f/4.0f * ml7::constants::pi2 );
    TESTLABS_CHECK_EQ( translation, ml7::Vector2( 9.0f, 10.0f ) );
    ml7::Matrix2x3( 0.0f, 8.0f, 11.0f, -7.0f, 0.0f, 12.0f ).decompose( scaling, theta, translation );
    TESTLABS_CHECK_EQ_FLT( scaling.x, 7.0f );
    TESTLABS_CHECK_EQ_FLT( scaling.y, 8.0f );
    TESTLABS_CHECK_EQ_FLT( theta, -1.0f/4.0f * ml7::constants::pi2 );
    TESTLABS_CHECK_EQ( translation, ml7::Vector2( 11.0f, 12.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, -2.0f, 5.0f, 3.0f, -4.0f, -6.0f ).transform( ml7::Vector2( -5.0f, 6.0f ) ), ml7::Vector2( -12.0f, -45.0f ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 1.0f, 2.0f ), 0.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, -6.0f ) ).transform( ml7::Vector2( 1.0f, 2.0f ) ), 3 ), _::round( ml7::Vector2( 6.0f, -2.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 3.0f, 4.0f ), 1.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, -6.0f ) ).transform( ml7::Vector2( 1.0f, 2.0f ) ), 3 ), _::round( ml7::Vector2( -3.0f, -3.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 5.0f, 6.0f ), -2.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, -6.0f ) ).transform( ml7::Vector2( 1.0f, 2.0f ) ), 3 ), _::round( ml7::Vector2( 0.0f, -18.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 7.0f, 8.0f ), -1.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, -6.0f ) ).transform( ml7::Vector2( 1.0f, 2.0f ) ), 3 ), _::round( ml7::Vector2( 21.0f, -13.0f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, -2.0f, 5.0f, 3.0f, -4.0f, -6.0f ).transform2x2( ml7::Vector2( -5.0f, 6.0f ) ), ml7::Vector2( -17.0f, -39.0f ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 1.0f, 2.0f ), 0.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, -6.0f ) ).transform2x2( ml7::Vector2( 1.0f, 2.0f ) ), 3 ), _::round( ml7::Vector2( 1.0f, 4.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 3.0f, 4.0f ), 1.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, -6.0f ) ).transform2x2( ml7::Vector2( 1.0f, 2.0f ) ), 3 ), _::round( ml7::Vector2( -8.0f, 3.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 5.0f, 6.0f ), -2.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, -6.0f ) ).transform2x2( ml7::Vector2( 1.0f, 2.0f ) ), 3 ), _::round( ml7::Vector2( -5.0f, -12.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 7.0f, 8.0f ), -1.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, -6.0f ) ).transform2x2( ml7::Vector2( 1.0f, 2.0f ) ), 3 ), _::round( ml7::Vector2( 16.0f, -7.0f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, -2.0f, 5.0f, 3.0f, -4.0f, -6.0f ).transform_inverted( ml7::Vector2( -12.0f, -45.0f ) ), ml7::Vector2( -5.0f, 6.0f ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 1.0f, 2.0f ), 0.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, -6.0f ) ).transform_inverted( ml7::Vector2( 6.0f, -2.0f ) ), 3 ), _::round( ml7::Vector2( 1.0f, 2.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 3.0f, 4.0f ), 1.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, -6.0f ) ).transform_inverted( ml7::Vector2( -3.0f, -3.0f ) ), 3 ), _::round( ml7::Vector2( 1.0f, 2.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 5.0f, 6.0f ), -2.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, -6.0f ) ).transform_inverted( ml7::Vector2( 0.0f, -18.0f ) ), 3 ), _::round( ml7::Vector2( 1.0f, 2.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 7.0f, 8.0f ), -1.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, -6.0f ) ).transform_inverted( ml7::Vector2( 21.0f, -13.0f ) ), 3 ), _::round( ml7::Vector2( 1.0f, 2.0f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, -2.0f, 5.0f, 3.0f, -4.0f, -6.0f ).transform2x2_inverted( ml7::Vector2( -17.0f, -39.0f ) ), ml7::Vector2( -5.0f, 6.0f ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 1.0f, 2.0f ), 0.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, -6.0f ) ).transform2x2_inverted( ml7::Vector2( 1.0f, 4.0f ) ), 3 ), _::round( ml7::Vector2( 1.0f, 2.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 3.0f, 4.0f ), 1.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, -6.0f ) ).transform2x2_inverted( ml7::Vector2( -8.0f, 3.0f ) ), 3 ), _::round( ml7::Vector2( 1.0f, 2.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 5.0f, 6.0f ), -2.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, -6.0f ) ).transform2x2_inverted( ml7::Vector2( -5.0f, -12.0f ) ), 3 ), _::round( ml7::Vector2( 1.0f, 2.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3::compose( ml7::Vector2( 7.0f, 8.0f ), -1.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 5.0f, -6.0f ) ).transform2x2_inverted( ml7::Vector2( 16.0f, -7.0f ) ), 3 ), _::round( ml7::Vector2( 1.0f, 2.0f ), 3 ) );

    TESTLABS_CHECK_EQ( _::round( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f ).invert(), 3 ), _::round( ml7::Matrix2x3( -5.0f/3.0f, 2.0f/3.0f, 1.0f, 4.0f/3.0f, -1.0f/3.0f, -2.0f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f ).set_column( 0, ml7::Vector2( 7.0f, 8.0f ) ), ml7::Matrix2x3( 7.0f, 2.0f, 3.0f, 8.0f, 5.0f, 6.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f ).set_column( 1, ml7::Vector2( 7.0f, 8.0f ) ), ml7::Matrix2x3( 1.0f, 7.0f, 3.0f, 4.0f, 8.0f, 6.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f ).set_column( 2, ml7::Vector2( 7.0f, 8.0f ) ), ml7::Matrix2x3( 1.0f, 2.0f, 7.0f, 4.0f, 5.0f, 8.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 0 ][ 0 ], 1.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 0 ][ 1 ], 2.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 0 ][ 2 ], 3.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 1 ][ 0 ], 4.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 1 ][ 1 ], 5.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 1 ][ 2 ], 6.0f );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 0 ][ 0 ] = 7.0f, 7.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 0 ][ 1 ] = 7.0f, 7.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 0 ][ 2 ] = 7.0f, 7.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 1 ][ 0 ] = 7.0f, 7.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 1 ][ 1 ] = 7.0f, 7.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 1 ][ 2 ] = 7.0f, 7.0f );

    TESTLABS_CHECK_EQ( +ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ), ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) );
    TESTLABS_CHECK_EQ( -ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ), ml7::Matrix2x3( -1.0f, 2.0f, -3.0f, 4.0f, -5.0f, 6.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) + ml7::Matrix2x3( -7.0f, 8.0f, -9.0f, 10.0f, -11.0f, 12.0f ), ml7::Matrix2x3( -6.0f, 6.0f, -6.0f, 6.0f, -6.0f, 6.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) - ml7::Matrix2x3( -7.0f, 8.0f, -9.0f, 10.0f, -11.0f, 12.0f ), ml7::Matrix2x3( 8.0f, -10.0f, 12.0f, -14.0f, 16.0f, -18.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) * 7.0f, ml7::Matrix2x3( 7.0f, -14.0f, 21.0f, -28.0f, 35.0f, -42.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 7.0f, -14.0f, 21.0f, -28.0f, 35.0f, -42.0f ) / 7.0f, ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) * ml7::Matrix2x3( -7.0f, 8.0f, -9.0f, 10.0f, -11.0f, 12.0f ), ml7::Matrix2x3( -27.0f, 30.0f, -30.0f, 78.0f, -87.0f, 90.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( -7.0f, 8.0f, -9.0f, 10.0f, -11.0f, 12.0f ) * ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ), ml7::Matrix2x3( -39.0f, 54.0f, -78.0f, 54.0f, -75.0f, 108.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, -2.0f, 7.0f, 3.0f, -4.0f, 8.0f ) * ml7::Vector2( -5.0f, 6.0f ), ml7::Vector2( -10.0f, -31.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) += ml7::Matrix2x3( -7.0f, 8.0f, -9.0f, 10.0f, -11.0f, 12.0f ), ml7::Matrix2x3( -6.0f, 6.0f, -6.0f, 6.0f, -6.0f, 6.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) -= ml7::Matrix2x3( -7.0f, 8.0f, -9.0f, 10.0f, -11.0f, 12.0f ), ml7::Matrix2x3( 8.0f, -10.0f, 12.0f, -14.0f, 16.0f, -18.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) *= 7.0f, ml7::Matrix2x3( 7.0f, -14.0f, 21.0f, -28.0f, 35.0f, -42.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3( 7.0f, -14.0f, 21.0f, -28.0f, 35.0f, -42.0f ) /= 7.0f, ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) );

    TESTLABS_CHECK( ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) == ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) );
    TESTLABS_CHECK( !(ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) == ml7::Matrix2x3( -1.0f, 2.0f, -3.0f, 4.0f, -5.0f, 6.0f )) );

    TESTLABS_CHECK( ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) != ml7::Matrix2x3( -1.0f, 2.0f, -3.0f, 4.0f, -5.0f, 6.0f ) );
    TESTLABS_CHECK( !(ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) != ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f )) );

    TESTLABS_CHECK_EQ( 7.0f * ml7::Matrix2x3( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) , ml7::Matrix2x3( 7.0f, -14.0f, 21.0f, -28.0f, 35.0f, -42.0f ) );
}



#endif // F77_TESTS_ML7_MATRIX2X3_H

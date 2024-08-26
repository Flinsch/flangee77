#pragma once
#ifndef F77_TESTS_ML7_MATRIX2X2_H
#define F77_TESTS_ML7_MATRIX2X2_H

#include <MathLabs/Matrix2x2.h>

#include <MathLabs/constants.h>
#include <MathLabs/utilities.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



namespace cl7 {
    template <> inline
    cl7::string to_string(const ml7::Matrix2x2& m) { return TEXT("{ ") + cl7::to_string(m.a) + TEXT(", ") + cl7::to_string(m.b) + TEXT("; ") + cl7::to_string(m.c) + TEXT(", ") + cl7::to_string(m.d) + TEXT(" }"); }
}



ml7::Matrix2x2 round(ml7::Matrix2x2 m, unsigned num_decimals) { for ( unsigned k = 0; k < 4; ++k ) m.data[k] = ml7::utilities::round(m.data[k], num_decimals); return m; }
ml7::Vector2 round(ml7::Vector2 v, unsigned num_decimals) { for ( unsigned k = 0; k < 2; ++k ) v.data[k] = ml7::utilities::round(v.data[k], num_decimals); return v; }



TESTLABS_CASE( TEXT("CoreLabs:  Matrix2x2:  ") )
{
    TESTLABS_CHECK_EQ( ml7::Matrix2x2(), ml7::Matrix2x2( 1.0f, 0.0f, 0.0f, 1.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2::from_axes( ml7::Vector2( 0.0f, 1.0f ), ml7::Vector2( -1.0f, 0.0f ) ), ml7::Matrix2x2( 0.0f, -1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2::from_axes( ml7::Vector2( 0.0f, 1.0f ), ml7::Vector2( -1.0f, 0.0f ) ).transform( ml7::Vector2( 1.0f, 1.0f ) ), ml7::Vector2( -1.0f, 1.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2::scaling( -1.0f ), ml7::Matrix2x2( -1.0f, 0.0f, 0.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2::scaling( ml7::Vector2( 0.5f, 2.0f ) ), ml7::Matrix2x2( 0.5f, 0.0f, 0.0f, 2.0f ) );

    TESTLABS_CHECK_EQ( round( ml7::Matrix2x2::rotation( 0.0f/4.0f * ml7::constants::pi2 ), 3 ), round( ml7::Matrix2x2( 1.0f, 0.0f, 0.0f, 1.0f ), 3 ) );
    TESTLABS_CHECK_EQ( round( ml7::Matrix2x2::rotation( 1.0f/4.0f * ml7::constants::pi2 ), 3 ), round( ml7::Matrix2x2( 0.0f, -1.0f, 1.0f, 0.0f ), 3 ) );
    TESTLABS_CHECK_EQ( round( ml7::Matrix2x2::rotation( 2.0f/4.0f * ml7::constants::pi2 ), 3 ), round( ml7::Matrix2x2( -1.0f, 0.0f, 0.0f, -1.0f ), 3 ) );
    TESTLABS_CHECK_EQ( round( ml7::Matrix2x2::rotation( 3.0f/4.0f * ml7::constants::pi2 ), 3 ), round( ml7::Matrix2x2( 0.0f, 1.0f, -1.0f, 0.0f ), 3 ) );

    TESTLABS_CHECK_EQ( round( ml7::Matrix2x2::compose( -1.0f, 0.0f/4.0f * ml7::constants::pi2 ), 3 ), round( ml7::Matrix2x2( -1.0f, 0.0f, 0.0f, -1.0f ), 3 ) );
    TESTLABS_CHECK_EQ( round( ml7::Matrix2x2::compose( -2.0f, 1.0f/4.0f * ml7::constants::pi2 ), 3 ), round( ml7::Matrix2x2( 0.0f, 2.0f, -2.0f, 0.0f ), 3 ) );
    TESTLABS_CHECK_EQ( round( ml7::Matrix2x2::compose( -3.0f, -2.0f/4.0f * ml7::constants::pi2 ), 3 ), round( ml7::Matrix2x2( 3.0f, 0.0f, 0.0f, 3.0f ), 3 ) );
    TESTLABS_CHECK_EQ( round( ml7::Matrix2x2::compose( -4.0f, -1.0f/4.0f * ml7::constants::pi2 ), 3 ), round( ml7::Matrix2x2( 0.0f, -4.0f, 4.0f, 0.0f ), 3 ) );

    TESTLABS_CHECK_EQ( round( ml7::Matrix2x2::compose( ml7::Vector2( 1.0f, 2.0f ), 0.0f/4.0f * ml7::constants::pi2 ), 3 ), round( ml7::Matrix2x2( 1.0f, 0.0f, 0.0f, 2.0f ), 3 ) );
    TESTLABS_CHECK_EQ( round( ml7::Matrix2x2::compose( ml7::Vector2( 3.0f, 4.0f ), 1.0f/4.0f * ml7::constants::pi2 ), 3 ), round( ml7::Matrix2x2( 0.0f, -4.0f, 3.0f, 0.0f ), 3 ) );
    TESTLABS_CHECK_EQ( round( ml7::Matrix2x2::compose( ml7::Vector2( 5.0f, 6.0f ), -2.0f/4.0f * ml7::constants::pi2 ), 3 ), round( ml7::Matrix2x2( -5.0f, 0.0f, 0.0f, -6.0f ), 3 ) );
    TESTLABS_CHECK_EQ( round( ml7::Matrix2x2::compose( ml7::Vector2( 7.0f, 8.0f ), -1.0f/4.0f * ml7::constants::pi2 ), 3 ), round( ml7::Matrix2x2( 0.0f, 8.0f, -7.0f, 0.0f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).determinant(), -2.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 2.0f, 3.0f, 4.0f, 1.0f ).determinant(), -10.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 3.0f, 4.0f, 1.0f, 2.0f ).determinant(), 2.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 4.0f, 1.0f, 2.0f, 3.0f ).determinant(), 10.0f );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).transposed(), ml7::Matrix2x2( 1.0f, 3.0f, 2.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).inverted(), ml7::Matrix2x2( -2.0f, 1.0f, 1.5f, -0.5f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).get_row( 0 ), ml7::Vector2( 1.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).get_row( 1 ), ml7::Vector2( 3.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).get_column( 0 ), ml7::Vector2( 1.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).get_column( 1 ), ml7::Vector2( 2.0f, 4.0f ) );

    ml7::Vector2 x, y;
    ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).to_axes( x, y );
    TESTLABS_CHECK_EQ( x, ml7::Vector2( 1.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( y, ml7::Vector2( 2.0f, 4.0f ) );

    ml7::Vector2 scaling;
    float theta;
    ml7::Matrix2x2( 1.0f, 0.0f, 0.0f, 2.0f ).decompose( scaling, theta );
    TESTLABS_CHECK_EQ_FLT( scaling.x, 1.0f );
    TESTLABS_CHECK_EQ_FLT( scaling.y, 2.0f );
    TESTLABS_CHECK_EQ_FLT( theta, 0.0f/4.0f * ml7::constants::pi2 );
    ml7::Matrix2x2( 0.0f, -4.0f, 3.0f, 0.0f ).decompose( scaling, theta );
    TESTLABS_CHECK_EQ_FLT( scaling.x, 3.0f );
    TESTLABS_CHECK_EQ_FLT( scaling.y, 4.0f );
    TESTLABS_CHECK_EQ_FLT( theta, 1.0f/4.0f * ml7::constants::pi2 );
    ml7::Matrix2x2( -5.0f, 0.0f, 0.0f, -6.0f ).decompose( scaling, theta );
    TESTLABS_CHECK_EQ_FLT( scaling.x, 5.0f );
    TESTLABS_CHECK_EQ_FLT( scaling.y, 6.0f );
    TESTLABS_CHECK_EQ_FLT( theta, -2.0f/4.0f * ml7::constants::pi2 );
    ml7::Matrix2x2( 0.0f, 8.0f, -7.0f, 0.0f ).decompose( scaling, theta );
    TESTLABS_CHECK_EQ_FLT( scaling.x, 7.0f );
    TESTLABS_CHECK_EQ_FLT( scaling.y, 8.0f );
    TESTLABS_CHECK_EQ_FLT( theta, -1.0f/4.0f * ml7::constants::pi2 );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ).transform( ml7::Vector2( -5.0f, 6.0f ) ), ml7::Vector2( -17.0f, -39.0f ) );
    TESTLABS_CHECK_EQ( round( ml7::Matrix2x2::compose( ml7::Vector2( 1.0f, 2.0f ), 0.0f/4.0f * ml7::constants::pi2 ).transform( ml7::Vector2( 1.0f, 2.0f ) ), 3 ), round( ml7::Vector2( 1.0f, 4.0f ), 3 ) );
    TESTLABS_CHECK_EQ( round( ml7::Matrix2x2::compose( ml7::Vector2( 3.0f, 4.0f ), 1.0f/4.0f * ml7::constants::pi2 ).transform( ml7::Vector2( 1.0f, 2.0f ) ), 3 ), round( ml7::Vector2( -8.0f, 3.0f ), 3 ) );
    TESTLABS_CHECK_EQ( round( ml7::Matrix2x2::compose( ml7::Vector2( 5.0f, 6.0f ), -2.0f/4.0f * ml7::constants::pi2 ).transform( ml7::Vector2( 1.0f, 2.0f ) ), 3 ), round( ml7::Vector2( -5.0f, -12.0f ), 3 ) );
    TESTLABS_CHECK_EQ( round( ml7::Matrix2x2::compose( ml7::Vector2( 7.0f, 8.0f ), -1.0f/4.0f * ml7::constants::pi2 ).transform( ml7::Vector2( 1.0f, 2.0f ) ), 3 ), round( ml7::Vector2( 16.0f, -7.0f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).transpose(), ml7::Matrix2x2( 1.0f, 3.0f, 2.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).invert(), ml7::Matrix2x2( -2.0f, 1.0f, 1.5f, -0.5f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).set_row( 0, ml7::Vector2( 5.0f, 6.0f ) ), ml7::Matrix2x2( 5.0f, 6.0f, 3.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).set_row( 1, ml7::Vector2( 5.0f, 6.0f ) ), ml7::Matrix2x2( 1.0f, 2.0f, 5.0f, 6.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).set_column( 0, ml7::Vector2( 5.0f, 6.0f ) ), ml7::Matrix2x2( 5.0f, 2.0f, 6.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).set_column( 1, ml7::Vector2( 5.0f, 6.0f ) ), ml7::Matrix2x2( 1.0f, 5.0f, 3.0f, 6.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f )[ 0 ][ 0 ], 1.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f )[ 0 ][ 1 ], 2.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f )[ 1 ][ 0 ], 3.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f )[ 1 ][ 1 ], 4.0f );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f )[ 0 ][ 0 ] = 5.0f, 5.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f )[ 0 ][ 1 ] = 5.0f, 5.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f )[ 1 ][ 0 ] = 5.0f, 5.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f )[ 1 ][ 1 ] = 5.0f, 5.0f );

    TESTLABS_CHECK_EQ( +ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ), ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) );
    TESTLABS_CHECK_EQ( -ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ), ml7::Matrix2x2( -1.0f, 2.0f, -3.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) + ml7::Matrix2x2( -5.0f, 6.0f, -7.0f, 8.0f ), ml7::Matrix2x2( -4.0f, 4.0f, -4.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) - ml7::Matrix2x2( -5.0f, 6.0f, -7.0f, 8.0f ), ml7::Matrix2x2( 6.0f, -8.0f, 10.0f, -12.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) * 5.0f, ml7::Matrix2x2( 5.0f, -10.0f, 15.0f, -20.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 5.0f, -10.0f, 15.0f, -20.0f ) / 5.0f, ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) * ml7::Matrix2x2( -5.0f, 6.0f, -7.0f, 8.0f ), ml7::Matrix2x2( 9.0f, -10.0f, 13.0f, -14.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( -5.0f, 6.0f, -7.0f, 8.0f ) * ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ), ml7::Matrix2x2( 13.0f, -14.0f, 17.0f, -18.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) * ml7::Vector2( -5.0f, 6.0f ), ml7::Vector2( -17.0f, -39.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) += ml7::Matrix2x2( -5.0f, 6.0f, -7.0f, 8.0f ), ml7::Matrix2x2( -4.0f, 4.0f, -4.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) -= ml7::Matrix2x2( -5.0f, 6.0f, -7.0f, 8.0f ), ml7::Matrix2x2( 6.0f, -8.0f, 10.0f, -12.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) *= 5.0f, ml7::Matrix2x2( 5.0f, -10.0f, 15.0f, -20.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 5.0f, -10.0f, 15.0f, -20.0f ) /= 5.0f, ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) );

    TESTLABS_CHECK( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) == ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) );
    TESTLABS_CHECK( !(ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) == ml7::Matrix2x2( -1.0f, 2.0f, -3.0f, 4.0f )) );

    TESTLABS_CHECK( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) != ml7::Matrix2x2( -1.0f, 2.0f, -3.0f, 4.0f ) );
    TESTLABS_CHECK( !(ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) != ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f )) );

    TESTLABS_CHECK_EQ( 5.0f * ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ), ml7::Matrix2x2( 5.0f, -10.0f, 15.0f, -20.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector2( -5.0f, 6.0f ) * ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ), ml7::Vector2( 13.0f, -14.0f ) );
}



#endif // F77_TESTS_ML7_MATRIX2X2_H

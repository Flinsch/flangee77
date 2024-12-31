#ifndef F77_TESTS_ML7_MATRIX2X2_H
#define F77_TESTS_ML7_MATRIX2X2_H

#include <MathLabs/Matrix2x2.h>

#include <TestLabs/TestSuite.h>

#include "./shared.h"



TESTLABS_CASE( u8"CoreLabs:  Matrix2x2:  " )
{
    TESTLABS_CHECK_EQ( ml7::Matrix2x2(), ml7::Matrix2x2( 1.0f, 0.0f, 0.0f, 1.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2::from_axes( ml7::Vector2( 1.0f, 0.0f ), ml7::Vector2( 0.0f, 1.0f ) ), ml7::Matrix2x2( 1.0f, 0.0f, 0.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2::from_axes( ml7::Vector2( 1.0f, 0.0f ), ml7::Vector2( 0.0f, 1.0f ) ).transform( ml7::Vector2( 1.0f, 2.0f ) ), ml7::Vector2( 1.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2::from_axes( ml7::Vector2( 0.0f, 1.0f ), ml7::Vector2( -1.0f, 0.0f ) ), ml7::Matrix2x2( 0.0f, -1.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2::from_axes( ml7::Vector2( 0.0f, 1.0f ), ml7::Vector2( -1.0f, 0.0f ) ).transform( ml7::Vector2( 1.0f, 2.0f ) ), ml7::Vector2( -2.0f, 1.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2::scaling( -1.0f ), ml7::Matrix2x2( -1.0f, 0.0f, 0.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2::scaling( ml7::Vector2( 0.5f, 2.0f ) ), ml7::Matrix2x2( 0.5f, 0.0f, 0.0f, 2.0f ) );

    // rotation see below

    // compose (and decompose) see below

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 2.0f, 3.0f, 4.0f, 1.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 3.0f, 4.0f, 1.0f, 2.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 4.0f, 1.0f, 2.0f, 3.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 3.0f, 12.0f, 2.0f, 8.0f ).is_invertible(), false );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).determinant(), -2.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 2.0f, 3.0f, 4.0f, 1.0f ).determinant(), -10.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 3.0f, 4.0f, 1.0f, 2.0f ).determinant(), 2.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 4.0f, 1.0f, 2.0f, 3.0f ).determinant(), 10.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 3.0f, 12.0f, 2.0f, 8.0f ).determinant(), 0.0f );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).transposed(), ml7::Matrix2x2( 1.0f, 3.0f, 2.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 3.0f, 2.0f, 4.0f ).transposed(), ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).inverted(), ml7::Matrix2x2( -2.0f, 1.0f, 1.5f, -0.5f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( -2.0f, 1.0f, 1.5f, -0.5f ).inverted(), ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).get_row( 0 ), ml7::Vector2( 1.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).get_row( 1 ), ml7::Vector2( 3.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).get_column( 0 ), ml7::Vector2( 1.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).get_column( 1 ), ml7::Vector2( 2.0f, 4.0f ) );

    ml7::Vector2 x, y;
    ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).to_axes( x, y );
    TESTLABS_CHECK_EQ( x, ml7::Vector2( 1.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( y, ml7::Vector2( 2.0f, 4.0f ) );

    // decompose see below

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ).transform( ml7::Vector2( -5.0f, 6.0f ) ), ml7::Vector2( -17.0f, -39.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ).transform_inverted( ml7::Vector2( -17.0f, -39.0f ) ), ml7::Vector2( -5.0f, 6.0f ) );

    // (further) transform etc. see below

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).transpose(), ml7::Matrix2x2( 1.0f, 3.0f, 2.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 3.0f, 2.0f, 4.0f ).transpose(), ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ).invert(), ml7::Matrix2x2( -2.0f, 1.0f, 1.5f, -0.5f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( -2.0f, 1.0f, 1.5f, -0.5f ).invert(), ml7::Matrix2x2( 1.0f, 2.0f, 3.0f, 4.0f ) );

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
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 5.0f, -8.0f, 12.0f, 7.0f ) + ml7::Matrix2x2( -6.0f, 9.0f, -14.0f, 3.0f ), ml7::Matrix2x2( -1.0f, 1.0f, -2.0f, 10.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 5.0f, -8.0f, 12.0f, 7.0f ) - ml7::Matrix2x2( -6.0f, 9.0f, -14.0f, 3.0f ), ml7::Matrix2x2( 11.0f, -17.0f, 26.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) * 5.0f, ml7::Matrix2x2( 5.0f, -10.0f, 15.0f, -20.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 5.0f, -10.0f, 15.0f, -20.0f ) / 5.0f, ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) * ml7::Matrix2x2( -5.0f, 6.0f, -7.0f, 8.0f ), ml7::Matrix2x2( 9.0f, -10.0f, 13.0f, -14.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( -5.0f, 6.0f, -7.0f, 8.0f ) * ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ), ml7::Matrix2x2( 13.0f, -14.0f, 17.0f, -18.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) * ml7::Vector2( -5.0f, 6.0f ), ml7::Vector2( -17.0f, -39.0f ) );

    TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) * ml7::Matrix2x2( -5.0f, 6.0f, -7.0f, 8.0f ) * ml7::Vector2( 1.0f, 2.0f ), 3 ), ml7::round( ml7::Vector2( -11.0f, -15.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) * (ml7::Matrix2x2( -5.0f, 6.0f, -7.0f, 8.0f ) * ml7::Vector2( 1.0f, 2.0f )), 3 ), ml7::round( ml7::Vector2( -11.0f, -15.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( (ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) * ml7::Matrix2x2( -5.0f, 6.0f, -7.0f, 8.0f )) * ml7::Vector2( 1.0f, 2.0f ), 3 ), ml7::round( ml7::Vector2( -11.0f, -15.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x2( 9.0f, -10.0f, 13.0f, -14.0f ) * ml7::Vector2( 1.0f, 2.0f ), 3 ), ml7::round( ml7::Vector2( -11.0f, -15.0f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 5.0f, -8.0f, 12.0f, 7.0f ) += ml7::Matrix2x2( -6.0f, 9.0f, -14.0f, 3.0f ), ml7::Matrix2x2( -1.0f, 1.0f, -2.0f, 10.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 5.0f, -8.0f, 12.0f, 7.0f ) -= ml7::Matrix2x2( -6.0f, 9.0f, -14.0f, 3.0f ), ml7::Matrix2x2( 11.0f, -17.0f, 26.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) *= 5.0f, ml7::Matrix2x2( 5.0f, -10.0f, 15.0f, -20.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x2( 5.0f, -10.0f, 15.0f, -20.0f ) /= 5.0f, ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) );

    TESTLABS_CHECK( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) == ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) );
    TESTLABS_CHECK( !(ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) == ml7::Matrix2x2( -1.0f, 2.0f, -3.0f, 4.0f )) );

    TESTLABS_CHECK( ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) != ml7::Matrix2x2( -1.0f, 2.0f, -3.0f, 4.0f ) );
    TESTLABS_CHECK( !(ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ) != ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f )) );

    TESTLABS_CHECK_EQ( 5.0f * ml7::Matrix2x2( 1.0f, -2.0f, 3.0f, -4.0f ), ml7::Matrix2x2( 5.0f, -10.0f, 15.0f, -20.0f ) );
}



TESTLABS_CASE( u8"CoreLabs:  Matrix2x2:  rotation" )
{
    struct Entry
    {
        float angle;
        ml7::Matrix2x2 expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( 1.0f, 0.0f, 0.0f, 1.0f ) },
        { 1.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( 0.0f, -1.0f, 1.0f, 0.0f ) },
        { 2.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( -1.0f, 0.0f, 0.0f, -1.0f ) },
        { 3.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( 0.0f, 1.0f, -1.0f, 0.0f ) },
        { 4.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( 1.0f, 0.0f, 0.0f, 1.0f ) },
        { -1.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( 0.0f, 1.0f, -1.0f, 0.0f ) },
        { -2.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( -1.0f, 0.0f, 0.0f, -1.0f ) },
        { -3.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( 0.0f, -1.0f, 1.0f, 0.0f ) },
    };

    TESTLABS_SUBCASE_BATCH( u8"rotation", container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x2::rotation( entry.angle ), 3 ), ml7::round( entry.expected, 3 ) );
    }
}



TESTLABS_CASE( u8"CoreLabs:  Matrix2x2:  compose(float, ...) / decompose" )
{
    struct Entry
    {
        float scaling;
        float angle;
        ml7::Matrix2x2 expected;
    } entry;

    const std::vector<Entry> container {
        { 1.0f, 0.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( 1.0f, 0.0f, 0.0f, 1.0f ) },
        { 2.0f, 1.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( 0.0f, -2.0f, 2.0f, 0.0f ) },
        { 3.0f, 2.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( -3.0f, 0.0f, 0.0f, -3.0f ) },
        { 4.0f, 3.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( 0.0f, 4.0f, -4.0f, 0.0f ) },
        { 5.0f, 4.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( 5.0f, 0.0f, 0.0f, 5.0f ) },
        { 4.0f, -1.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( 0.0f, 4.0f, -4.0f, 0.0f ) },
        { 3.0f, -2.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( -3.0f, 0.0f, 0.0f, -3.0f ) },
        { 2.0f, -3.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( 0.0f, -2.0f, 2.0f, 0.0f ) },
    };

    TESTLABS_SUBCASE_BATCH( u8"compose(float, ...)", container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x2::compose( entry.scaling, entry.angle ), 3 ), ml7::round( entry.expected, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( u8"decompose", container, entry )
    {
        float expected_angle = entry.angle < -ml7::constants::pi ? ml7::constants::pi - ::fmodf( -entry.angle - ml7::constants::pi, ml7::constants::pi2 ) : ::fmodf( entry.angle + ml7::constants::pi, ml7::constants::pi2 ) - ml7::constants::pi;

        ml7::Vector2 scaling;
        float angle;
        TESTLABS_CHECK( entry.expected.decompose( scaling, angle ) );
        TESTLABS_CHECK_EQ( ml7::round( scaling.x, 5 ), ml7::round( entry.scaling, 5 ) );
        TESTLABS_CHECK_EQ( ml7::round( scaling.y, 5 ), ml7::round( entry.scaling, 5 ) );
        TESTLABS_CHECK_EQ( ml7::round( angle, 5 ), ml7::round( expected_angle, 5 ) );
    }
}

TESTLABS_CASE( u8"CoreLabs:  Matrix2x2:  compose(Vector2, ...) / decompose" )
{
    struct Entry
    {
        ml7::Vector2 scaling;
        float angle;
        ml7::Matrix2x2 expected;
    } entry;

    const std::vector<Entry> container {
        { ml7::Vector2( 1.0f, 2.0f ), 0.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( 1.0f, 0.0f, 0.0f, 2.0f ) },
        { ml7::Vector2( 3.0f, 4.0f ), 1.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( 0.0f, -4.0f, 3.0f, 0.0f ) },
        { ml7::Vector2( 5.0f, 6.0f ), 2.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( -5.0f, 0.0f, 0.0f, -6.0f ) },
        { ml7::Vector2( 7.0f, 8.0f ), 3.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( 0.0f, 8.0f, -7.0f, 0.0f ) },
        { ml7::Vector2( 9.0f, 10.0f ), 4.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( 9.0f, 0.0f, 0.0f, 10.0f ) },
        { ml7::Vector2( 7.0f, 8.0f ), -1.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( 0.0f, 8.0f, -7.0f, 0.0f ) },
        { ml7::Vector2( 5.0f, 6.0f ), -2.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( -5.0f, 0.0f, 0.0f, -6.0f ) },
        { ml7::Vector2( 3.0f, 4.0f ), -3.0f/4.0f * ml7::constants::pi2, ml7::Matrix2x2( 0.0f, -4.0f, 3.0f, 0.0f ) },
    };

    TESTLABS_SUBCASE_BATCH( u8"compose(Vector2, ...)", container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x2::compose( entry.scaling, entry.angle ), 3 ), ml7::round( entry.expected, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( u8"decompose", container, entry )
    {
        float expected_angle = entry.angle < -ml7::constants::pi ? ml7::constants::pi - ::fmodf( -entry.angle - ml7::constants::pi, ml7::constants::pi2 ) : ::fmodf( entry.angle + ml7::constants::pi, ml7::constants::pi2 ) - ml7::constants::pi;

        ml7::Vector2 scaling;
        float angle;
        TESTLABS_CHECK( entry.expected.decompose( scaling, angle ) );
        TESTLABS_CHECK_EQ( ml7::round( scaling.x, 5 ), ml7::round( entry.scaling.x, 5 ) );
        TESTLABS_CHECK_EQ( ml7::round( scaling.y, 5 ), ml7::round( entry.scaling.y, 5 ) );
        TESTLABS_CHECK_EQ( ml7::round( angle, 5 ), ml7::round( expected_angle, 5 ) );
    }
}



TESTLABS_CASE( u8"CoreLabs:  Matrix2x2:  transform etc." )
{
    struct Entry
    {
        ml7::Vector2 scaling;
        float angle;
        ml7::Vector2 untransformed;
        ml7::Vector2 transformed;
    } entry;

    const std::vector<Entry> container {
        { ml7::Vector2( 1.0f, 2.0f ), 0.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( 1.0f, 4.0f ) },
        { ml7::Vector2( 3.0f, 4.0f ), 1.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -8.0f, 3.0f ) },
        { ml7::Vector2( 5.0f, 6.0f ), 2.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -5.0f, -12.0f ) },
        { ml7::Vector2( 7.0f, 8.0f ), 3.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( 16.0f, -7.0f ) },
        { ml7::Vector2( 9.0f, 10.0f ), 4.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( 9.0f, 20.0f ) },
        { ml7::Vector2( 7.0f, 8.0f ), -1.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( 16.0f, -7.0f ) },
        { ml7::Vector2( 5.0f, 6.0f ), -2.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -5.0f, -12.0f ) },
        { ml7::Vector2( 3.0f, 4.0f ), -3.0f/4.0f * ml7::constants::pi2, ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -8.0f, 3.0f ) },
    };

    TESTLABS_SUBCASE_BATCH( u8"transform", container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x2::compose( entry.scaling, entry.angle ).transform( entry.untransformed ), 3 ), ml7::round( entry.transformed, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( u8"transform_inverted", container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x2::compose( entry.scaling, entry.angle ).transform_inverted( entry.transformed ), 3 ), ml7::round( entry.untransformed, 3 ) );
    }
}



#endif // F77_TESTS_ML7_MATRIX2X2_H

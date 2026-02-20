
#include <TestLabs/TestSuite.h>

#include <MathLabs/Matrix2x3.h>

#include "./shared.h"



TESTLABS_CASE( u8"CoreLabs:  Matrix2x3:  " )
{
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f(), ml7::Matrix2x3f( 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( ml7::Matrix2x2f( 1.0f, 2.0f, 3.0f, 4.0f ) ), ml7::Matrix2x3f( 1.0f, 2.0f, 0.0f, 3.0f, 4.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( ml7::Matrix2x2f( 1.0f, 2.0f, 3.0f, 4.0f ), ml7::Vector2f( 5.0f, 6.0f ) ), ml7::Matrix2x3f( 1.0f, 2.0f, 5.0f, 3.0f, 4.0f, 6.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3f::from_axes( ml7::Vector2f( 1.0f, 0.0f ), ml7::Vector2f( 0.0f, 1.0f ) ), ml7::Matrix2x3f( 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f::from_axes( ml7::Vector2f( 1.0f, 0.0f ), ml7::Vector2f( 0.0f, 1.0f ) ).transform( ml7::Vector2f( 1.0f, 2.0f ) ), ml7::Vector2f( 1.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f::from_axes( ml7::Vector2f( 0.0f, 1.0f ), ml7::Vector2f( -1.0f, 0.0f ) ), ml7::Matrix2x3f( 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f::from_axes( ml7::Vector2f( 0.0f, 1.0f ), ml7::Vector2f( -1.0f, 0.0f ) ).transform( ml7::Vector2f( 1.0f, 2.0f ) ), ml7::Vector2f( -2.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f::from_axes( ml7::Vector2f( 0.0f, 1.0f ), ml7::Vector2f( -1.0f, 0.0f ), ml7::Vector2f( 2.0f, 3.0f ) ), ml7::Matrix2x3f( 0.0f, -1.0f, 2.0f, 1.0f, 0.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f::from_axes( ml7::Vector2f( 0.0f, 1.0f ), ml7::Vector2f( -1.0f, 0.0f ), ml7::Vector2f( 2.0f, 3.0f ) ).transform( ml7::Vector2f( 1.0f, 2.0f ) ), ml7::Vector2f( 0.0f, 4.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3f::scaling( -1.0f ), ml7::Matrix2x3f( -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f::scaling( ml7::Vector2f( 0.5f, 2.0f ) ), ml7::Matrix2x3f( 0.5f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f ) );

    // rotation see below

    TESTLABS_CHECK_EQ( ml7::Matrix2x3f::translation( ml7::Vector2f( 5.0f, 6.0f ) ), ml7::Matrix2x3f( 1.0f, 0.0f, 5.0f, 0.0f, 1.0f, 6.0f ) );

    // compose (and decompose) see below

    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 5.0f, 3.0f, 4.0f, 6.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 2.0f, 3.0f, 5.0f, 4.0f, 1.0f, 6.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 3.0f, 4.0f, 5.0f, 1.0f, 2.0f, 6.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 4.0f, 1.0f, 5.0f, 2.0f, 3.0f, 6.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 3.0f, 12.0f, 0.0f, 2.0f, 8.0f, 0.0f ).is_invertible(), false );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 5.0f, 3.0f, 4.0f, 6.0f ).determinant(), -2.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 2.0f, 3.0f, 5.0f, 4.0f, 1.0f, 6.0f ).determinant(), -10.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 3.0f, 4.0f, 5.0f, 1.0f, 2.0f, 6.0f ).determinant(), 2.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 4.0f, 1.0f, 5.0f, 2.0f, 3.0f, 6.0f ).determinant(), 10.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 3.0f, 12.0f, 0.0f, 2.0f, 8.0f, 0.0f ).determinant(), 0.0f );

    TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f ).inverted(), 3 ), ml7::round( ml7::Matrix2x3f( -5.0f/3.0f, 2.0f/3.0f, 1.0f, 4.0f/3.0f, -1.0f/3.0f, -2.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x3f( -5.0f/3.0f, 2.0f/3.0f, 1.0f, 4.0f/3.0f, -1.0f/3.0f, -2.0f ).inverted(), 3 ), ml7::round( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f ).get_column( 0 ), ml7::Vector2f( 1.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f ).get_column( 1 ), ml7::Vector2f( 2.0f, 5.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f ).get_column( 2 ), ml7::Vector2f( 3.0f, 6.0f ) );

    ml7::Vector2f x, y, origin;
    ml7::Matrix2x3f( 1.0f, 2.0f, 5.0f, 3.0f, 4.0f, 6.0f ).to_axes( x, y );
    TESTLABS_CHECK_EQ( x, ml7::Vector2f( 1.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( y, ml7::Vector2f( 2.0f, 4.0f ) );
    ml7::Matrix2x3f( 1.0f, 2.0f, 5.0f, 3.0f, 4.0f, 6.0f ).to_axes( x, y, origin );
    TESTLABS_CHECK_EQ( x, ml7::Vector2f( 1.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( y, ml7::Vector2f( 2.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( origin, ml7::Vector2f( 5.0f, 6.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 5.0f, 3.0f, 4.0f, 6.0f ).to_matrix2x2(), ml7::Matrix2x2f( 1.0f, 2.0f, 3.0f, 4.0f ) );

    ml7::Matrix2x2f m2x2;
    ml7::Vector2f translation;
    ml7::Matrix2x3f( 1.0f, 2.0f, 5.0f, 3.0f, 4.0f, 6.0f ).decompose( m2x2, translation );
    TESTLABS_CHECK_EQ( m2x2, ml7::Matrix2x2f( 1.0f, 2.0f, 3.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( translation, ml7::Vector2f( 5.0f, 6.0f ) );

    // (further) decompose see below

    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, -2.0f, 5.0f, 3.0f, -4.0f, -6.0f ).transform( ml7::Vector2f( -5.0f, 6.0f ) ), ml7::Vector2f( -12.0f, -45.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, -2.0f, 5.0f, 3.0f, -4.0f, -6.0f ).transform2x2( ml7::Vector2f( -5.0f, 6.0f ) ), ml7::Vector2f( -17.0f, -39.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, -2.0f, 5.0f, 3.0f, -4.0f, -6.0f ).transform_inverted( ml7::Vector2f( -12.0f, -45.0f ) ), ml7::Vector2f( -5.0f, 6.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, -2.0f, 5.0f, 3.0f, -4.0f, -6.0f ).transform2x2_inverted( ml7::Vector2f( -17.0f, -39.0f ) ), ml7::Vector2f( -5.0f, 6.0f ) );

    // (further) transform etc. see below

    TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f ).invert(), 3 ), ml7::round( ml7::Matrix2x3f( -5.0f/3.0f, 2.0f/3.0f, 1.0f, 4.0f/3.0f, -1.0f/3.0f, -2.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x3f( -5.0f/3.0f, 2.0f/3.0f, 1.0f, 4.0f/3.0f, -1.0f/3.0f, -2.0f ).invert(), 3 ), ml7::round( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f ).set_column( 0, ml7::Vector2f( 7.0f, 8.0f ) ), ml7::Matrix2x3f( 7.0f, 2.0f, 3.0f, 8.0f, 5.0f, 6.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f ).set_column( 1, ml7::Vector2f( 7.0f, 8.0f ) ), ml7::Matrix2x3f( 1.0f, 7.0f, 3.0f, 4.0f, 8.0f, 6.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f ).set_column( 2, ml7::Vector2f( 7.0f, 8.0f ) ), ml7::Matrix2x3f( 1.0f, 2.0f, 7.0f, 4.0f, 5.0f, 8.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 0 ][ 0 ], 1.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 0 ][ 1 ], 2.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 0 ][ 2 ], 3.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 1 ][ 0 ], 4.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 1 ][ 1 ], 5.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 1 ][ 2 ], 6.0f );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 0 ][ 0 ] = 7.0f, 7.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 0 ][ 1 ] = 7.0f, 7.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 0 ][ 2 ] = 7.0f, 7.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 1 ][ 0 ] = 7.0f, 7.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 1 ][ 1 ] = 7.0f, 7.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f )[ 1 ][ 2 ] = 7.0f, 7.0f );

    TESTLABS_CHECK_EQ( +ml7::Matrix2x3f( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ), ml7::Matrix2x3f( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) );
    TESTLABS_CHECK_EQ( -ml7::Matrix2x3f( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ), ml7::Matrix2x3f( -1.0f, 2.0f, -3.0f, 4.0f, -5.0f, 6.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 5.0f, -8.0f, 12.0f, 7.0f, -9.0f, 15.0f ) + ml7::Matrix2x3f( -6.0f, 9.0f, -14.0f, 3.0f, 11.0f, -7.0f ), ml7::Matrix2x3f( -1.0f, 1.0f, -2.0f, 10.0f, 2.0f, 8.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 5.0f, -8.0f, 12.0f, 7.0f, -9.0f, 15.0f ) - ml7::Matrix2x3f( -6.0f, 9.0f, -14.0f, 3.0f, 11.0f, -7.0f ), ml7::Matrix2x3f( 11.0f, -17.0f, 26.0f, 4.0f, -20.0f, 22.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) * 7.0f, ml7::Matrix2x3f( 7.0f, -14.0f, 21.0f, -28.0f, 35.0f, -42.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 7.0f, -14.0f, 21.0f, -28.0f, 35.0f, -42.0f ) / 7.0f, ml7::Matrix2x3f( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) * ml7::Matrix2x3f( -7.0f, 8.0f, -9.0f, 10.0f, -11.0f, 12.0f ), ml7::Matrix2x3f( -27.0f, 30.0f, -30.0f, 78.0f, -87.0f, 90.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( -7.0f, 8.0f, -9.0f, 10.0f, -11.0f, 12.0f ) * ml7::Matrix2x3f( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ), ml7::Matrix2x3f( -39.0f, 54.0f, -78.0f, 54.0f, -75.0f, 108.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, -2.0f, 7.0f, 3.0f, -4.0f, 8.0f ) * ml7::Vector2f( -5.0f, 6.0f ), ml7::Vector2f( -10.0f, -31.0f ) );

    TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x3f(1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) * ml7::Matrix2x3f( -7.0f, 8.0f, -9.0f, 10.0f, -11.0f, 12.0f ) * ml7::Vector2f( 1.0f, 2.0f ), 3 ), ml7::round( ml7::Vector2f( 3.0f, -6.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x3f( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) * (ml7::Matrix2x3f( -7.0f, 8.0f, -9.0f, 10.0f, -11.0f, 12.0f ) * ml7::Vector2f( 1.0f, 2.0f )), 3 ), ml7::round( ml7::Vector2f( 3.0f, -6.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( (ml7::Matrix2x3f( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) * ml7::Matrix2x3f( -7.0f, 8.0f, -9.0f, 10.0f, -11.0f, 12.0f )) * ml7::Vector2f( 1.0f, 2.0f ), 3 ), ml7::round( ml7::Vector2f( 3.0f, -6.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x3f( -27.0f, 30.0f, -30.0f, 78.0f, -87.0f, 90.0f ) * ml7::Vector2f( 1.0f, 2.0f ), 3 ), ml7::round( ml7::Vector2f( 3.0f, -6.0f ), 3 ) );

    TESTLABS_CHECK_EQ( 7.0f * ml7::Matrix2x3f( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) , ml7::Matrix2x3f( 7.0f, -14.0f, 21.0f, -28.0f, 35.0f, -42.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 5.0f, -8.0f, 12.0f, 7.0f, -9.0f, 15.0f ) += ml7::Matrix2x3f( -6.0f, 9.0f, -14.0f, 3.0f, 11.0f, -7.0f ), ml7::Matrix2x3f( -1.0f, 1.0f, -2.0f, 10.0f, 2.0f, 8.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 5.0f, -8.0f, 12.0f, 7.0f, -9.0f, 15.0f ) -= ml7::Matrix2x3f( -6.0f, 9.0f, -14.0f, 3.0f, 11.0f, -7.0f ), ml7::Matrix2x3f( 11.0f, -17.0f, 26.0f, 4.0f, -20.0f, 22.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) *= 7.0f, ml7::Matrix2x3f( 7.0f, -14.0f, 21.0f, -28.0f, 35.0f, -42.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix2x3f( 7.0f, -14.0f, 21.0f, -28.0f, 35.0f, -42.0f ) /= 7.0f, ml7::Matrix2x3f( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) );

    TESTLABS_CHECK( ml7::Matrix2x3f( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) == ml7::Matrix2x3f( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) );
    TESTLABS_CHECK( !(ml7::Matrix2x3f( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) == ml7::Matrix2x3f( -1.0f, 2.0f, -3.0f, 4.0f, -5.0f, 6.0f )) );

    TESTLABS_CHECK( ml7::Matrix2x3f( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) != ml7::Matrix2x3f( -1.0f, 2.0f, -3.0f, 4.0f, -5.0f, 6.0f ) );
    TESTLABS_CHECK( !(ml7::Matrix2x3f( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f ) != ml7::Matrix2x3f( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f )) );
}



TESTLABS_CASE( u8"CoreLabs:  Matrix2x3:  rotation" )
{
    struct Entry
    {
        float angle;
        ml7::Matrix2x3f expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f/4.0f * ml7::constantsf::pi2, ml7::Matrix2x3f( 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { 1.0f/4.0f * ml7::constantsf::pi2, ml7::Matrix2x3f( 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f ) },
        { 2.0f/4.0f * ml7::constantsf::pi2, ml7::Matrix2x3f( -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) },
        { 3.0f/4.0f * ml7::constantsf::pi2, ml7::Matrix2x3f( 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f ) },
        { 4.0f/4.0f * ml7::constantsf::pi2, ml7::Matrix2x3f( 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { -1.0f/4.0f * ml7::constantsf::pi2, ml7::Matrix2x3f( 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f ) },
        { -2.0f/4.0f * ml7::constantsf::pi2, ml7::Matrix2x3f( -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) },
        { -3.0f/4.0f * ml7::constantsf::pi2, ml7::Matrix2x3f( 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f ) },
    };

    TESTLABS_SUBCASE_BATCH( u8"rotation", container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x3f::rotation( entry.angle ), 3 ), ml7::round( entry.expected, 3 ) );
    }
}



TESTLABS_CASE( u8"CoreLabs:  Matrix2x3:  compose(float, ...) / decompose" )
{
    struct Entry
    {
        float scaling;
        float angle;
        ml7::Vector2f translation;
        ml7::Matrix2x3f expected;
    } entry;

    const std::vector<Entry> container {
        { 1.0f, 0.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, 6.0f ), ml7::Matrix2x3f( 1.0f, 0.0f, 5.0f, 0.0f, 1.0f, 6.0f ) },
        { 2.0f, 1.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, 6.0f ), ml7::Matrix2x3f( 0.0f, -2.0f, 5.0f, 2.0f, 0.0f, 6.0f ) },
        { 3.0f, 2.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, 6.0f ), ml7::Matrix2x3f( -3.0f, 0.0f, 5.0f, 0.0f, -3.0f, 6.0f ) },
        { 4.0f, 3.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, 6.0f ), ml7::Matrix2x3f( 0.0f, 4.0f, 5.0f, -4.0f, 0.0f, 6.0f ) },
        { 5.0f, 4.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, 6.0f ), ml7::Matrix2x3f( 5.0f, 0.0f, 5.0f, 0.0f, 5.0f, 6.0f ) },
        { 4.0f, -1.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, 6.0f ), ml7::Matrix2x3f( 0.0f, 4.0f, 5.0f, -4.0f, 0.0f, 6.0f ) },
        { 3.0f, -2.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, 6.0f ), ml7::Matrix2x3f( -3.0f, 0.0f, 5.0f, 0.0f, -3.0f, 6.0f ) },
        { 2.0f, -3.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, 6.0f ), ml7::Matrix2x3f( 0.0f, -2.0f, 5.0f, 2.0f, 0.0f, 6.0f ) },
    };

    TESTLABS_SUBCASE_BATCH( u8"compose(float, ...)", container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x3f::compose( entry.scaling, entry.angle, entry.translation ), 3 ), ml7::round( entry.expected, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( u8"decompose", container, entry )
    {
        float expected_angle = entry.angle < -ml7::constantsf::pi ? ml7::constantsf::pi - ::fmodf( -entry.angle - ml7::constantsf::pi, ml7::constantsf::pi2 ) : ::fmodf( entry.angle + ml7::constantsf::pi, ml7::constantsf::pi2 ) - ml7::constantsf::pi;

        ml7::Vector2f scaling;
        float angle;
        ml7::Vector2f translation;
        TESTLABS_CHECK( entry.expected.decompose( scaling, angle, translation ) );
        TESTLABS_CHECK_EQ( ml7::round( scaling.x, 5 ), ml7::round( entry.scaling, 5 ) );
        TESTLABS_CHECK_EQ( ml7::round( scaling.y, 5 ), ml7::round( entry.scaling, 5 ) );
        TESTLABS_CHECK_EQ( ml7::round( angle, 5 ), ml7::round( expected_angle, 5 ) );
        TESTLABS_CHECK_EQ( ml7::round( translation.x, 5 ), ml7::round( entry.translation.x, 5 ) );
        TESTLABS_CHECK_EQ( ml7::round( translation.y, 5 ), ml7::round( entry.translation.y, 5 ) );
    }
}

TESTLABS_CASE( u8"CoreLabs:  Matrix2x3:  compose(Vector2, ...) / decompose" )
{
    struct Entry
    {
        ml7::Vector2f scaling;
        float angle;
        ml7::Vector2f translation;
        ml7::Matrix2x3f expected;
    } entry;

    const std::vector<Entry> container {
        { ml7::Vector2f( 1.0f, 2.0f ), 0.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, 6.0f ), ml7::Matrix2x3f( 1.0f, 0.0f, 5.0f, 0.0f, 2.0f, 6.0f ) },
        { ml7::Vector2f( 3.0f, 4.0f ), 1.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, 6.0f ), ml7::Matrix2x3f( 0.0f, -4.0f, 5.0f, 3.0f, 0.0f, 6.0f ) },
        { ml7::Vector2f( 5.0f, 6.0f ), 2.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, 6.0f ), ml7::Matrix2x3f( -5.0f, 0.0f, 5.0f, 0.0f, -6.0f, 6.0f ) },
        { ml7::Vector2f( 7.0f, 8.0f ), 3.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, 6.0f ), ml7::Matrix2x3f( 0.0f, 8.0f, 5.0f, -7.0f, 0.0f, 6.0f ) },
        { ml7::Vector2f( 9.0f, 10.0f ), 4.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, 6.0f ), ml7::Matrix2x3f( 9.0f, 0.0f, 5.0f, 0.0f, 10.0f , 6.0f) },
        { ml7::Vector2f( 7.0f, 8.0f ), -1.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, 6.0f ), ml7::Matrix2x3f( 0.0f, 8.0f, 5.0f, -7.0f, 0.0f, 6.0f ) },
        { ml7::Vector2f( 5.0f, 6.0f ), -2.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, 6.0f ), ml7::Matrix2x3f( -5.0f, 0.0f, 5.0f, 0.0f, -6.0f, 6.0f ) },
        { ml7::Vector2f( 3.0f, 4.0f ), -3.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, 6.0f ), ml7::Matrix2x3f( 0.0f, -4.0f, 5.0f, 3.0f, 0.0f, 6.0f ) },
    };

    TESTLABS_SUBCASE_BATCH( u8"compose(Vector2, ...)", container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x3f::compose( entry.scaling, entry.angle, entry.translation ), 3 ), ml7::round( entry.expected, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( u8"decompose", container, entry )
    {
        float expected_angle = entry.angle < -ml7::constantsf::pi ? ml7::constantsf::pi - ::fmodf( -entry.angle - ml7::constantsf::pi, ml7::constantsf::pi2 ) : ::fmodf( entry.angle + ml7::constantsf::pi, ml7::constantsf::pi2 ) - ml7::constantsf::pi;

        ml7::Vector2f scaling;
        float angle;
        ml7::Vector2f translation;
        TESTLABS_CHECK( entry.expected.decompose( scaling, angle, translation ) );
        TESTLABS_CHECK_EQ( ml7::round( scaling.x, 5 ), ml7::round( entry.scaling.x, 5 ) );
        TESTLABS_CHECK_EQ( ml7::round( scaling.y, 5 ), ml7::round( entry.scaling.y, 5 ) );
        TESTLABS_CHECK_EQ( ml7::round( angle, 5 ), ml7::round( expected_angle, 5 ) );
        TESTLABS_CHECK_EQ( ml7::round( translation.x, 5 ), ml7::round( entry.translation.x, 5 ) );
        TESTLABS_CHECK_EQ( ml7::round( translation.y, 5 ), ml7::round( entry.translation.y, 5 ) );
    }
}



TESTLABS_CASE( u8"CoreLabs:  Matrix2x3:  transform etc." )
{
    struct Entry
    {
        ml7::Vector2f scaling;
        float angle;
        ml7::Vector2f translation;
        ml7::Vector2f untransformed;
        ml7::Vector2f transformed;
        ml7::Vector2f transformed_2x2;
    } entry;

    const std::vector<Entry> container {
        { ml7::Vector2f( 1.0f, 2.0f ), 0.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, -6.0f ), ml7::Vector2f( 1.0f, 2.0f ), ml7::Vector2f( 6.0f, -2.0f ), ml7::Vector2f( 1.0f, 4.0f ) },
        { ml7::Vector2f( 3.0f, 4.0f ), 1.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, -6.0f ), ml7::Vector2f( 1.0f, 2.0f ), ml7::Vector2f( -3.0f, -3.0f ), ml7::Vector2f( -8.0f, 3.0f ) },
        { ml7::Vector2f( 5.0f, 6.0f ), 2.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, -6.0f ), ml7::Vector2f( 1.0f, 2.0f ), ml7::Vector2f( 0.0f, -18.0f ), ml7::Vector2f( -5.0f, -12.0f ) },
        { ml7::Vector2f( 7.0f, 8.0f ), 3.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, -6.0f ), ml7::Vector2f( 1.0f, 2.0f ), ml7::Vector2f( 21.0f, -13.0f ), ml7::Vector2f( 16.0f, -7.0f ) },
        { ml7::Vector2f( 9.0f, 10.0f ), 4.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, -6.0f ), ml7::Vector2f( 1.0f, 2.0f ), ml7::Vector2f( 14.0f, 14.0f ), ml7::Vector2f( 9.0f, 20.0f ) },
        { ml7::Vector2f( 7.0f, 8.0f ), -1.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, -6.0f ), ml7::Vector2f( 1.0f, 2.0f ), ml7::Vector2f( 21.0f, -13.0f ), ml7::Vector2f( 16.0f, -7.0f ) },
        { ml7::Vector2f( 5.0f, 6.0f ), -2.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, -6.0f ), ml7::Vector2f( 1.0f, 2.0f ), ml7::Vector2f( 0.0f, -18.0f ), ml7::Vector2f( -5.0f, -12.0f ) },
        { ml7::Vector2f( 3.0f, 4.0f ), -3.0f/4.0f * ml7::constantsf::pi2, ml7::Vector2f( 5.0f, -6.0f ), ml7::Vector2f( 1.0f, 2.0f ), ml7::Vector2f( -3.0f, -3.0f ), ml7::Vector2f( -8.0f, 3.0f ) },
    };

    TESTLABS_SUBCASE_BATCH( u8"transform", container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x3f::compose( entry.scaling, entry.angle, entry.translation ).transform( entry.untransformed ), 3 ), ml7::round( entry.transformed, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( u8"transform2x2", container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x3f::compose( entry.scaling, entry.angle, entry.translation ).transform2x2( entry.untransformed ), 3 ), ml7::round( entry.transformed_2x2, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( u8"transform_inverted", container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x3f::compose( entry.scaling, entry.angle, entry.translation ).transform_inverted( entry.transformed ), 3 ), ml7::round( entry.untransformed, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( u8"transform2x2_inverted", container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix2x3f::compose( entry.scaling, entry.angle, entry.translation ).transform2x2_inverted( entry.transformed_2x2 ), 3 ), ml7::round( entry.untransformed, 3 ) );
    }
}

#pragma once
#ifndef F77_TESTS_ML7_VECTOR2_H
#define F77_TESTS_ML7_VECTOR2_H

#include <MathLabs/Vector2.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



namespace tl7 {
namespace internals {
    inline
    cl7::string to_string(const ml7::Vector2& v) { return TEXT("{ ") + cl7::to_string(v.x) + TEXT(", ") + cl7::to_string(v.y) + TEXT(" }"); }
}
}



TESTLABS_CASE( TEXT("CoreLabs:  Vector2:  ") )
{
    TESTLABS_CHECK_EQ( ml7::Vector2(), ml7::Vector2( 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f ), ml7::Vector2( 1.0f, 1.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector2( 3.0f, 4.0f ).length(), 5.0f );
    TESTLABS_CHECK_EQ( ml7::Vector2( 3.0f, 4.0f ).lensqr(), 25.0f );

    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2(  0.0f, +1.0f ).azimuth_symmetric(), 3 ), ml7::round( +0.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2( +1.0f, +1.0f ).azimuth_symmetric(), 3 ), ml7::round( +1.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2( +1.0f,  0.0f ).azimuth_symmetric(), 3 ), ml7::round( +2.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2( +1.0f, -1.0f ).azimuth_symmetric(), 3 ), ml7::round( +3.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2(  0.0f, -1.0f ).azimuth_symmetric(), 3 ), ml7::round( +4.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2( -1.0f, -1.0f ).azimuth_symmetric(), 3 ), ml7::round( -3.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2( -1.0f,  0.0f ).azimuth_symmetric(), 3 ), ml7::round( -2.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2( -1.0f, +1.0f ).azimuth_symmetric(), 3 ), ml7::round( -1.0f/8.0f * ml7::constants::pi2, 3 ) );

    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2(  0.0f, +1.0f ).azimuth_asymmetric(), 3 ), ml7::round( 0.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2( +1.0f, +1.0f ).azimuth_asymmetric(), 3 ), ml7::round( 1.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2( +1.0f,  0.0f ).azimuth_asymmetric(), 3 ), ml7::round( 2.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2( +1.0f, -1.0f ).azimuth_asymmetric(), 3 ), ml7::round( 3.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2(  0.0f, -1.0f ).azimuth_asymmetric(), 3 ), ml7::round( 4.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2( -1.0f, -1.0f ).azimuth_asymmetric(), 3 ), ml7::round( 5.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2( -1.0f,  0.0f ).azimuth_asymmetric(), 3 ), ml7::round( 6.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2( -1.0f, +1.0f ).azimuth_asymmetric(), 3 ), ml7::round( 7.0f/8.0f * ml7::constants::pi2, 3 ) );

    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2( +1.0f,  0.0f ).elevation(), 3 ), ml7::round( +0.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2( +1.0f, +1.0f ).elevation(), 3 ), ml7::round( +1.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2(  0.0f, +1.0f ).elevation(), 3 ), ml7::round( +2.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2( -1.0f, +1.0f ).elevation(), 3 ), ml7::round( +1.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2( -1.0f,  0.0f ).elevation(), 3 ), ml7::round( +0.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2( -1.0f, -1.0f ).elevation(), 3 ), ml7::round( -1.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2(  0.0f, -1.0f ).elevation(), 3 ), ml7::round( -2.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2( +1.0f, -1.0f ).elevation(), 3 ), ml7::round( -1.0f/8.0f * ml7::constants::pi2, 3 ) );

    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, 2.0f ).x_(), ml7::Vector2( 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, 2.0f )._y(), ml7::Vector2( 0.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, 2.0f ).xx(), ml7::Vector2( 1.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, 2.0f ).yy(), ml7::Vector2( 2.0f, 2.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector2( 2.0f, 0.0f ).normalized(), ml7::Vector2( 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( -1.0f, -2.0f ).abs(), ml7::Vector2( 1.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, -2.0f ).dot( ml7::Vector2( -1.0f, 2.0f ) ), -5.0f );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, -2.0f ).dotsqr( ml7::Vector2( -1.0f, 2.0f ) ), 25.0f );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, 2.0f ).perp(), ml7::Vector2( -2.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, -2.0f ).perpdot( ml7::Vector2( -1.0f, 2.0f ) ), ml7::Vector2( 1.0f, -2.0f ).perp().dot( ml7::Vector2( -1.0f, 2.0f ) ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, -2.0f ).reflected( ml7::Vector2( 1.0f, 1.0f ) ), ml7::Vector2( 3.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, -2.0f ).refracted( ml7::Vector2( 1.0f, 1.0f ), 0.0f ), ml7::Vector2( -1.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, -2.0f ).refracted( ml7::Vector2( 1.0f, 1.0f ), 0.5f ), ml7::Vector2( 0.0f, -1.5f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, -2.0f ).refracted( ml7::Vector2( 1.0f, 1.0f ), 1.0f ), ml7::Vector2( 1.0f, -2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, 0.0f ).comp( ml7::Vector2( 2.0f, -2.0f ) ), 2.0f );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, 0.0f ).proj( ml7::Vector2( 2.0f, -2.0f ) ), ml7::Vector2( 2.0f, 0.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, 2.0f ).clear(), ml7::Vector2( 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, 2.0f ).invert(), ml7::Vector2( -1.0f, -2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 2.0f, 0.0f ).normalize(), ml7::Vector2( 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 2.0f, 0.0f ).length( 3.0f ), ml7::Vector2( 3.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, -2.0f ).reflect( ml7::Vector2( 1.0f, 1.0f ) ), ml7::Vector2( 3.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, -2.0f ).refract( ml7::Vector2( 1.0f, 1.0f ), 0.0f ), ml7::Vector2( -1.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, -2.0f ).refract( ml7::Vector2( 1.0f, 1.0f ), 0.5f ), ml7::Vector2( 0.0f, -1.5f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, -2.0f ).refract( ml7::Vector2( 1.0f, 1.0f ), 1.0f ), ml7::Vector2( 1.0f, -2.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, 2.0f )[ 0 ], 1.0f );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, 2.0f )[ 1 ], 2.0f );

    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, 2.0f )[ 0 ] = 3.0f, 3.0f );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, 2.0f )[ 1 ] = 3.0f, 3.0f );

    TESTLABS_CHECK_EQ( +ml7::Vector2( 1.0f, -2.0f ), ml7::Vector2( 1.0f, -2.0f ) );
    TESTLABS_CHECK_EQ( -ml7::Vector2( 1.0f, -2.0f ), ml7::Vector2( -1.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 5.0f, -8.0f ) + ml7::Vector2( -6.0f, 9.0f ), ml7::Vector2( -1.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 5.0f, -8.0f ) - ml7::Vector2( -6.0f, 9.0f ), ml7::Vector2( 11.0f, -17.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, -2.0f ) * ml7::Vector2( -3.0f, 4.0f ), ml7::Vector2( -3.0f, -8.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( -3.0f, -8.0f ) / ml7::Vector2( -3.0f, 4.0f ), ml7::Vector2( 1.0f, -2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, -2.0f ) * 3.0f, ml7::Vector2( 3.0f, -6.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 3.0f, -6.0f ) / 3.0f, ml7::Vector2( 1.0f, -2.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector2( 5.0f, -8.0f ) += ml7::Vector2( -6.0f, 9.0f ), ml7::Vector2( -1.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 5.0f, -8.0f ) -= ml7::Vector2( -6.0f, 9.0f ), ml7::Vector2( 11.0f, -17.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, -2.0f ) *= ml7::Vector2( -3.0f, 4.0f ), ml7::Vector2( -3.0f, -8.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( -3.0f, -8.0f ) /= ml7::Vector2( -3.0f, 4.0f ), ml7::Vector2( 1.0f, -2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 1.0f, -2.0f ) *= 3.0f, ml7::Vector2( 3.0f, -6.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2( 3.0f, -6.0f ) /= 3.0f, ml7::Vector2( 1.0f, -2.0f ) );

    TESTLABS_CHECK( ml7::Vector2( 1.0f, -2.0f ) == ml7::Vector2( 1.0f, -2.0f ) );
    TESTLABS_CHECK( !(ml7::Vector2( 1.0f, -2.0f ) == ml7::Vector2( -1.0f, 2.0f )) );

    TESTLABS_CHECK( ml7::Vector2( 1.0f, -2.0f ) != ml7::Vector2( -1.0f, 2.0f ) );
    TESTLABS_CHECK( !(ml7::Vector2( 1.0f, -2.0f ) != ml7::Vector2( 1.0f, -2.0f )) );

    for ( float x = -1.0f; x <= +1.0f; x += 1.0f )
    for ( float y = -1.0f; y <= +1.0f; y += 1.0f )
    {
        TESTLABS_CHECK_EQ( ml7::Vector2( x, y ) == ml7::Vector2( 0.0f, 0.0f ), x == 0.0f && y == 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector2( x, y ) != ml7::Vector2( 0.0f, 0.0f ), x != 0.0f || y != 0.0f );

        TESTLABS_CHECK_EQ( ml7::Vector2( x, y ) < ml7::Vector2( 0.0f, 0.0f ), x < 0.0f && y < 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector2( x, y ) > ml7::Vector2( 0.0f, 0.0f ), x > 0.0f && y > 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector2( x, y ) <= ml7::Vector2( 0.0f, 0.0f ), x <= 0.0f && y <= 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector2( x, y ) >= ml7::Vector2( 0.0f, 0.0f ), x >= 0.0f && y >= 0.0f );
    }

    TESTLABS_CHECK_EQ( ml7::Vector2::min2( ml7::Vector2( 1.0f, -2.0f ), ml7::Vector2( -3.0f, 4.0f ) ), ml7::Vector2( -3.0f, -2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2::max2( ml7::Vector2( 1.0f, -2.0f ), ml7::Vector2( -3.0f, 4.0f ) ), ml7::Vector2( 1.0f, 4.0f ) );

    TESTLABS_CHECK( ml7::Vector2::ccw( ml7::Vector2( 1.0f, 0.0f ), ml7::Vector2( 2.0f, 0.0f ), ml7::Vector2( 3.0f, 0.0f ) ) == 0.0f );
    TESTLABS_CHECK( ml7::Vector2::ccw( ml7::Vector2( 1.0f, 0.0f ), ml7::Vector2( 2.0f, 0.0f ), ml7::Vector2( 3.0f, -1.0f ) ) < 0.0f );
    TESTLABS_CHECK( ml7::Vector2::ccw( ml7::Vector2( 1.0f, 0.0f ), ml7::Vector2( 2.0f, 0.0f ), ml7::Vector2( 3.0f, +1.0f ) ) > 0.0f );

    TESTLABS_CHECK_EQ( 5.0f * ml7::Vector2( 1.0f, -2.0f ), ml7::Vector2( 5.0f, -10.0f ) );
}



#endif // F77_TESTS_ML7_VECTOR2_H


#include <TestLabs/TestSuite.h>

#include <MathLabs/Vector2.h>

#include "./shared.h"



TESTLABS_CASE( u8"CoreLabs:  Vector2:  " )
{
    TESTLABS_CHECK_EQ( ml7::Vector2f(), ml7::Vector2f( 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f ), ml7::Vector2f( 1.0f, 1.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector2f( 3.0f, 4.0f ).length(), 5.0f );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 3.0f, 4.0f ).length_squared(), 25.0f );

    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f(  0.0f, +1.0f ).azimuth_symmetric(), 3 ), ml7::round( +0.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f( +1.0f, +1.0f ).azimuth_symmetric(), 3 ), ml7::round( +1.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f( +1.0f,  0.0f ).azimuth_symmetric(), 3 ), ml7::round( +2.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f( +1.0f, -1.0f ).azimuth_symmetric(), 3 ), ml7::round( +3.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f(  0.0f, -1.0f ).azimuth_symmetric(), 3 ), ml7::round( +4.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f( -1.0f, -1.0f ).azimuth_symmetric(), 3 ), ml7::round( -3.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f( -1.0f,  0.0f ).azimuth_symmetric(), 3 ), ml7::round( -2.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f( -1.0f, +1.0f ).azimuth_symmetric(), 3 ), ml7::round( -1.0f/8.0f * ml7::constantsf::pi2, 3 ) );

    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f(  0.0f, +1.0f ).azimuth_asymmetric(), 3 ), ml7::round( 0.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f( +1.0f, +1.0f ).azimuth_asymmetric(), 3 ), ml7::round( 1.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f( +1.0f,  0.0f ).azimuth_asymmetric(), 3 ), ml7::round( 2.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f( +1.0f, -1.0f ).azimuth_asymmetric(), 3 ), ml7::round( 3.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f(  0.0f, -1.0f ).azimuth_asymmetric(), 3 ), ml7::round( 4.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f( -1.0f, -1.0f ).azimuth_asymmetric(), 3 ), ml7::round( 5.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f( -1.0f,  0.0f ).azimuth_asymmetric(), 3 ), ml7::round( 6.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f( -1.0f, +1.0f ).azimuth_asymmetric(), 3 ), ml7::round( 7.0f/8.0f * ml7::constantsf::pi2, 3 ) );

    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f( +1.0f,  0.0f ).elevation(), 3 ), ml7::round( +0.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f( +1.0f, +1.0f ).elevation(), 3 ), ml7::round( +1.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f(  0.0f, +1.0f ).elevation(), 3 ), ml7::round( +2.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f( -1.0f, +1.0f ).elevation(), 3 ), ml7::round( +1.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f( -1.0f,  0.0f ).elevation(), 3 ), ml7::round( +0.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f( -1.0f, -1.0f ).elevation(), 3 ), ml7::round( -1.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f(  0.0f, -1.0f ).elevation(), 3 ), ml7::round( -2.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector2f( +1.0f, -1.0f ).elevation(), 3 ), ml7::round( -1.0f/8.0f * ml7::constantsf::pi2, 3 ) );

    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, 2.0f ).x_(), ml7::Vector2f( 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, 2.0f )._y(), ml7::Vector2f( 0.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, 2.0f ).xx(), ml7::Vector2f( 1.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, 2.0f ).yy(), ml7::Vector2f( 2.0f, 2.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector2f( 2.0f, 0.0f ).normalized(), ml7::Vector2f( 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( -1.0f, -2.0f ).abs(), ml7::Vector2f( 1.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, -2.0f ).dot( ml7::Vector2f( -1.0f, 2.0f ) ), -5.0f );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, -2.0f ).dot_squared( ml7::Vector2f( -1.0f, 2.0f ) ), 25.0f );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, 2.0f ).perp(), ml7::Vector2f( -2.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, -2.0f ).perpdot( ml7::Vector2f( -1.0f, 2.0f ) ), ml7::Vector2f( 1.0f, -2.0f ).perp().dot( ml7::Vector2f( -1.0f, 2.0f ) ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, -2.0f ).cross( ml7::Vector2f( -1.0f, 2.0f ) ), ml7::Vector2f( 1.0f, -2.0f ).perp().dot( ml7::Vector2f( -1.0f, 2.0f ) ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, -2.0f ).reflected( ml7::Vector2f( 1.0f, 1.0f ) ), ml7::Vector2f( 3.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, -2.0f ).refracted( ml7::Vector2f( 1.0f, 1.0f ), 0.0f ), ml7::Vector2f( -1.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, -2.0f ).refracted( ml7::Vector2f( 1.0f, 1.0f ), 0.5f ), ml7::Vector2f( 0.0f, -1.5f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, -2.0f ).refracted( ml7::Vector2f( 1.0f, 1.0f ), 1.0f ), ml7::Vector2f( 1.0f, -2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, 0.0f ).comp( ml7::Vector2f( 2.0f, -2.0f ) ), 2.0f );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, 0.0f ).proj( ml7::Vector2f( 2.0f, -2.0f ) ), ml7::Vector2f( 2.0f, 0.0f ) );

    TESTLABS_CHECK( ml7::Vector2f( 1.0f, 0.0f ).ccw( ml7::Vector2f( 2.0f, 0.0f ) ) == 0.0f );
    TESTLABS_CHECK( ml7::Vector2f( 1.0f, 0.0f ).ccw( ml7::Vector2f( 2.0f, -1.0f ) ) < 0.0f );
    TESTLABS_CHECK( ml7::Vector2f( 1.0f, 0.0f ).ccw( ml7::Vector2f( 2.0f, +1.0f ) ) > 0.0f );

    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, 2.0f ).clear(), ml7::Vector2f( 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, 2.0f ).invert(), ml7::Vector2f( -1.0f, -2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 2.0f, 0.0f ).normalize(), ml7::Vector2f( 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 2.0f, 0.0f ).length( 3.0f ), ml7::Vector2f( 3.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, -2.0f ).reflect( ml7::Vector2f( 1.0f, 1.0f ) ), ml7::Vector2f( 3.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, -2.0f ).refract( ml7::Vector2f( 1.0f, 1.0f ), 0.0f ), ml7::Vector2f( -1.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, -2.0f ).refract( ml7::Vector2f( 1.0f, 1.0f ), 0.5f ), ml7::Vector2f( 0.0f, -1.5f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, -2.0f ).refract( ml7::Vector2f( 1.0f, 1.0f ), 1.0f ), ml7::Vector2f( 1.0f, -2.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, 2.0f )[ 0 ], 1.0f );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, 2.0f )[ 1 ], 2.0f );

    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, 2.0f )[ 0 ] = 3.0f, 3.0f );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, 2.0f )[ 1 ] = 3.0f, 3.0f );

    TESTLABS_CHECK_EQ( +ml7::Vector2f( 1.0f, -2.0f ), ml7::Vector2f( 1.0f, -2.0f ) );
    TESTLABS_CHECK_EQ( -ml7::Vector2f( 1.0f, -2.0f ), ml7::Vector2f( -1.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 5.0f, -8.0f ) + ml7::Vector2f( -6.0f, 9.0f ), ml7::Vector2f( -1.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 5.0f, -8.0f ) - ml7::Vector2f( -6.0f, 9.0f ), ml7::Vector2f( 11.0f, -17.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, -2.0f ) * ml7::Vector2f( -3.0f, 4.0f ), ml7::Vector2f( -3.0f, -8.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( -3.0f, -8.0f ) / ml7::Vector2f( -3.0f, 4.0f ), ml7::Vector2f( 1.0f, -2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, -2.0f ) * 3.0f, ml7::Vector2f( 3.0f, -6.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 3.0f, -6.0f ) / 3.0f, ml7::Vector2f( 1.0f, -2.0f ) );
    TESTLABS_CHECK_EQ( 5.0f * ml7::Vector2f( 1.0f, -2.0f ), ml7::Vector2f( 5.0f, -10.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector2f( 5.0f, -8.0f ) += ml7::Vector2f( -6.0f, 9.0f ), ml7::Vector2f( -1.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 5.0f, -8.0f ) -= ml7::Vector2f( -6.0f, 9.0f ), ml7::Vector2f( 11.0f, -17.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, -2.0f ) *= ml7::Vector2f( -3.0f, 4.0f ), ml7::Vector2f( -3.0f, -8.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( -3.0f, -8.0f ) /= ml7::Vector2f( -3.0f, 4.0f ), ml7::Vector2f( 1.0f, -2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 1.0f, -2.0f ) *= 3.0f, ml7::Vector2f( 3.0f, -6.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f( 3.0f, -6.0f ) /= 3.0f, ml7::Vector2f( 1.0f, -2.0f ) );

    TESTLABS_CHECK( ml7::Vector2f( 1.0f, -2.0f ) == ml7::Vector2f( 1.0f, -2.0f ) );
    TESTLABS_CHECK( !(ml7::Vector2f( 1.0f, -2.0f ) == ml7::Vector2f( -1.0f, 2.0f )) );

    TESTLABS_CHECK( ml7::Vector2f( 1.0f, -2.0f ) != ml7::Vector2f( -1.0f, 2.0f ) );
    TESTLABS_CHECK( !(ml7::Vector2f( 1.0f, -2.0f ) != ml7::Vector2f( 1.0f, -2.0f )) );

    for ( float x = -1.0f; x <= +1.0f; x += 1.0f )
    for ( float y = -1.0f; y <= +1.0f; y += 1.0f )
    {
        TESTLABS_CHECK_EQ( ml7::Vector2f( x, y ) == ml7::Vector2f( 0.0f, 0.0f ), x == 0.0f && y == 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector2f( x, y ) != ml7::Vector2f( 0.0f, 0.0f ), x != 0.0f || y != 0.0f );

        TESTLABS_CHECK_EQ( ml7::Vector2f( x, y ) < ml7::Vector2f( 0.0f, 0.0f ), x < 0.0f && y < 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector2f( x, y ) > ml7::Vector2f( 0.0f, 0.0f ), x > 0.0f && y > 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector2f( x, y ) <= ml7::Vector2f( 0.0f, 0.0f ), x <= 0.0f && y <= 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector2f( x, y ) >= ml7::Vector2f( 0.0f, 0.0f ), x >= 0.0f && y >= 0.0f );
    }

    TESTLABS_CHECK_EQ( ml7::Vector2f::min2( ml7::Vector2f( 1.0f, -2.0f ), ml7::Vector2f( -3.0f, 4.0f ) ), ml7::Vector2f( -3.0f, -2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector2f::max2( ml7::Vector2f( 1.0f, -2.0f ), ml7::Vector2f( -3.0f, 4.0f ) ), ml7::Vector2f( 1.0f, 4.0f ) );

    TESTLABS_CHECK( ml7::Vector2f::ccw( ml7::Vector2f( 1.0f, 0.0f ), ml7::Vector2f( 2.0f, 0.0f ), ml7::Vector2f( 3.0f, 0.0f ) ) == 0.0f );
    TESTLABS_CHECK( ml7::Vector2f::ccw( ml7::Vector2f( 1.0f, 0.0f ), ml7::Vector2f( 2.0f, 0.0f ), ml7::Vector2f( 3.0f, -1.0f ) ) < 0.0f );
    TESTLABS_CHECK( ml7::Vector2f::ccw( ml7::Vector2f( 1.0f, 0.0f ), ml7::Vector2f( 2.0f, 0.0f ), ml7::Vector2f( 3.0f, +1.0f ) ) > 0.0f );
}

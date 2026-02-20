
#include <TestLabs/TestSuite.h>

#include <MathLabs/Vector3.h>

#include "./shared.h"



TESTLABS_CASE( u8"CoreLabs:  Vector3:  " )
{
    TESTLABS_CHECK_EQ( ml7::Vector3f(), ml7::Vector3f( 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f ), ml7::Vector3f( 1.0f, 1.0f, 1.0f ) );

    TESTLABS_CHECK_EQ( ml7::round( ml7::Vector3f( 1.0f, 2.0f, 3.0f ).length(), 3 ), ml7::round( 3.742f, 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::Vector3f( 1.0f, 2.0f, 3.0f ).length_squared(), 3 ), ml7::round( 14.0f, 3 ) );

    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f(  0.0f, 0.0f, +1.0f ).azimuth_symmetric(), 3 ), ml7::round( +0.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f( +1.0f, 0.0f, +1.0f ).azimuth_symmetric(), 3 ), ml7::round( +1.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f( +1.0f, 0.0f,  0.0f ).azimuth_symmetric(), 3 ), ml7::round( +2.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f( +1.0f, 0.0f, -1.0f ).azimuth_symmetric(), 3 ), ml7::round( +3.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f(  0.0f, 0.0f, -1.0f ).azimuth_symmetric(), 3 ), ml7::round( +4.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f( -1.0f, 0.0f, -1.0f ).azimuth_symmetric(), 3 ), ml7::round( -3.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f( -1.0f, 0.0f,  0.0f ).azimuth_symmetric(), 3 ), ml7::round( -2.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f( -1.0f, 0.0f, +1.0f ).azimuth_symmetric(), 3 ), ml7::round( -1.0f/8.0f * ml7::constantsf::pi2, 3 ) );

    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f(  0.0f, 0.0f, +1.0f ).azimuth_asymmetric(), 3 ), ml7::round( 0.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f( +1.0f, 0.0f, +1.0f ).azimuth_asymmetric(), 3 ), ml7::round( 1.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f( +1.0f, 0.0f,  0.0f ).azimuth_asymmetric(), 3 ), ml7::round( 2.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f( +1.0f, 0.0f, -1.0f ).azimuth_asymmetric(), 3 ), ml7::round( 3.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f(  0.0f, 0.0f, -1.0f ).azimuth_asymmetric(), 3 ), ml7::round( 4.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f( -1.0f, 0.0f, -1.0f ).azimuth_asymmetric(), 3 ), ml7::round( 5.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f( -1.0f, 0.0f,  0.0f ).azimuth_asymmetric(), 3 ), ml7::round( 6.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f( -1.0f, 0.0f, +1.0f ).azimuth_asymmetric(), 3 ), ml7::round( 7.0f/8.0f * ml7::constantsf::pi2, 3 ) );

    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f( +1.0f,  0.0f,  0.0f ).elevation(), 3 ), ml7::round( +0.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f( +1.0f, +1.0f,  0.0f ).elevation(), 3 ), ml7::round( +1.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f(  0.0f, +1.0f,  0.0f ).elevation(), 3 ), ml7::round( +2.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f(  0.0f, +1.0f, -1.0f ).elevation(), 3 ), ml7::round( +1.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f(  0.0f,  0.0f, -1.0f ).elevation(), 3 ), ml7::round( +0.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f(  0.0f, -1.0f, -1.0f ).elevation(), 3 ), ml7::round( -1.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f(  0.0f, -1.0f,  0.0f ).elevation(), 3 ), ml7::round( -2.0f/8.0f * ml7::constantsf::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::round( ml7::Vector3f( +1.0f, -1.0f,  0.0f ).elevation(), 3 ), ml7::round( -1.0f/8.0f * ml7::constantsf::pi2, 3 ) );

    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 2.0f, 3.0f ).x__(), ml7::Vector3f( 1.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 2.0f, 3.0f )._y_(), ml7::Vector3f( 0.0f, 2.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 2.0f, 3.0f ).__z(), ml7::Vector3f( 0.0f, 0.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 2.0f, 3.0f ).xy_(), ml7::Vector3f( 1.0f, 2.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 2.0f, 3.0f ).x_z(), ml7::Vector3f( 1.0f, 0.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 2.0f, 3.0f )._yz(), ml7::Vector3f( 0.0f, 2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 2.0f, 3.0f ).xxx(), ml7::Vector3f( 1.0f, 1.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 2.0f, 3.0f ).yyy(), ml7::Vector3f( 2.0f, 2.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 2.0f, 3.0f ).zzz(), ml7::Vector3f( 3.0f, 3.0f, 3.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector3f( 2.0f, 0.0f, 0.0f ).normalized(), ml7::Vector3f( 1.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( -1.0f, -2.0f, -3.0f ).abs(), ml7::Vector3f( 1.0f, 2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, -2.0f, 3.0f ).dot( ml7::Vector3f( -1.0f, 2.0f, -3.0f ) ), -14.0f );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, -2.0f, 3.0f ).dot_squared( ml7::Vector3f( -1.0f, 2.0f, -3.0f ) ), 196.0f );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 2.0f, 3.0f ).cross( ml7::Vector3f( -4.0f, -5.0f, -6.0f ) ), ml7::Vector3f( 3.0f, -6.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, -2.0f, 0.0f ).reflected( ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ), ml7::Vector3f( 3.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, -2.0f, 0.0f ).refracted( ml7::Vector3f( 1.0f, 1.0f, 0.0f ), 0.0f ), ml7::Vector3f( -1.0f, -1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, -2.0f, 0.0f ).refracted( ml7::Vector3f( 1.0f, 1.0f, 0.0f ), 0.5f ), ml7::Vector3f( 0.0f, -1.5f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, -2.0f, 0.0f ).refracted( ml7::Vector3f( 1.0f, 1.0f, 0.0f ), 1.0f ), ml7::Vector3f( 1.0f, -2.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 0.0f, 0.0f ).comp( ml7::Vector3f( 2.0f, -2.0f, 0.0f ) ), 2.0f );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 0.0f, 0.0f ).proj( ml7::Vector3f( 2.0f, -2.0f, 0.0f ) ), ml7::Vector3f( 2.0f, 0.0f, 0.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 2.0f, 3.0f ).clear(), ml7::Vector3f( 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 2.0f, 3.0f ).invert(), ml7::Vector3f( -1.0f, -2.0f, -3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 2.0f, 0.0f, 0.0f ).normalize(), ml7::Vector3f( 1.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 2.0f, 0.0f, 0.0f ).length( 3.0f ), ml7::Vector3f( 3.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, -2.0f, 0.0f ).reflect( ml7::Vector3f( 1.0f, 1.0f, 0.0f ) ), ml7::Vector3f( 3.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, -2.0f, 0.0f ).refract( ml7::Vector3f( 1.0f, 1.0f, 0.0f ), 0.0f ), ml7::Vector3f( -1.0f, -1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, -2.0f, 0.0f ).refract( ml7::Vector3f( 1.0f, 1.0f, 0.0f ), 0.5f ), ml7::Vector3f( 0.0f, -1.5f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, -2.0f, 0.0f ).refract( ml7::Vector3f( 1.0f, 1.0f, 0.0f ), 1.0f ), ml7::Vector3f( 1.0f, -2.0f, 0.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 2.0f, 3.0f )[ 0 ], 1.0f );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 2.0f, 3.0f )[ 1 ], 2.0f );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 2.0f, 3.0f )[ 2 ], 3.0f );

    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 2.0f, 3.0f )[ 0 ] = 4.0f, 4.0f );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 2.0f, 3.0f )[ 1 ] = 4.0f, 4.0f );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, 2.0f, 3.0f )[ 2 ] = 4.0f, 4.0f );

    TESTLABS_CHECK_EQ( +ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( -ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -1.0f, 2.0f, -3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 5.0f, -8.0f, 12.0f ) + ml7::Vector3f( -6.0f, 9.0f, -14.0f ), ml7::Vector3f( -1.0f, 1.0f, -2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 5.0f, -8.0f, 12.0f ) - ml7::Vector3f( -6.0f, 9.0f, -14.0f ), ml7::Vector3f( 11.0f, -17.0f, 26.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, -2.0f, 3.0f ) * ml7::Vector3f( -3.0f, 4.0f, -5.0f ), ml7::Vector3f( -3.0f, -8.0f, -15.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( -3.0f, -8.0f, -15.0f ) / ml7::Vector3f( -3.0f, 4.0f, -5.0f ), ml7::Vector3f( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, -2.0f, 3.0f ) * 4.0f, ml7::Vector3f( 4.0f, -8.0f, 12.0f  ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 4.0f, -8.0f, 12.0f  ) / 4.0f, ml7::Vector3f( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( 5.0f * ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( 5.0f, -10.0f, 15.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector3f( 5.0f, -8.0f, 12.0f ) += ml7::Vector3f( -6.0f, 9.0f, -14.0f ), ml7::Vector3f( -1.0f, 1.0f, -2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 5.0f, -8.0f, 12.0f ) -= ml7::Vector3f( -6.0f, 9.0f, -14.0f ), ml7::Vector3f( 11.0f, -17.0f, 26.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, -2.0f, 3.0f ) *= ml7::Vector3f( -3.0f, 4.0f, -5.0f ), ml7::Vector3f( -3.0f, -8.0f, -15.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( -3.0f, -8.0f, -15.0f ) /= ml7::Vector3f( -3.0f, 4.0f, -5.0f ), ml7::Vector3f( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 1.0f, -2.0f, 3.0f ) *= 4.0f, ml7::Vector3f( 4.0f, -8.0f, 12.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f( 4.0f, -8.0f, 12.0f ) /= 4.0f, ml7::Vector3f( 1.0f, -2.0f, 3.0f ) );

    TESTLABS_CHECK( ml7::Vector3f( 1.0f, -2.0f, 3.0f ) == ml7::Vector3f( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK( !(ml7::Vector3f( 1.0f, -2.0f, 3.0f ) == ml7::Vector3f( -1.0f, 2.0f, -3.0f )) );

    TESTLABS_CHECK( ml7::Vector3f( 1.0f, -2.0f, 3.0f ) != ml7::Vector3f( -1.0f, 2.0f, -3.0f ) );
    TESTLABS_CHECK( !(ml7::Vector3f( 1.0f, -2.0f, 3.0f ) != ml7::Vector3f( 1.0f, -2.0f, 3.0f )) );

    for ( float x = -1.0f; x <= +1.0f; x += 1.0f )
    for ( float y = -1.0f; y <= +1.0f; y += 1.0f )
    for ( float z = -1.0f; z <= +1.0f; z += 1.0f )
    {
        TESTLABS_CHECK_EQ( ml7::Vector3f( x, y, z ) == ml7::Vector3f( 0.0f, 0.0f, 0.0f ), x == 0.0f && y == 0.0f && z == 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector3f( x, y, z ) != ml7::Vector3f( 0.0f, 0.0f, 0.0f ), x != 0.0f || y != 0.0f || z != 0.0f );

        TESTLABS_CHECK_EQ( ml7::Vector3f( x, y, z ) < ml7::Vector3f( 0.0f, 0.0f, 0.0f ), x < 0.0f && y < 0.0f && z < 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector3f( x, y, z ) > ml7::Vector3f( 0.0f, 0.0f, 0.0f ), x > 0.0f && y > 0.0f && z > 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector3f( x, y, z ) <= ml7::Vector3f( 0.0f, 0.0f, 0.0f ), x <= 0.0f && y <= 0.0f && z <= 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector3f( x, y, z ) >= ml7::Vector3f( 0.0f, 0.0f, 0.0f ), x >= 0.0f && y >= 0.0f && z >= 0.0f );
    }

    TESTLABS_CHECK_EQ( ml7::Vector3f::min2( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -3.0f, 4.0f, -5.0f ) ), ml7::Vector3f( -3.0f, -2.0f, -5.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3f::max2( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -3.0f, 4.0f, -5.0f ) ), ml7::Vector3f( 1.0f, 4.0f, 3.0f ) );
}

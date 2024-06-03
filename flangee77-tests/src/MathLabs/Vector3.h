#pragma once
#ifndef F77_TESTS_ML7_VECTOR3_H
#define F77_TESTS_ML7_VECTOR3_H

#include <MathLabs/Vector3.h>

#include <MathLabs/constants.h>
#include <MathLabs/utilities.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



namespace cl7 {
    template <> inline
    cl7::string to_string(const ml7::Vector3& v) { return TEXT("{\"") + cl7::to_string(v.x) + TEXT(", ") + cl7::to_string(v.y) + TEXT(", ") + cl7::to_string(v.z) + TEXT("\"}"); }
}



TESTLABS_CASE( TEXT("CoreLabs:  Vector3:  ") )
{
    TESTLABS_CHECK_EQ( ml7::Vector3(), ml7::Vector3( 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f ), ml7::Vector3( 1.0f, 1.0f, 1.0f ) );

    TESTLABS_CHECK_EQ( ml7::utilities::round( ml7::Vector3( 1.0f, 2.0f, 3.0f ).length(), 3 ), ml7::utilities::round( 3.742f, 3 ) );
    TESTLABS_CHECK_EQ( ml7::utilities::round( ml7::Vector3( 1.0f, 2.0f, 3.0f ).lensqr(), 3 ), ml7::utilities::round( 14.0f, 3 ) );

    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3(  0.0f, 0.0f, +1.0f ).azimuth_symmetric(), 3 ), ml7::utilities::round( +0.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3( +1.0f, 0.0f, +1.0f ).azimuth_symmetric(), 3 ), ml7::utilities::round( +1.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3( +1.0f, 0.0f,  0.0f ).azimuth_symmetric(), 3 ), ml7::utilities::round( +2.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3( +1.0f, 0.0f, -1.0f ).azimuth_symmetric(), 3 ), ml7::utilities::round( +3.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3(  0.0f, 0.0f, -1.0f ).azimuth_symmetric(), 3 ), ml7::utilities::round( +4.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3( -1.0f, 0.0f, -1.0f ).azimuth_symmetric(), 3 ), ml7::utilities::round( -3.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3( -1.0f, 0.0f,  0.0f ).azimuth_symmetric(), 3 ), ml7::utilities::round( -2.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3( -1.0f, 0.0f, +1.0f ).azimuth_symmetric(), 3 ), ml7::utilities::round( -1.0f/8.0f * ml7::constants::pi2, 3 ) );

    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3(  0.0f, 0.0f, +1.0f ).azimuth_asymmetric(), 3 ), ml7::utilities::round( 0.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3( +1.0f, 0.0f, +1.0f ).azimuth_asymmetric(), 3 ), ml7::utilities::round( 1.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3( +1.0f, 0.0f,  0.0f ).azimuth_asymmetric(), 3 ), ml7::utilities::round( 2.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3( +1.0f, 0.0f, -1.0f ).azimuth_asymmetric(), 3 ), ml7::utilities::round( 3.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3(  0.0f, 0.0f, -1.0f ).azimuth_asymmetric(), 3 ), ml7::utilities::round( 4.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3( -1.0f, 0.0f, -1.0f ).azimuth_asymmetric(), 3 ), ml7::utilities::round( 5.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3( -1.0f, 0.0f,  0.0f ).azimuth_asymmetric(), 3 ), ml7::utilities::round( 6.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3( -1.0f, 0.0f, +1.0f ).azimuth_asymmetric(), 3 ), ml7::utilities::round( 7.0f/8.0f * ml7::constants::pi2, 3 ) );

    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3( +1.0f,  0.0f,  0.0f ).elevation(), 3 ), ml7::utilities::round( +0.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3( +1.0f, +1.0f,  0.0f ).elevation(), 3 ), ml7::utilities::round( +1.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3(  0.0f, +1.0f,  0.0f ).elevation(), 3 ), ml7::utilities::round( +2.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3(  0.0f, +1.0f, -1.0f ).elevation(), 3 ), ml7::utilities::round( +1.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3(  0.0f,  0.0f, -1.0f ).elevation(), 3 ), ml7::utilities::round( +0.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3(  0.0f, -1.0f, -1.0f ).elevation(), 3 ), ml7::utilities::round( -1.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3(  0.0f, -1.0f,  0.0f ).elevation(), 3 ), ml7::utilities::round( -2.0f/8.0f * ml7::constants::pi2, 3 ) );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( ml7::Vector3( +1.0f, -1.0f,  0.0f ).elevation(), 3 ), ml7::utilities::round( -1.0f/8.0f * ml7::constants::pi2, 3 ) );

    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, 2.0f, 3.0f ).x__(), ml7::Vector3( 1.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, 2.0f, 3.0f )._y_(), ml7::Vector3( 0.0f, 2.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, 2.0f, 3.0f ).__z(), ml7::Vector3( 0.0f, 0.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, 2.0f, 3.0f ).xy_(), ml7::Vector3( 1.0f, 2.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, 2.0f, 3.0f ).x_z(), ml7::Vector3( 1.0f, 0.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, 2.0f, 3.0f )._yz(), ml7::Vector3( 0.0f, 2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, 2.0f, 3.0f ).xxx(), ml7::Vector3( 1.0f, 1.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, 2.0f, 3.0f ).yyy(), ml7::Vector3( 2.0f, 2.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, 2.0f, 3.0f ).zzz(), ml7::Vector3( 3.0f, 3.0f, 3.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector3( 2.0f, 0.0f, 0.0f ).normalized(), ml7::Vector3( 1.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( -1.0f, -2.0f, -3.0f ).abs(), ml7::Vector3( 1.0f, 2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, -2.0f, 3.0f ).dot( ml7::Vector3( -1.0f, 2.0f, -3.0f ) ), -14.0f );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, -2.0f, 3.0f ).dotsqr( ml7::Vector3( -1.0f, 2.0f, -3.0f ) ), 196.0f );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, 2.0f, 3.0f ).cross( ml7::Vector3( -4.0f, -5.0f, -6.0f ) ), ml7::Vector3( 3.0f, -6.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, -2.0f, 0.0f ).reflected( ml7::Vector3( 1.0f, 1.0f, 0.0f ) ), ml7::Vector3( 3.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, -2.0f, 0.0f ).refracted( ml7::Vector3( 1.0f, 1.0f, 0.0f ), 0.0f ), ml7::Vector3( -1.0f, -1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, -2.0f, 0.0f ).refracted( ml7::Vector3( 1.0f, 1.0f, 0.0f ), 0.5f ), ml7::Vector3( 0.0f, -1.5f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, -2.0f, 0.0f ).refracted( ml7::Vector3( 1.0f, 1.0f, 0.0f ), 1.0f ), ml7::Vector3( 1.0f, -2.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, 0.0f, 0.0f ).comp( ml7::Vector3( 2.0f, -2.0f, 0.0f ) ), 2.0f );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, 0.0f, 0.0f ).proj( ml7::Vector3( 2.0f, -2.0f, 0.0f ) ), ml7::Vector3( 2.0f, 0.0f, 0.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, 2.0f, 3.0f ).clear(), ml7::Vector3( 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, 2.0f, 3.0f ).invert(), ml7::Vector3( -1.0f, -2.0f, -3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 2.0f, 0.0f, 0.0f ).normalize(), ml7::Vector3( 1.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 2.0f, 0.0f, 0.0f ).length( 3.0f ), ml7::Vector3( 3.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, -2.0f, 0.0f ).reflect( ml7::Vector3( 1.0f, 1.0f, 0.0f ) ), ml7::Vector3( 3.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, -2.0f, 0.0f ).refract( ml7::Vector3( 1.0f, 1.0f, 0.0f ), 0.0f ), ml7::Vector3( -1.0f, -1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, -2.0f, 0.0f ).refract( ml7::Vector3( 1.0f, 1.0f, 0.0f ), 0.5f ), ml7::Vector3( 0.0f, -1.5f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, -2.0f, 0.0f ).refract( ml7::Vector3( 1.0f, 1.0f, 0.0f ), 1.0f ), ml7::Vector3( 1.0f, -2.0f, 0.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, 2.0f, 3.0f )[ 0 ], 1.0f );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, 2.0f, 3.0f )[ 1 ], 2.0f );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, 2.0f, 3.0f )[ 2 ], 3.0f );

    TESTLABS_CHECK_EQ( +ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( -ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -1.0f, 2.0f, -3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, -2.0f, 3.0f ) + ml7::Vector3( -3.0f, 4.0f, -5.0f ), ml7::Vector3( -2.0f, 2.0f, -2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, -2.0f, 3.0f ) - ml7::Vector3( -3.0f, 4.0f, -5.0f ), ml7::Vector3( 4.0f, -6.0f, 8.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, -2.0f, 3.0f ) * ml7::Vector3( -3.0f, 4.0f, -5.0f ), ml7::Vector3( -3.0f, -8.0f, -15.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( -3.0f, -8.0f, -15.0f ) / ml7::Vector3( -3.0f, 4.0f, -5.0f ), ml7::Vector3( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, -2.0f, 3.0f ) * 5.0f, ml7::Vector3( 5.0f, -10.0f, 15.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 5.0f, -10.0f, 15.0f ) / 5.0f, ml7::Vector3( 1.0f, -2.0f, 3.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, -2.0f, 3.0f ) += ml7::Vector3( -3.0f, 4.0f, -5.0f ), ml7::Vector3( -2.0f, 2.0f, -2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, -2.0f, 3.0f ) -= ml7::Vector3( -3.0f, 4.0f, -5.0f ), ml7::Vector3( 4.0f, -6.0f, 8.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, -2.0f, 3.0f ) *= ml7::Vector3( -3.0f, 4.0f, -5.0f ), ml7::Vector3( -3.0f, -8.0f, -15.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( -3.0f, -8.0f, -15.0f ) /= ml7::Vector3( -3.0f, 4.0f, -5.0f ), ml7::Vector3( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 1.0f, -2.0f, 3.0f ) *= 5.0f, ml7::Vector3( 5.0f, -10.0f, 15.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3( 5.0f, -10.0f, 15.0f ) /= 5.0f, ml7::Vector3( 1.0f, -2.0f, 3.0f ) );

    TESTLABS_CHECK( ml7::Vector3( 1.0f, -2.0f, 3.0f ) == ml7::Vector3( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK( !(ml7::Vector3( 1.0f, -2.0f, 3.0f ) == ml7::Vector3( -1.0f, 2.0f, -3.0f )) );

    TESTLABS_CHECK( ml7::Vector3( 1.0f, -2.0f, 3.0f ) != ml7::Vector3( -1.0f, 2.0f, -3.0f ) );
    TESTLABS_CHECK( !(ml7::Vector3( 1.0f, -2.0f, 3.0f ) != ml7::Vector3( 1.0f, -2.0f, 3.0f )) );

    for ( float x = -1.0f; x <= +1.0f; x += 1.0f )
    for ( float y = -1.0f; y <= +1.0f; y += 1.0f )
    for ( float z = -1.0f; z <= +1.0f; z += 1.0f )
    {
        TESTLABS_CHECK_EQ( ml7::Vector3( x, y, z ) == ml7::Vector3( 0.0f, 0.0f, 0.0f ), x == 0.0f && y == 0.0f && z == 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector3( x, y, z ) != ml7::Vector3( 0.0f, 0.0f, 0.0f ), x != 0.0f || y != 0.0f || z != 0.0f );

        TESTLABS_CHECK_EQ( ml7::Vector3( x, y, z ) < ml7::Vector3( 0.0f, 0.0f, 0.0f ), x < 0.0f && y < 0.0f && z < 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector3( x, y, z ) > ml7::Vector3( 0.0f, 0.0f, 0.0f ), x > 0.0f && y > 0.0f && z > 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector3( x, y, z ) <= ml7::Vector3( 0.0f, 0.0f, 0.0f ), x <= 0.0f && y <= 0.0f && z <= 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector3( x, y, z ) >= ml7::Vector3( 0.0f, 0.0f, 0.0f ), x >= 0.0f && y >= 0.0f && z >= 0.0f );
    }

    TESTLABS_CHECK_EQ( ml7::Vector3::min2( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -3.0f, 4.0f, -5.0f ) ), ml7::Vector3( -3.0f, -2.0f, -5.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector3::max2( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -3.0f, 4.0f, -5.0f ) ), ml7::Vector3( 1.0f, 4.0f, 3.0f ) );
}



#endif // F77_TESTS_ML7_VECTOR3_H

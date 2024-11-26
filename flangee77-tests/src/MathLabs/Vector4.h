#pragma once
#ifndef F77_TESTS_ML7_VECTOR4_H
#define F77_TESTS_ML7_VECTOR4_H

#include <MathLabs/Vector4.h>

#include <MathLabs/constants.h>
#include <MathLabs/functions.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



namespace tl7 {
namespace internals {
    inline
    cl7::string to_string(const ml7::Vector4& v) { return TEXT("{ ") + cl7::to_string(v.x) + TEXT(", ") + cl7::to_string(v.y) + TEXT(", ") + cl7::to_string(v.z) + TEXT(", ") + cl7::to_string(v.w) + TEXT(" }"); }
}
}



TESTLABS_CASE( TEXT("CoreLabs:  Vector3:  ") )
{
    TESTLABS_CHECK_EQ( ml7::Vector4(), ml7::Vector4( 0.0f, 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 1.0f ), ml7::Vector4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 2.0f, 3.0f ), ml7::Vector4( 2.0f, 2.0f, 2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 2.0f, 4.0f, 6.0f ), ml7::Vector4( 2.0f, 4.0f, 6.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( ml7::Vector3( 2.0f, 4.0f, 6.0f ) ), ml7::Vector4( 2.0f, 4.0f, 6.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( ml7::Vector3( 2.0f, 4.0f, 6.0f ), 2.0f ), ml7::Vector4( 2.0f, 4.0f, 6.0f, 2.0f ) );

    TESTLABS_CHECK_EQ( ml7::round( ml7::Vector4( 1.0f, 2.0f, 3.0f, 4.0f ).length(), 3 ), ml7::round( 5.477f, 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::Vector4( 1.0f, 2.0f, 3.0f, 4.0f ).lensqr(), 3 ), ml7::round( 30.0f, 3 ) );

    TESTLABS_CHECK_EQ( ml7::Vector4( 2.0f, 4.0f, 6.0f, 2.0f ).dehomogenized(), ml7::Vector4( 1.0f, 2.0f, 3.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 2.0f, 0.0f, 0.0f, 0.0f ).normalized(), ml7::Vector4( 1.0f, 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( -1.0f, -2.0f, -3.0f, -4.0f ).abs(), ml7::Vector4( 1.0f, 2.0f, 3.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 1.0f, 2.0f, 3.0f, 0.0f ).to_vector3(), ml7::Vector3( 1.0f, 2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 1.0f, 2.0f, 3.0f, 1.0f ).to_vector3(), ml7::Vector3( 1.0f, 2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 2.0f, 4.0f, 6.0f, 2.0f ).to_vector3(), ml7::Vector3( 1.0f, 2.0f, 3.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector4( 1.0f, 2.0f, 3.0f, 4.0f ).clear(), ml7::Vector4( 0.0f, 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 1.0f, 2.0f, 3.0f, 4.0f ).invert(), ml7::Vector4( -1.0f, -2.0f, -3.0f, -4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 2.0f, 4.0f, 6.0f, 2.0f ).dehomogenize(), ml7::Vector4( 1.0f, 2.0f, 3.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 2.0f, 0.0f, 0.0f, 0.0f ).normalize(), ml7::Vector4( 1.0f, 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 2.0f, 0.0f, 0.0f, 0.0f ).length( 3.0f ), ml7::Vector4( 3.0f, 0.0f, 0.0f, 0.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector4( 1.0f, 2.0f, 3.0f, 4.0f )[ 0 ], 1.0f );
    TESTLABS_CHECK_EQ( ml7::Vector4( 1.0f, 2.0f, 3.0f, 4.0f )[ 1 ], 2.0f );
    TESTLABS_CHECK_EQ( ml7::Vector4( 1.0f, 2.0f, 3.0f, 4.0f )[ 2 ], 3.0f );
    TESTLABS_CHECK_EQ( ml7::Vector4( 1.0f, 2.0f, 3.0f, 4.0f )[ 3 ], 4.0f );

    TESTLABS_CHECK_EQ( ml7::Vector4( 1.0f, 2.0f, 3.0f, 4.0f )[ 0 ] = 5.0f, 5.0f );
    TESTLABS_CHECK_EQ( ml7::Vector4( 1.0f, 2.0f, 3.0f, 4.0f )[ 1 ] = 5.0f, 5.0f );
    TESTLABS_CHECK_EQ( ml7::Vector4( 1.0f, 2.0f, 3.0f, 4.0f )[ 2 ] = 5.0f, 5.0f );
    TESTLABS_CHECK_EQ( ml7::Vector4( 1.0f, 2.0f, 3.0f, 4.0f )[ 3 ] = 5.0f, 5.0f );

    TESTLABS_CHECK_EQ( +ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f ), ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f ) );
    TESTLABS_CHECK_EQ( -ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f ), ml7::Vector4( -1.0f, 2.0f, -3.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 5.0f, -8.0f, 12.0f, 7.0f ) + ml7::Vector4( -6.0f, 9.0f, -14.0f, 3.0f ), ml7::Vector4( -1.0f, 1.0f, -2.0f, 10.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 5.0f, -8.0f, 12.0f, 7.0f ) - ml7::Vector4( -6.0f, 9.0f, -14.0f, 3.0f ), ml7::Vector4( 11.0f, -17.0f, 26.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f ) * ml7::Vector4( -3.0f, 4.0f, -5.0f, 6.0f ), ml7::Vector4( -3.0f, -8.0f, -15.0f, -24.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( -3.0f, -8.0f, -15.0f, -24.0f ) / ml7::Vector4( -3.0f, 4.0f, -5.0f, 6.0f ), ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f ) * 5.0f, ml7::Vector4( 5.0f, -10.0f, 15.0f, -20.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 5.0f, -10.0f, 15.0f, -20.0f ) / 5.0f, ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f ) );

    TESTLABS_CHECK_EQ( ml7::Vector4( 5.0f, -8.0f, 12.0f, 7.0f ) += ml7::Vector4( -6.0f, 9.0f, -14.0f, 3.0f ), ml7::Vector4( -1.0f, 1.0f, -2.0f, 10.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 5.0f, -8.0f, 12.0f, 7.0f ) -= ml7::Vector4( -6.0f, 9.0f, -14.0f, 3.0f ), ml7::Vector4( 11.0f, -17.0f, 26.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f ) *= ml7::Vector4( -3.0f, 4.0f, -5.0f, 6.0f ), ml7::Vector4( -3.0f, -8.0f, -15.0f, -24.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( -3.0f, -8.0f, -15.0f, -24.0f ) /= ml7::Vector4( -3.0f, 4.0f, -5.0f, 6.0f ), ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f ) *= 5.0f, ml7::Vector4( 5.0f, -10.0f, 15.0f, -20.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4( 5.0f, -10.0f, 15.0f, -20.0f ) /= 5.0f, ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f ) );

    TESTLABS_CHECK( ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f ) == ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f ) );
    TESTLABS_CHECK( !(ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f ) == ml7::Vector4( -1.0f, 2.0f, -3.0f, 4.0f )) );

    TESTLABS_CHECK( ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f ) != ml7::Vector4( -1.0f, 2.0f, -3.0f, 4.0f ) );
    TESTLABS_CHECK( !(ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f ) != ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f )) );

    for ( float x = -1.0f; x <= +1.0f; x += 1.0f )
    for ( float y = -1.0f; y <= +1.0f; y += 1.0f )
    for ( float z = -1.0f; z <= +1.0f; z += 1.0f )
    for ( float w = -1.0f; w <= +1.0f; w += 1.0f )
    {
        TESTLABS_CHECK_EQ( ml7::Vector4( x, y, z, w ) == ml7::Vector4( 0.0f, 0.0f, 0.0f, 0.0f ), x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector4( x, y, z, w ) != ml7::Vector4( 0.0f, 0.0f, 0.0f, 0.0f ), x != 0.0f || y != 0.0f || z != 0.0f || w != 0.0f );

        TESTLABS_CHECK_EQ( ml7::Vector4( x, y, z, w ) < ml7::Vector4( 0.0f, 0.0f, 0.0f, 0.0f ), x < 0.0f && y < 0.0f && z < 0.0f && w < 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector4( x, y, z, w ) > ml7::Vector4( 0.0f, 0.0f, 0.0f, 0.0f ), x > 0.0f && y > 0.0f && z > 0.0f && w > 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector4( x, y, z, w ) <= ml7::Vector4( 0.0f, 0.0f, 0.0f, 0.0f ), x <= 0.0f && y <= 0.0f && z <= 0.0f && w <= 0.0f );
        TESTLABS_CHECK_EQ( ml7::Vector4( x, y, z, w ) >= ml7::Vector4( 0.0f, 0.0f, 0.0f, 0.0f ), x >= 0.0f && y >= 0.0f && z >= 0.0f && w >= 0.0f );
    }

    TESTLABS_CHECK_EQ( ml7::Vector4::min2( ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f ), ml7::Vector4( -3.0f, 4.0f, -5.0f, 6.0f ) ), ml7::Vector4( -3.0f, -2.0f, -5.0f, -4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Vector4::max2( ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f ), ml7::Vector4( -3.0f, 4.0f, -5.0f, 6.0f ) ), ml7::Vector4( 1.0f, 4.0f, 3.0f, 6.0f ) );

    TESTLABS_CHECK_EQ( 5.0f * ml7::Vector4( 1.0f, -2.0f, 3.0f, -4.0f ), ml7::Vector4( 5.0f, -10.0f, 15.0f, -20.0f ) );
}



#endif // F77_TESTS_ML7_VECTOR4_H

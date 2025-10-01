#ifndef F77_TESTS_ML7_QUATERNION_H
#define F77_TESTS_ML7_QUATERNION_H

#include <MathLabs/Quaternion.h>

#include <TestLabs/TestSuite.h>

#include "./shared.h"



namespace tl7::internals {
    inline
    cl7::u8string to_string(const ml7::QuaternionF& q) { return u8"{ ( " + cl7::to_string(q.x) + u8", " + cl7::to_string(q.y) + u8", " + cl7::to_string(q.z) + u8" ), " + cl7::to_string(q.w) + u8" }"; }
}



namespace ml7 {
    inline
    ml7::QuaternionF round(ml7::QuaternionF q, unsigned num_decimals) { for ( unsigned k = 0; k < 4; ++k ) q.data[k] = ml7::round(q.data[k], num_decimals); return q; }
}



TESTLABS_CASE( u8"CoreLabs:  Quaternion:  " )
{
    TESTLABS_CHECK_EQ( ml7::QuaternionF(), ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ) );

    TESTLABS_CHECK_EQ( ml7::QuaternionF( ml7::Vector3f( 1.0f, 2.0f, 3.0f ), 4.0f ), ml7::QuaternionF( 1.0f, 2.0f, 3.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( ml7::Matrix3x3f() ), ml7::QuaternionF() );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( ml7::Matrix3x3f( 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f ) ), ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( ml7::Matrix3x3f( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f ) ), 3 ), ml7::round( ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( ml7::Matrix3x3f( 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f ) ), 3 ), ml7::round( ml7::QuaternionF( 0.0f, 0.7071f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( ml7::Matrix3x3f( 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f ) ), 3 ), ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, 0.7071f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::QuaternionF::from_matrix3x3( ml7::Matrix3x3f() ), ml7::QuaternionF() );
    TESTLABS_CHECK_EQ( ml7::QuaternionF::from_matrix3x3( ml7::Matrix3x3f( 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f ) ), ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF::from_matrix3x3( ml7::Matrix3x3f( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f ) ), 3 ), ml7::round( ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF::from_matrix3x3( ml7::Matrix3x3f( 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f ) ), 3 ), ml7::round( ml7::QuaternionF( 0.0f, 0.7071f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF::from_matrix3x3( ml7::Matrix3x3f( 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f ) ), 3 ), ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, 0.7071f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::QuaternionF::from_axes( ml7::Vector3f( 1.0f, 0.0f, 0.0f ), ml7::Vector3f( 0.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, 0.0f, 1.0f ) ), ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuaternionF::from_axes( ml7::Vector3f( 1.0f, 0.0f, 0.0f ), ml7::Vector3f( 0.0f, 1.0f, 0.0f ), ml7::Vector3f( 0.0f, 0.0f, 1.0f ) ).transform( ml7::Vector3f( 1.0f, 2.0f, 3.0f ) ), ml7::Vector3f( 1.0f, 2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF::from_axes( ml7::Vector3f( 1.0f, 0.0f, 0.0f ), ml7::Vector3f( 0.0f, 0.0f, 1.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ) ), 3 ), ml7::round( ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF::from_axes( ml7::Vector3f( 1.0f, 0.0f, 0.0f ), ml7::Vector3f( 0.0f, 0.0f, 1.0f ), ml7::Vector3f( 0.0f, -1.0f, 0.0f ) ).transform( ml7::Vector3f( 1.0f, 2.0f, 3.0f ) ), 3 ), ml7::round( ml7::Vector3f( 1.0f, -3.0f, 2.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF::from_axes( ml7::Vector3f( 0.0f, 0.0f, -1.0f ), ml7::Vector3f( 0.0f, 1.0f, 0.0f ), ml7::Vector3f( 1.0f, 0.0f, 0.0f ) ), 3 ), ml7::round( ml7::QuaternionF( 0.0f, 0.7071f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF::from_axes( ml7::Vector3f( 0.0f, 0.0f, -1.0f ), ml7::Vector3f( 0.0f, 1.0f, 0.0f ), ml7::Vector3f( 1.0f, 0.0f, 0.0f ) ).transform( ml7::Vector3f( 1.0f, 2.0f, 3.0f ) ), 3 ), ml7::round( ml7::Vector3f( 3.0f, 2.0f, -1.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF::from_axes( ml7::Vector3f( 0.0f, 1.0f, 0.0f ), ml7::Vector3f( -1.0f, 0.0f, 0.0f ), ml7::Vector3f( 0.0f, 0.0f, 1.0f ) ), 3 ), ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF::from_axes( ml7::Vector3f( 0.0f, 1.0f, 0.0f ), ml7::Vector3f( -1.0f, 0.0f, 0.0f ), ml7::Vector3f( 0.0f, 0.0f, 1.0f ) ).transform( ml7::Vector3f( 1.0f, 2.0f, 3.0f ) ), 3 ), ml7::round( ml7::Vector3f( -2.0f, 1.0f, 3.0f ), 3 ) );

    // rotation etc. see below

    TESTLABS_CHECK_EQ( ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 0.0f ).is_invertible(), false );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 1.0f, 0.0f, 0.0f, 0.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 0.0f, 1.0f, 0.0f, 0.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 0.0f, 0.0f, 1.0f, 0.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 1.0f, 0.0f, 0.0f, 1.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 0.0f, 1.0f, 0.0f, 1.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 0.0f, 0.0f, 1.0f, 1.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, 0.7071f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 0.0f, 0.7071f, 0.0f, 0.7071f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, 0.7071f ).is_invertible(), true );

    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 0.0f ).norm(), 3 ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ).norm(), 3 ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 1.0f, 0.0f, 0.0f, 0.0f ).norm(), 3 ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 1.0f, 0.0f, 0.0f ).norm(), 3 ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 1.0f, 0.0f ).norm(), 3 ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 1.0f, 0.0f, 0.0f, 1.0f ).norm(), 3 ), 1.414f );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 1.0f, 0.0f, 1.0f ).norm(), 3 ), 1.414f );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 1.0f, 1.0f ).norm(), 3 ), 1.414f );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, 0.7071f ).norm(), 3 ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.7071f, 0.0f, 0.7071f ).norm(), 3 ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, 0.7071f ).norm(), 3 ), 1.0f );

    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ).normalized(), 3 ), ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 1.0f, 0.0f, 0.0f, 1.0f ).normalized(), 3 ), ml7::round( ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, -1.0f, 0.0f, 1.0f ).normalized(), 3 ), ml7::round( ml7::QuaternionF( 0.0f, -0.7071f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 1.0f, -1.0f ).normalized(), 3 ), ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, -0.7071f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ).conjugated(), ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 1.0f, 2.0f, 3.0f, 0.7071f ).conjugated(), ml7::QuaternionF( -1.0f, -2.0f, -3.0f, 0.7071f ) );

    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ).inverted(), 3 ), ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, 0.7071f ).inverted(), 3 ), ml7::round( ml7::QuaternionF( -0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.7071f, 0.0f, 0.7071f ).inverted(), 3 ), ml7::round( ml7::QuaternionF( 0.0f, -0.7071f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, 0.7071f ).inverted(), 3 ), ml7::round( ml7::QuaternionF( 0.0f, 0.0f, -0.7071f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.1889793f, 0.3779585f, 0.5669378f, 0.7071179f ).inverted(), 3 ), ml7::round( ml7::QuaternionF( -0.1889793f, -0.3779585f, -0.5669378f, 0.7071179f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( -0.1889793f, -0.3779585f, -0.5669378f, 0.7071179f ).inverted(), 3 ), ml7::round( ml7::QuaternionF( 0.1889793f, 0.3779585f, 0.5669378f, 0.7071179f ), 3 ) );

    // to_matrix3x3, to_axes, and to_axis_angle see below (with rotation etc.)

    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.2260934f, -0.7913268f, 0.5652334f, -0.0565233f ).transform( ml7::Vector3f( 1.0f, 2.0f, 3.0f ) ), 3 ), ml7::round( ml7::Vector3f( -0.4440895f, -2.5111822f, -2.7380192f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.2260934f, -0.7913268f, 0.5652334f, -0.0565233f ).transform_inverted( ml7::Vector3f( -0.4440895f, -2.5111822f, -2.7380192f ) ), 3 ), ml7::round( ml7::Vector3f( 1.0f, 2.0f, 3.0f ), 3 ) );

    // (further) transform etc. see below

    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ).normalize(), 3 ), ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 1.0f, 0.0f, 0.0f, 1.0f ).normalize(), 3 ), ml7::round( ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, -1.0f, 0.0f, 1.0f ).normalize(), 3 ), ml7::round( ml7::QuaternionF( 0.0f, -0.7071f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 1.0f, -1.0f ).normalize(), 3 ), ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, -0.7071f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ).conjugate(), ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 1.0f, 2.0f, 3.0f, 0.7071f ).conjugate(), ml7::QuaternionF( -1.0f, -2.0f, -3.0f, 0.7071f ) );

    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ).invert(), 3 ), ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, 0.7071f ).invert(), 3 ), ml7::round( ml7::QuaternionF( -0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.7071f, 0.0f, 0.7071f ).invert(), 3 ), ml7::round( ml7::QuaternionF( 0.0f, -0.7071f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, 0.7071f ).invert(), 3 ), ml7::round( ml7::QuaternionF( 0.0f, 0.0f, -0.7071f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.1889793f, 0.3779585f, 0.5669378f, 0.7071179f ).invert(), 3 ), ml7::round( ml7::QuaternionF( -0.1889793f, -0.3779585f, -0.5669378f, 0.7071179f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( -0.1889793f, -0.3779585f, -0.5669378f, 0.7071179f ).invert(), 3 ), ml7::round( ml7::QuaternionF( 0.1889793f, 0.3779585f, 0.5669378f, 0.7071179f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::QuaternionF( 1.0f, -2.0f, 3.0f, -4.0f ).dot( ml7::QuaternionF( -1.0f, 2.0f, -3.0f, 4.0f ) ), -30.0f );

    TESTLABS_CHECK_EQ( ml7::QuaternionF( 1.0f, 2.0f, 3.0f, 4.0f )[ 0 ], 1.0f );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 1.0f, 2.0f, 3.0f, 4.0f )[ 1 ], 2.0f );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 1.0f, 2.0f, 3.0f, 4.0f )[ 2 ], 3.0f );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 1.0f, 2.0f, 3.0f, 4.0f )[ 3 ], 4.0f );

    TESTLABS_CHECK_EQ( ml7::QuaternionF( 1.0f, 2.0f, 3.0f, 4.0f )[ 0 ] = 5.0f, 5.0f );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 1.0f, 2.0f, 3.0f, 4.0f )[ 1 ] = 5.0f, 5.0f );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 1.0f, 2.0f, 3.0f, 4.0f )[ 2 ] = 5.0f, 5.0f );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 1.0f, 2.0f, 3.0f, 4.0f )[ 3 ] = 5.0f, 5.0f );

    TESTLABS_CHECK_EQ( +ml7::QuaternionF( 1.0f, -2.0f, 3.0f, -4.0f ), ml7::QuaternionF( 1.0f, -2.0f, 3.0f, -4.0f ) );
    TESTLABS_CHECK_EQ( -ml7::QuaternionF( 1.0f, -2.0f, 3.0f, -4.0f ), ml7::QuaternionF( -1.0f, 2.0f, -3.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 5.0f, -8.0f, 12.0f, 7.0f ) + ml7::QuaternionF( -6.0f, 9.0f, -14.0f, 3.0f ), ml7::QuaternionF( -1.0f, 1.0f, -2.0f, 10.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 5.0f, -8.0f, 12.0f, 7.0f ) - ml7::QuaternionF( -6.0f, 9.0f, -14.0f, 3.0f ), ml7::QuaternionF( 11.0f, -17.0f, 26.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 1.0f, -2.0f, 3.0f, -4.0f ) * 5.0f, ml7::QuaternionF( 5.0f, -10.0f, 15.0f, -20.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 5.0f, -10.0f, 15.0f, -20.0f ) / 5.0f, ml7::QuaternionF( 1.0f, -2.0f, 3.0f, -4.0f ) );

    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ) * ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ), 3 ), ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, 0.7071f ) * ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ), ml7::round( ml7::QuaternionF( 1.0f, 0.0f, 0.0f, 0.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.7071f, 0.0f, 0.7071f ) * ml7::QuaternionF( 0.0f, 0.7071f, 0.0f, 0.7071f ), 3 ), ml7::round( ml7::QuaternionF( 0.0f, 1.0f, 0.0f, 0.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, 0.7071f ) * ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, 0.7071f ), 3 ), ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 1.0f, 0.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, 0.7071f ) * ml7::QuaternionF( 0.0f, 0.7071f, 0.0f, 0.7071f ), 3 ), ml7::round( ml7::QuaternionF( 0.5f, 0.5f, 0.5f, 0.5f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, 0.7071f ) * ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, 0.7071f ), 3 ), ml7::round( ml7::QuaternionF( 0.5f, -0.5f, 0.5f, 0.5f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.7071f, 0.0f, 0.7071f ) * ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ), ml7::round( ml7::QuaternionF( 0.5f, 0.5f, -0.5f, 0.5f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.7071f, 0.0f, 0.7071f ) * ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, 0.7071f ), 3 ), ml7::round( ml7::QuaternionF( 0.5f, 0.5f, 0.5f, 0.5f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, 0.7071f ) * ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ), ml7::round( ml7::QuaternionF( 0.5f, 0.5f, 0.5f, 0.5f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, 0.7071f ) * ml7::QuaternionF( 0.0f, 0.7071f, 0.0f, 0.7071f ), 3 ), ml7::round( ml7::QuaternionF( -0.5f, 0.5f, 0.5f, 0.5f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 1.0f, -2.0f, 3.0f, -4.0f ) * ml7::QuaternionF( -5.0f, 6.0f, -7.0f, 8.0f ), 3 ), ml7::round( ml7::QuaternionF( 24.0f, -48.0f, 48.0f, 6.0f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 0.2260934f, -0.7913268f, 0.5652334f, -0.0565233f ) * ml7::Vector3f( 1.0f, 2.0f, 3.0f ), 3 ), ml7::round( ml7::Vector3f( -0.4440895f, -2.5111822f, -2.7380192f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 1.0f, -2.0f, 3.0f, -4.0f ).normalized() * ml7::QuaternionF( -5.0f, 6.0f, -7.0f, 8.0f ).normalized() * ml7::Vector3f( 1.0f, 2.0f, 3.0f ), 3 ), ml7::round( ml7::Vector3f( -0.8758619f, -3.3517244f, -1.4137932f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 1.0f, -2.0f, 3.0f, -4.0f ).normalized() * (ml7::QuaternionF( -5.0f, 6.0f, -7.0f, 8.0f ).normalized() * ml7::Vector3f( 1.0f, 2.0f, 3.0f )), 3 ), ml7::round( ml7::Vector3f( -0.8758619f, -3.3517244f, -1.4137932f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( (ml7::QuaternionF( 1.0f, -2.0f, 3.0f, -4.0f ).normalized() * ml7::QuaternionF( -5.0f, 6.0f, -7.0f, 8.0f ).normalized()) * ml7::Vector3f( 1.0f, 2.0f, 3.0f ), 3 ), ml7::round( ml7::Vector3f( -0.8758619f, -3.3517244f, -1.4137932f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF( 24.0f, -48.0f, 48.0f, 6.0f ).normalized() * ml7::Vector3f( 1.0f, 2.0f, 3.0f ), 3 ), ml7::round( ml7::Vector3f( -0.8758619f, -3.3517244f, -1.4137932f ), 3 ) );

    TESTLABS_CHECK_EQ( 5.0f * ml7::QuaternionF( 1.0f, -2.0f, 3.0f, -4.0f ), ml7::QuaternionF( 5.0f, -10.0f, 15.0f, -20.0f ) );

    TESTLABS_CHECK_EQ( ml7::QuaternionF( 5.0f, -8.0f, 12.0f, 7.0f ) += ml7::QuaternionF( -6.0f, 9.0f, -14.0f, 3.0f ), ml7::QuaternionF( -1.0f, 1.0f, -2.0f, 10.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 5.0f, -8.0f, 12.0f, 7.0f ) -= ml7::QuaternionF( -6.0f, 9.0f, -14.0f, 3.0f ), ml7::QuaternionF( 11.0f, -17.0f, 26.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 1.0f, -2.0f, 3.0f, -4.0f ) *= 5.0f, ml7::QuaternionF( 5.0f, -10.0f, 15.0f, -20.0f ) );
    TESTLABS_CHECK_EQ( ml7::QuaternionF( 5.0f, -10.0f, 15.0f, -20.0f ) /= 5.0f, ml7::QuaternionF( 1.0f, -2.0f, 3.0f, -4.0f ) );

    TESTLABS_CHECK( ml7::QuaternionF( 1.0f, -2.0f, 3.0f, -4.0f ) == ml7::QuaternionF( 1.0f, -2.0f, 3.0f, -4.0f ) );
    TESTLABS_CHECK( !(ml7::QuaternionF( 1.0f, -2.0f, 3.0f, -4.0f ) == ml7::QuaternionF( -1.0f, 2.0f, -3.0f, 4.0f )) );

    TESTLABS_CHECK( ml7::QuaternionF( 1.0f, -2.0f, 3.0f, -4.0f ) != ml7::QuaternionF( -1.0f, 2.0f, -3.0f, 4.0f ) );
    TESTLABS_CHECK( !(ml7::QuaternionF( 1.0f, -2.0f, 3.0f, -4.0f ) != ml7::QuaternionF( 1.0f, -2.0f, 3.0f, -4.0f )) );
}



TESTLABS_CASE( u8"CoreLabs:  Quaternion:  rotx" )
{
    struct Entry
    {
        float angle;
        ml7::QuaternionF expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ) },
        { 1.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, 0.7071f ) },
        { 2.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 1.0f, 0.0f, 0.0f, 0.0f ) },
        { 3.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, -0.7071f ) },
        { 4.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.0f, -1.0f ) },
        { -1.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( -0.7071f, 0.0f, 0.0f, 0.7071f ) },
        { -2.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( -1.0f, 0.0f, 0.0f, 0.0f ) },
        { -3.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( -0.7071f, 0.0f, 0.0f, -0.7071f ) },
    };

    TESTLABS_SUBCASE_BATCH( u8"rotx", container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF::rotx( entry.angle ), 3 ), ml7::round( entry.expected, 3 ) );
    }
}

TESTLABS_CASE( u8"CoreLabs:  Quaternion:  roty" )
{
    struct Entry
    {
        float angle;
        ml7::QuaternionF expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ) },
        { 1.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.7071f, 0.0f, 0.7071f ) },
        { 2.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 1.0f, 0.0f, 0.0f ) },
        { 3.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.7071f, 0.0f, -0.7071f ) },
        { 4.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.0f, -1.0f ) },
        { -1.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, -0.7071f, 0.0f, 0.7071f ) },
        { -2.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, -1.0f, 0.0f, 0.0f ) },
        { -3.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, -0.7071f, 0.0f, -0.7071f ) },
    };

    TESTLABS_SUBCASE_BATCH( u8"roty", container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF::roty( entry.angle ), 3 ), ml7::round( entry.expected, 3 ) );
    }
}

TESTLABS_CASE( u8"CoreLabs:  Quaternion:  rotz" )
{
    struct Entry
    {
        float angle;
        ml7::QuaternionF expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ) },
        { 1.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, 0.7071f ) },
        { 2.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 1.0f, 0.0f ) },
        { 3.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, -0.7071f ) },
        { 4.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.0f, -1.0f ) },
        { -1.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, -0.7071f, 0.7071f ) },
        { -2.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, -1.0f, 0.0f ) },
        { -3.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, -0.7071f, -0.7071f ) },
    };

    TESTLABS_SUBCASE_BATCH( u8"rotz", container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF::rotz( entry.angle ), 3 ), ml7::round( entry.expected, 3 ) );
    }
}

TESTLABS_CASE( u8"CoreLabs:  Quaternion:  rotation etc." )
{
    struct Entry
    {
        ml7::Vector3f axis;
        float angle;
        ml7::QuaternionF expected;
    } entry;

    const std::vector<Entry> container {
        { ml7::Vector3f::X, 0.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ) },
        { ml7::Vector3f::X, 1.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, 0.7071f ) },
        { ml7::Vector3f::X, 2.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 1.0f, 0.0f, 0.0f, 0.0f ) },
        { ml7::Vector3f::X, 3.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, -0.7071f ) },
        { ml7::Vector3f::X, 4.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.0f, -1.0f ) },
        { ml7::Vector3f::X, -1.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( -0.7071f, 0.0f, 0.0f, 0.7071f ) },
        { ml7::Vector3f::X, -2.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( -1.0f, 0.0f, 0.0f, 0.0f ) },
        { ml7::Vector3f::X, -3.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( -0.7071f, 0.0f, 0.0f, -0.7071f ) },

        { ml7::Vector3f::Y, 0.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ) },
        { ml7::Vector3f::Y, 1.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.7071f, 0.0f, 0.7071f ) },
        { ml7::Vector3f::Y, 2.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 1.0f, 0.0f, 0.0f ) },
        { ml7::Vector3f::Y, 3.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.7071f, 0.0f, -0.7071f ) },
        { ml7::Vector3f::Y, 4.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.0f, -1.0f ) },
        { ml7::Vector3f::Y, -1.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, -0.7071f, 0.0f, 0.7071f ) },
        { ml7::Vector3f::Y, -2.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, -1.0f, 0.0f, 0.0f ) },
        { ml7::Vector3f::Y, -3.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, -0.7071f, 0.0f, -0.7071f ) },

        { ml7::Vector3f::Z, 0.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ) },
        { ml7::Vector3f::Z, 1.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, 0.7071f ) },
        { ml7::Vector3f::Z, 2.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 1.0f, 0.0f ) },
        { ml7::Vector3f::Z, 3.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, -0.7071f ) },
        { ml7::Vector3f::Z, 4.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.0f, -1.0f ) },
        { ml7::Vector3f::Z, -1.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, -0.7071f, 0.7071f ) },
        { ml7::Vector3f::Z, -2.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, -1.0f, 0.0f ) },
        { ml7::Vector3f::Z, -3.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, -0.7071f, -0.7071f ) },

        { -ml7::Vector3f::X, -0.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ) },
        { -ml7::Vector3f::X, -1.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, 0.7071f ) },
        { -ml7::Vector3f::X, -2.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 1.0f, 0.0f, 0.0f, 0.0f ) },
        { -ml7::Vector3f::X, -3.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.7071f, 0.0f, 0.0f, -0.7071f ) },
        { -ml7::Vector3f::X, -4.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.0f, -1.0f ) },
        { -ml7::Vector3f::X, 1.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( -0.7071f, 0.0f, 0.0f, 0.7071f ) },
        { -ml7::Vector3f::X, 2.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( -1.0f, 0.0f, 0.0f, 0.0f ) },
        { -ml7::Vector3f::X, 3.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( -0.7071f, 0.0f, 0.0f, -0.7071f ) },

        { -ml7::Vector3f::Y, -0.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ) },
        { -ml7::Vector3f::Y, -1.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.7071f, 0.0f, 0.7071f ) },
        { -ml7::Vector3f::Y, -2.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 1.0f, 0.0f, 0.0f ) },
        { -ml7::Vector3f::Y, -3.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.7071f, 0.0f, -0.7071f ) },
        { -ml7::Vector3f::Y, -4.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.0f, -1.0f ) },
        { -ml7::Vector3f::Y, 1.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, -0.7071f, 0.0f, 0.7071f ) },
        { -ml7::Vector3f::Y, 2.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, -1.0f, 0.0f, 0.0f ) },
        { -ml7::Vector3f::Y, 3.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, -0.7071f, 0.0f, -0.7071f ) },

        { -ml7::Vector3f::Z, -0.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.0f, 1.0f ) },
        { -ml7::Vector3f::Z, -1.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, 0.7071f ) },
        { -ml7::Vector3f::Z, -2.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 1.0f, 0.0f ) },
        { -ml7::Vector3f::Z, -3.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.7071f, -0.7071f ) },
        { -ml7::Vector3f::Z, -4.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, 0.0f, -1.0f ) },
        { -ml7::Vector3f::Z, 1.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, -0.7071f, 0.7071f ) },
        { -ml7::Vector3f::Z, 2.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, -1.0f, 0.0f ) },
        { -ml7::Vector3f::Z, 3.0f/4.0f * ml7::constantsf::pi2, ml7::QuaternionF( 0.0f, 0.0f, -0.7071f, -0.7071f ) },

        { ml7::Vector3f( 1.0f, 1.0f, 1.0f ) * 1.0f/::sqrtf(3.0f), ml7::constantsf::pi * 1.0f/6.0f, ml7::QuaternionF( 0.1494293f, 0.1494293f, 0.1494293f, 0.9659258f ) },
        { ml7::Vector3f( 1.0f, 0.0f, 1.0f ) * 1.0f/::sqrtf(2.0f), ml7::constantsf::pi * 1.0f/4.0f, ml7::QuaternionF( 0.270598f, 0.0f, 0.270598f, 0.9238796f ) },
        { ml7::Vector3f( 1.0f, 2.0f, 0.0f ) * 1.0f/::sqrtf(5.0f), ml7::constantsf::pi * 1.0f/3.0f, ml7::QuaternionF( 0.2236075f, 0.4472144f, 0.0f, 0.8660248f ) },
        { ml7::Vector3f( 3.0f, 1.0f, 2.0f ) * 1.0f/::sqrtf(10.0f), ml7::constantsf::pi * 1.0f/6.0f, ml7::QuaternionF( 0.2075169f, 0.0691724f, 0.1383448f, 0.9659258f ) },
        { ml7::Vector3f( 2.0f, 1.0f, -1.0f ) * 1.0f/::sqrtf(6.0f), ml7::constantsf::pi * 2.0f/3.0f, ml7::QuaternionF( 0.7071081f, 0.3535536f, -0.3535536f, 0.4999979f ) },
        { ml7::Vector3f( 3.0f, 2.0f, 1.0f ) * 1.0f/::sqrtf(14.0f), ml7::constantsf::pi * 1.0f/2.0f, ml7::QuaternionF( 0.566948f, 0.3779649f, 0.1889824f, 0.7071055f ) },
        { ml7::Vector3f( 2.0f, 2.0f, 3.0f ) * 1.0f/::sqrtf(11.0f), ml7::constantsf::pi * 3.0f/4.0f, ml7::QuaternionF( 0.4481471f, 0.4481471f, 0.6722203f, 0.3826855f ) },
        { ml7::Vector3f( 2.0f, -3.0f, 4.0f ) * 1.0f/::sqrtf(29.0f), ml7::constantsf::pi * 1.0f/8.0f, ml7::QuaternionF( 0.0724548f, -0.1086821f, 0.1449094f, 0.9807853f ) },
        { ml7::Vector3f( 1.0f, 2.0f, 4.0f ) * 1.0f/::sqrtf(19.0f), ml7::constantsf::pi * 1.0f/5.0f, ml7::QuaternionF( 0.0674332f, 0.134866f, 0.2697324f, 0.9510564f ) },
        { ml7::Vector3f( 1.0f, 2.0f, -3.0f ) * 1.0f/::sqrtf(15.0f), ml7::constantsf::pi * 2.0f/5.0f, ml7::QuaternionF( 0.1570925f, 0.3141851f, -0.4712776f, 0.8090161f ) },
        { ml7::Vector3f( 4.0f, 1.0f, -2.0f ) * 1.0f/::sqrtf(21.0f), ml7::constantsf::pi * 7.0f/8.0f, ml7::QuaternionF( 0.8560993f, 0.2140248f, -0.4280496f, 0.1950921f ) },
        { ml7::Vector3f( 2.0f, 2.0f, -3.0f ) * 1.0f/::sqrtf(13.0f), ml7::constantsf::pi * 1.0f/7.0f, ml7::QuaternionF( 0.1079385f, 0.1079385f, -0.1619078f, 0.9749279f ) },
    };

    TESTLABS_SUBCASE_BATCH( u8"rotation (axis with angle magnitude)", container, entry )
    {
        ml7::Vector3f axis = entry.axis.normalized() * entry.angle;
        TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF::rotation( axis ), 3 ), ml7::round( entry.expected, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( u8"rotation (axis-angle)", container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF::rotation( entry.axis, entry.angle ), 3 ), ml7::round( entry.expected, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( u8"rotation_normalized", container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF::rotation_normalized( entry.axis.normalized(), entry.angle ), 3 ), ml7::round( entry.expected, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( u8"to_matrix3x3, to_axes, and to_axis_angle", container, entry )
    {
        ml7::Matrix3x3f m = ml7::Matrix3x3f::rotation( entry.axis, entry.angle );
        ml7::QuaternionF q = ml7::QuaternionF::rotation( entry.axis, entry.angle );

        TESTLABS_CHECK_EQ( ml7::round( q.to_matrix3x3(), 3 ), ml7::round( m, 3 ) );

        ml7::Vector3f x, y, z;
        q.to_axes( x, y, z );
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x3f::from_axes( x, y, z ), 2 ), ml7::round( m, 2 ) ); // Allow a larger rounding error than before!

        ml7::Vector3f axis;
        float angle;
        TESTLABS_CHECK_EQ( q.to_axis_angle( axis, angle ), entry.angle != 0.0f );
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x3f::rotation( axis, angle ), 2 ), ml7::round( m, 2 ) ); // Allow a larger rounding error than before!
    }
}



TESTLABS_CASE( u8"CoreLabs:  Quaternion:  transform etc." )
{
    struct Entry
    {
        ml7::Vector3f axis;
        float angle;
        ml7::Vector3f untransformed;
        ml7::Vector3f transformed;
    } entry;

    const std::vector<Entry> container {
        { ml7::Vector3f( 1.0f, 0.0f, 0.0f ), 0.0f/4.0f * ml7::constantsf::pi2, ml7::Vector3f( 1.0f, 2.0f, 3.0f ), ml7::Vector3f( 1.0f, 2.0f, 3.0f ) },
        { ml7::Vector3f( 1.0f, 1.0f, 1.0f ), 1.0f/4.0f * ml7::constantsf::pi2, ml7::Vector3f( 1.0f, 2.0f, 3.0f ), ml7::Vector3f( 2.5774f, 0.8453f, 2.5774f ) },
        // ...
    };

    TESTLABS_SUBCASE_BATCH( u8"transform", container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF::rotation( entry.axis, entry.angle ).transform( entry.untransformed ), 3 ), ml7::round( entry.transformed, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( u8"transform_inverted", container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::QuaternionF::rotation( entry.axis, entry.angle ).transform_inverted( entry.transformed ), 3 ), ml7::round( entry.untransformed, 3 ) );
    }
}



#endif // F77_TESTS_ML7_QUATERNION_H

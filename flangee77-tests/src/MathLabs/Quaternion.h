#pragma once
#ifndef F77_TESTS_ML7_QUATERNION_H
#define F77_TESTS_ML7_QUATERNION_H

#include <MathLabs/Quaternion.h>

#include <MathLabs/constants.h>
#include <MathLabs/functions.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



namespace tl7 {
namespace internals {
    inline
    cl7::string to_string(const ml7::Quaternion& q) { return TEXT("{ ( ") + cl7::to_string(q.x) + TEXT(", ") + cl7::to_string(q.y) + TEXT(", ") + cl7::to_string(q.z) + TEXT(" ), ") + cl7::to_string(q.w) + TEXT(" }"); }
}
}



namespace _ {
    ml7::Quaternion round(ml7::Quaternion q, unsigned num_decimals) { for ( unsigned k = 0; k < 4; ++k ) q.data[k] = ml7::round(q.data[k], num_decimals); return q; }
}



TESTLABS_CASE( TEXT("CoreLabs:  Quaternion:  ") )
{
    TESTLABS_CHECK_EQ( ml7::Quaternion(), ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ) );

    TESTLABS_CHECK_EQ( ml7::Quaternion( ml7::Vector3( 1.0f, 2.0f, 3.0f ), 4.0f ), ml7::Quaternion( 1.0f, 2.0f, 3.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Quaternion( ml7::Matrix3x3() ), ml7::Quaternion() );
    TESTLABS_CHECK_EQ( ml7::Quaternion( ml7::Matrix3x3( 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f ) ), ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( ml7::Matrix3x3( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f ) ), 3 ), _::round( ml7::Quaternion( 0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( ml7::Matrix3x3( 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f ) ), 3 ), _::round( ml7::Quaternion( 0.0f, 0.7071f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( ml7::Matrix3x3( 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f ) ), 3 ), _::round( ml7::Quaternion( 0.0f, 0.0f, 0.7071f, 0.7071f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::Quaternion::from_matrix3x3( ml7::Matrix3x3() ), ml7::Quaternion() );
    TESTLABS_CHECK_EQ( ml7::Quaternion::from_matrix3x3( ml7::Matrix3x3( 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f ) ), ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion::from_matrix3x3( ml7::Matrix3x3( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f ) ), 3 ), _::round( ml7::Quaternion( 0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion::from_matrix3x3( ml7::Matrix3x3( 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f ) ), 3 ), _::round( ml7::Quaternion( 0.0f, 0.7071f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion::from_matrix3x3( ml7::Matrix3x3( 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f ) ), 3 ), _::round( ml7::Quaternion( 0.0f, 0.0f, 0.7071f, 0.7071f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::Quaternion::from_axes( ml7::Vector3( 1.0f, 0.0f, 0.0f ), ml7::Vector3( 0.0f, 1.0f, 0.0f ), ml7::Vector3( 0.0f, 0.0f, 1.0f ) ), ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Quaternion::from_axes( ml7::Vector3( 1.0f, 0.0f, 0.0f ), ml7::Vector3( 0.0f, 1.0f, 0.0f ), ml7::Vector3( 0.0f, 0.0f, 1.0f ) ).transform( ml7::Vector3( 1.0f, 2.0f, 3.0f ) ), ml7::Vector3( 1.0f, 2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion::from_axes( ml7::Vector3( 1.0f, 0.0f, 0.0f ), ml7::Vector3( 0.0f, 0.0f, 1.0f ), ml7::Vector3( 0.0f, -1.0f, 0.0f ) ), 3 ), _::round( ml7::Quaternion( 0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion::from_axes( ml7::Vector3( 1.0f, 0.0f, 0.0f ), ml7::Vector3( 0.0f, 0.0f, 1.0f ), ml7::Vector3( 0.0f, -1.0f, 0.0f ) ).transform( ml7::Vector3( 1.0f, 2.0f, 3.0f ) ), 3 ), _::round( ml7::Vector3( 1.0f, -3.0f, 2.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion::from_axes( ml7::Vector3( 0.0f, 0.0f, -1.0f ), ml7::Vector3( 0.0f, 1.0f, 0.0f ), ml7::Vector3( 1.0f, 0.0f, 0.0f ) ), 3 ), _::round( ml7::Quaternion( 0.0f, 0.7071f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion::from_axes( ml7::Vector3( 0.0f, 0.0f, -1.0f ), ml7::Vector3( 0.0f, 1.0f, 0.0f ), ml7::Vector3( 1.0f, 0.0f, 0.0f ) ).transform( ml7::Vector3( 1.0f, 2.0f, 3.0f ) ), 3 ), _::round( ml7::Vector3( 3.0f, 2.0f, -1.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion::from_axes( ml7::Vector3( 0.0f, 1.0f, 0.0f ), ml7::Vector3( -1.0f, 0.0f, 0.0f ), ml7::Vector3( 0.0f, 0.0f, 1.0f ) ), 3 ), _::round( ml7::Quaternion( 0.0f, 0.0f, 0.7071f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion::from_axes( ml7::Vector3( 0.0f, 1.0f, 0.0f ), ml7::Vector3( -1.0f, 0.0f, 0.0f ), ml7::Vector3( 0.0f, 0.0f, 1.0f ) ).transform( ml7::Vector3( 1.0f, 2.0f, 3.0f ) ), 3 ), _::round( ml7::Vector3( -2.0f, 1.0f, 3.0f ), 3 ) );

    // rotation etc. see below

    TESTLABS_CHECK_EQ( ml7::Quaternion( 0.0f, 0.0f, 0.0f, 0.0f ).is_invertible(), false );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 1.0f, 0.0f, 0.0f, 0.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 0.0f, 1.0f, 0.0f, 0.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 0.0f, 0.0f, 1.0f, 0.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 1.0f, 0.0f, 0.0f, 1.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 0.0f, 1.0f, 0.0f, 1.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 0.0f, 0.0f, 1.0f, 1.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 0.7071f, 0.0f, 0.0f, 0.7071f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 0.0f, 0.7071f, 0.0f, 0.7071f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 0.0f, 0.0f, 0.7071f, 0.7071f ).is_invertible(), true );

    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.0f, 0.0f, 0.0f ).norm(), 3 ), 0.0f );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ).norm(), 3 ), 1.0f );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 1.0f, 0.0f, 0.0f, 0.0f ).norm(), 3 ), 1.0f );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 1.0f, 0.0f, 0.0f ).norm(), 3 ), 1.0f );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.0f, 1.0f, 0.0f ).norm(), 3 ), 1.0f );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 1.0f, 0.0f, 0.0f, 1.0f ).norm(), 3 ), 1.414f );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 1.0f, 0.0f, 1.0f ).norm(), 3 ), 1.414f );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.0f, 1.0f, 1.0f ).norm(), 3 ), 1.414f );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.7071f, 0.0f, 0.0f, 0.7071f ).norm(), 3 ), 1.0f );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.7071f, 0.0f, 0.7071f ).norm(), 3 ), 1.0f );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.0f, 0.7071f, 0.7071f ).norm(), 3 ), 1.0f );

    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ).normalized(), 3 ), _::round( ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 1.0f, 0.0f, 0.0f, 1.0f ).normalized(), 3 ), _::round( ml7::Quaternion( 0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, -1.0f, 0.0f, 1.0f ).normalized(), 3 ), _::round( ml7::Quaternion( 0.0f, -0.7071f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.0f, 1.0f, -1.0f ).normalized(), 3 ), _::round( ml7::Quaternion( 0.0f, 0.0f, 0.7071f, -0.7071f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ).conjugated(), ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 1.0f, 2.0f, 3.0f, 0.7071f ).conjugated(), ml7::Quaternion( -1.0f, -2.0f, -3.0f, 0.7071f ) );

    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ).inverted(), 3 ), _::round( ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.7071f, 0.0f, 0.0f, 0.7071f ).inverted(), 3 ), _::round( ml7::Quaternion( -0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.7071f, 0.0f, 0.7071f ).inverted(), 3 ), _::round( ml7::Quaternion( 0.0f, -0.7071f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.0f, 0.7071f, 0.7071f ).inverted(), 3 ), _::round( ml7::Quaternion( 0.0f, 0.0f, -0.7071f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.1889793f, 0.3779585f, 0.5669378f, 0.7071179f ).inverted(), 3 ), _::round( ml7::Quaternion( -0.1889793f, -0.3779585f, -0.5669378f, 0.7071179f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( -0.1889793f, -0.3779585f, -0.5669378f, 0.7071179f ).inverted(), 3 ), _::round( ml7::Quaternion( 0.1889793f, 0.3779585f, 0.5669378f, 0.7071179f ), 3 ) );

    // to_matrix3x3, to_axes, and to_axis_angle see below (with rotation etc.)

    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.2260934f, -0.7913268f, 0.5652334f, -0.0565233f ).transform( ml7::Vector3( 1.0f, 2.0f, 3.0f ) ), 3 ), _::round( ml7::Vector3( -0.4440895f, -2.5111822f, -2.7380192f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.2260934f, -0.7913268f, 0.5652334f, -0.0565233f ).transform_inverted( ml7::Vector3( -0.4440895f, -2.5111822f, -2.7380192f ) ), 3 ), _::round( ml7::Vector3( 1.0f, 2.0f, 3.0f ), 3 ) );

    // (further) transform etc. see below

    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ).normalize(), 3 ), _::round( ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 1.0f, 0.0f, 0.0f, 1.0f ).normalize(), 3 ), _::round( ml7::Quaternion( 0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, -1.0f, 0.0f, 1.0f ).normalize(), 3 ), _::round( ml7::Quaternion( 0.0f, -0.7071f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.0f, 1.0f, -1.0f ).normalize(), 3 ), _::round( ml7::Quaternion( 0.0f, 0.0f, 0.7071f, -0.7071f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ).conjugate(), ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 1.0f, 2.0f, 3.0f, 0.7071f ).conjugate(), ml7::Quaternion( -1.0f, -2.0f, -3.0f, 0.7071f ) );

    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ).invert(), 3 ), _::round( ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.7071f, 0.0f, 0.0f, 0.7071f ).invert(), 3 ), _::round( ml7::Quaternion( -0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.7071f, 0.0f, 0.7071f ).invert(), 3 ), _::round( ml7::Quaternion( 0.0f, -0.7071f, 0.0f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.0f, 0.7071f, 0.7071f ).invert(), 3 ), _::round( ml7::Quaternion( 0.0f, 0.0f, -0.7071f, 0.7071f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.1889793f, 0.3779585f, 0.5669378f, 0.7071179f ).invert(), 3 ), _::round( ml7::Quaternion( -0.1889793f, -0.3779585f, -0.5669378f, 0.7071179f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( -0.1889793f, -0.3779585f, -0.5669378f, 0.7071179f ).invert(), 3 ), _::round( ml7::Quaternion( 0.1889793f, 0.3779585f, 0.5669378f, 0.7071179f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::Quaternion( 1.0f, -2.0f, 3.0f, -4.0f ).dot( ml7::Quaternion( -1.0f, 2.0f, -3.0f, 4.0f ) ), -30.0f );

    TESTLABS_CHECK_EQ( ml7::Quaternion( 1.0f, 2.0f, 3.0f, 4.0f )[ 0 ], 1.0f );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 1.0f, 2.0f, 3.0f, 4.0f )[ 1 ], 2.0f );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 1.0f, 2.0f, 3.0f, 4.0f )[ 2 ], 3.0f );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 1.0f, 2.0f, 3.0f, 4.0f )[ 3 ], 4.0f );

    TESTLABS_CHECK_EQ( ml7::Quaternion( 1.0f, 2.0f, 3.0f, 4.0f )[ 0 ] = 5.0f, 5.0f );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 1.0f, 2.0f, 3.0f, 4.0f )[ 1 ] = 5.0f, 5.0f );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 1.0f, 2.0f, 3.0f, 4.0f )[ 2 ] = 5.0f, 5.0f );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 1.0f, 2.0f, 3.0f, 4.0f )[ 3 ] = 5.0f, 5.0f );

    TESTLABS_CHECK_EQ( +ml7::Quaternion( 1.0f, -2.0f, 3.0f, -4.0f ), ml7::Quaternion( 1.0f, -2.0f, 3.0f, -4.0f ) );
    TESTLABS_CHECK_EQ( -ml7::Quaternion( 1.0f, -2.0f, 3.0f, -4.0f ), ml7::Quaternion( -1.0f, 2.0f, -3.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 5.0f, -8.0f, 12.0f, 7.0f ) + ml7::Quaternion( -6.0f, 9.0f, -14.0f, 3.0f ), ml7::Quaternion( -1.0f, 1.0f, -2.0f, 10.0f ) );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 5.0f, -8.0f, 12.0f, 7.0f ) - ml7::Quaternion( -6.0f, 9.0f, -14.0f, 3.0f ), ml7::Quaternion( 11.0f, -17.0f, 26.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 1.0f, -2.0f, 3.0f, -4.0f ) * 5.0f, ml7::Quaternion( 5.0f, -10.0f, 15.0f, -20.0f ) );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 5.0f, -10.0f, 15.0f, -20.0f ) / 5.0f, ml7::Quaternion( 1.0f, -2.0f, 3.0f, -4.0f ) );

    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ) * ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ), 3 ), _::round( ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.7071f, 0.0f, 0.0f, 0.7071f ) * ml7::Quaternion( 0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ), _::round( ml7::Quaternion( 1.0f, 0.0f, 0.0f, 0.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.7071f, 0.0f, 0.7071f ) * ml7::Quaternion( 0.0f, 0.7071f, 0.0f, 0.7071f ), 3 ), _::round( ml7::Quaternion( 0.0f, 1.0f, 0.0f, 0.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.0f, 0.7071f, 0.7071f ) * ml7::Quaternion( 0.0f, 0.0f, 0.7071f, 0.7071f ), 3 ), _::round( ml7::Quaternion( 0.0f, 0.0f, 1.0f, 0.0f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.7071f, 0.0f, 0.0f, 0.7071f ) * ml7::Quaternion( 0.0f, 0.7071f, 0.0f, 0.7071f ), 3 ), _::round( ml7::Quaternion( 0.5f, 0.5f, 0.5f, 0.5f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.7071f, 0.0f, 0.0f, 0.7071f ) * ml7::Quaternion( 0.0f, 0.0f, 0.7071f, 0.7071f ), 3 ), _::round( ml7::Quaternion( 0.5f, -0.5f, 0.5f, 0.5f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.7071f, 0.0f, 0.7071f ) * ml7::Quaternion( 0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ), _::round( ml7::Quaternion( 0.5f, 0.5f, -0.5f, 0.5f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.7071f, 0.0f, 0.7071f ) * ml7::Quaternion( 0.0f, 0.0f, 0.7071f, 0.7071f ), 3 ), _::round( ml7::Quaternion( 0.5f, 0.5f, 0.5f, 0.5f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.0f, 0.7071f, 0.7071f ) * ml7::Quaternion( 0.7071f, 0.0f, 0.0f, 0.7071f ), 3 ), _::round( ml7::Quaternion( 0.5f, 0.5f, 0.5f, 0.5f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.0f, 0.0f, 0.7071f, 0.7071f ) * ml7::Quaternion( 0.0f, 0.7071f, 0.0f, 0.7071f ), 3 ), _::round( ml7::Quaternion( -0.5f, 0.5f, 0.5f, 0.5f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 1.0f, -2.0f, 3.0f, -4.0f ) * ml7::Quaternion( -5.0f, 6.0f, -7.0f, 8.0f ), 3 ), _::round( ml7::Quaternion( 24.0f, -48.0f, 48.0f, 6.0f ), 3 ) );

    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 0.2260934f, -0.7913268f, 0.5652334f, -0.0565233f ) * ml7::Vector3( 1.0f, 2.0f, 3.0f ), 3 ), _::round( ml7::Vector3( -0.4440895f, -2.5111822f, -2.7380192f ), 3 ) );

    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 1.0f, -2.0f, 3.0f, -4.0f ).normalized() * ml7::Quaternion( -5.0f, 6.0f, -7.0f, 8.0f ).normalized() * ml7::Vector3( 1.0f, 2.0f, 3.0f ), 3 ), _::round( ml7::Vector3( -0.8758619f, -3.3517244f, -1.4137932f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 1.0f, -2.0f, 3.0f, -4.0f ).normalized() * (ml7::Quaternion( -5.0f, 6.0f, -7.0f, 8.0f ).normalized() * ml7::Vector3( 1.0f, 2.0f, 3.0f )), 3 ), _::round( ml7::Vector3( -0.8758619f, -3.3517244f, -1.4137932f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( (ml7::Quaternion( 1.0f, -2.0f, 3.0f, -4.0f ).normalized() * ml7::Quaternion( -5.0f, 6.0f, -7.0f, 8.0f ).normalized()) * ml7::Vector3( 1.0f, 2.0f, 3.0f ), 3 ), _::round( ml7::Vector3( -0.8758619f, -3.3517244f, -1.4137932f ), 3 ) );
    TESTLABS_CHECK_EQ( _::round( ml7::Quaternion( 24.0f, -48.0f, 48.0f, 6.0f ).normalized() * ml7::Vector3( 1.0f, 2.0f, 3.0f ), 3 ), _::round( ml7::Vector3( -0.8758619f, -3.3517244f, -1.4137932f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::Quaternion( 5.0f, -8.0f, 12.0f, 7.0f ) += ml7::Quaternion( -6.0f, 9.0f, -14.0f, 3.0f ), ml7::Quaternion( -1.0f, 1.0f, -2.0f, 10.0f ) );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 5.0f, -8.0f, 12.0f, 7.0f ) -= ml7::Quaternion( -6.0f, 9.0f, -14.0f, 3.0f ), ml7::Quaternion( 11.0f, -17.0f, 26.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 1.0f, -2.0f, 3.0f, -4.0f ) *= 5.0f, ml7::Quaternion( 5.0f, -10.0f, 15.0f, -20.0f ) );
    TESTLABS_CHECK_EQ( ml7::Quaternion( 5.0f, -10.0f, 15.0f, -20.0f ) /= 5.0f, ml7::Quaternion( 1.0f, -2.0f, 3.0f, -4.0f ) );

    TESTLABS_CHECK( ml7::Quaternion( 1.0f, -2.0f, 3.0f, -4.0f ) == ml7::Quaternion( 1.0f, -2.0f, 3.0f, -4.0f ) );
    TESTLABS_CHECK( !(ml7::Quaternion( 1.0f, -2.0f, 3.0f, -4.0f ) == ml7::Quaternion( -1.0f, 2.0f, -3.0f, 4.0f )) );

    TESTLABS_CHECK( ml7::Quaternion( 1.0f, -2.0f, 3.0f, -4.0f ) != ml7::Quaternion( -1.0f, 2.0f, -3.0f, 4.0f ) );
    TESTLABS_CHECK( !(ml7::Quaternion( 1.0f, -2.0f, 3.0f, -4.0f ) != ml7::Quaternion( 1.0f, -2.0f, 3.0f, -4.0f )) );

    TESTLABS_CHECK_EQ( 5.0f * ml7::Quaternion( 1.0f, -2.0f, 3.0f, -4.0f ), ml7::Quaternion( 5.0f, -10.0f, 15.0f, -20.0f ) );
}



TESTLABS_CASE( TEXT("CoreLabs:  Quaternion:  rotx") )
{
    struct Entry
    {
        float angle;
        ml7::Quaternion expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ) },
        { 1.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.7071f, 0.0f, 0.0f, 0.7071f ) },
        { 2.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 1.0f, 0.0f, 0.0f, 0.0f ) },
        { 3.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.7071f, 0.0f, 0.0f, -0.7071f ) },
        { 4.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.0f, -1.0f ) },
        { -1.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( -0.7071f, 0.0f, 0.0f, 0.7071f ) },
        { -2.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( -1.0f, 0.0f, 0.0f, 0.0f ) },
        { -3.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( -0.7071f, 0.0f, 0.0f, -0.7071f ) },
    };

    TESTLABS_SUBCASE_BATCH( TEXT("rotx"), container, entry )
    {
        TESTLABS_CHECK_EQ( _::round( ml7::Quaternion::rotx( entry.angle ), 3 ), _::round( entry.expected, 3 ) );
    }
}

TESTLABS_CASE( TEXT("CoreLabs:  Quaternion:  roty") )
{
    struct Entry
    {
        float angle;
        ml7::Quaternion expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ) },
        { 1.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.7071f, 0.0f, 0.7071f ) },
        { 2.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 1.0f, 0.0f, 0.0f ) },
        { 3.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.7071f, 0.0f, -0.7071f ) },
        { 4.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.0f, -1.0f ) },
        { -1.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, -0.7071f, 0.0f, 0.7071f ) },
        { -2.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, -1.0f, 0.0f, 0.0f ) },
        { -3.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, -0.7071f, 0.0f, -0.7071f ) },
    };

    TESTLABS_SUBCASE_BATCH( TEXT("roty"), container, entry )
    {
        TESTLABS_CHECK_EQ( _::round( ml7::Quaternion::roty( entry.angle ), 3 ), _::round( entry.expected, 3 ) );
    }
}

TESTLABS_CASE( TEXT("CoreLabs:  Quaternion:  rotz") )
{
    struct Entry
    {
        float angle;
        ml7::Quaternion expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ) },
        { 1.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.7071f, 0.7071f ) },
        { 2.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 1.0f, 0.0f ) },
        { 3.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.7071f, -0.7071f ) },
        { 4.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.0f, -1.0f ) },
        { -1.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, -0.7071f, 0.7071f ) },
        { -2.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, -1.0f, 0.0f ) },
        { -3.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, -0.7071f, -0.7071f ) },
    };

    TESTLABS_SUBCASE_BATCH( TEXT("rotz"), container, entry )
    {
        TESTLABS_CHECK_EQ( _::round( ml7::Quaternion::rotz( entry.angle ), 3 ), _::round( entry.expected, 3 ) );
    }
}

TESTLABS_CASE( TEXT("CoreLabs:  Quaternion:  rotation etc.") )
{
    struct Entry
    {
        ml7::Vector3 axis;
        float angle;
        ml7::Quaternion expected;
    } entry;

    const std::vector<Entry> container {
        { ml7::Vector3::X, 0.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ) },
        { ml7::Vector3::X, 1.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.7071f, 0.0f, 0.0f, 0.7071f ) },
        { ml7::Vector3::X, 2.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 1.0f, 0.0f, 0.0f, 0.0f ) },
        { ml7::Vector3::X, 3.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.7071f, 0.0f, 0.0f, -0.7071f ) },
        { ml7::Vector3::X, 4.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.0f, -1.0f ) },
        { ml7::Vector3::X, -1.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( -0.7071f, 0.0f, 0.0f, 0.7071f ) },
        { ml7::Vector3::X, -2.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( -1.0f, 0.0f, 0.0f, 0.0f ) },
        { ml7::Vector3::X, -3.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( -0.7071f, 0.0f, 0.0f, -0.7071f ) },

        { ml7::Vector3::Y, 0.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ) },
        { ml7::Vector3::Y, 1.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.7071f, 0.0f, 0.7071f ) },
        { ml7::Vector3::Y, 2.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 1.0f, 0.0f, 0.0f ) },
        { ml7::Vector3::Y, 3.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.7071f, 0.0f, -0.7071f ) },
        { ml7::Vector3::Y, 4.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.0f, -1.0f ) },
        { ml7::Vector3::Y, -1.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, -0.7071f, 0.0f, 0.7071f ) },
        { ml7::Vector3::Y, -2.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, -1.0f, 0.0f, 0.0f ) },
        { ml7::Vector3::Y, -3.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, -0.7071f, 0.0f, -0.7071f ) },

        { ml7::Vector3::Z, 0.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ) },
        { ml7::Vector3::Z, 1.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.7071f, 0.7071f ) },
        { ml7::Vector3::Z, 2.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 1.0f, 0.0f ) },
        { ml7::Vector3::Z, 3.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.7071f, -0.7071f ) },
        { ml7::Vector3::Z, 4.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.0f, -1.0f ) },
        { ml7::Vector3::Z, -1.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, -0.7071f, 0.7071f ) },
        { ml7::Vector3::Z, -2.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, -1.0f, 0.0f ) },
        { ml7::Vector3::Z, -3.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, -0.7071f, -0.7071f ) },

        { -ml7::Vector3::X, -0.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ) },
        { -ml7::Vector3::X, -1.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.7071f, 0.0f, 0.0f, 0.7071f ) },
        { -ml7::Vector3::X, -2.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 1.0f, 0.0f, 0.0f, 0.0f ) },
        { -ml7::Vector3::X, -3.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.7071f, 0.0f, 0.0f, -0.7071f ) },
        { -ml7::Vector3::X, -4.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.0f, -1.0f ) },
        { -ml7::Vector3::X, 1.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( -0.7071f, 0.0f, 0.0f, 0.7071f ) },
        { -ml7::Vector3::X, 2.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( -1.0f, 0.0f, 0.0f, 0.0f ) },
        { -ml7::Vector3::X, 3.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( -0.7071f, 0.0f, 0.0f, -0.7071f ) },

        { -ml7::Vector3::Y, -0.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ) },
        { -ml7::Vector3::Y, -1.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.7071f, 0.0f, 0.7071f ) },
        { -ml7::Vector3::Y, -2.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 1.0f, 0.0f, 0.0f ) },
        { -ml7::Vector3::Y, -3.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.7071f, 0.0f, -0.7071f ) },
        { -ml7::Vector3::Y, -4.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.0f, -1.0f ) },
        { -ml7::Vector3::Y, 1.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, -0.7071f, 0.0f, 0.7071f ) },
        { -ml7::Vector3::Y, 2.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, -1.0f, 0.0f, 0.0f ) },
        { -ml7::Vector3::Y, 3.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, -0.7071f, 0.0f, -0.7071f ) },

        { -ml7::Vector3::Z, -0.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ) },
        { -ml7::Vector3::Z, -1.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.7071f, 0.7071f ) },
        { -ml7::Vector3::Z, -2.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 1.0f, 0.0f ) },
        { -ml7::Vector3::Z, -3.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.7071f, -0.7071f ) },
        { -ml7::Vector3::Z, -4.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, 0.0f, -1.0f ) },
        { -ml7::Vector3::Z, 1.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, -0.7071f, 0.7071f ) },
        { -ml7::Vector3::Z, 2.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, -1.0f, 0.0f ) },
        { -ml7::Vector3::Z, 3.0f/4.0f * ml7::constants::pi2, ml7::Quaternion( 0.0f, 0.0f, -0.7071f, -0.7071f ) },

        { ml7::Vector3( 1.0f, 1.0f, 1.0f ) * 1.0f/::sqrtf(3.0f), ml7::constants::pi * 1.0f/6.0f, ml7::Quaternion( 0.1494293f, 0.1494293f, 0.1494293f, 0.9659258f ) },
        { ml7::Vector3( 1.0f, 0.0f, 1.0f ) * 1.0f/::sqrtf(2.0f), ml7::constants::pi * 1.0f/4.0f, ml7::Quaternion( 0.270598f, 0.0f, 0.270598f, 0.9238796f ) },
        { ml7::Vector3( 1.0f, 2.0f, 0.0f ) * 1.0f/::sqrtf(5.0f), ml7::constants::pi * 1.0f/3.0f, ml7::Quaternion( 0.2236075f, 0.4472144f, 0.0f, 0.8660248f ) },
        { ml7::Vector3( 3.0f, 1.0f, 2.0f ) * 1.0f/::sqrtf(10.0f), ml7::constants::pi * 1.0f/6.0f, ml7::Quaternion( 0.2075169f, 0.0691724f, 0.1383448f, 0.9659258f ) },
        { ml7::Vector3( 2.0f, 1.0f, -1.0f ) * 1.0f/::sqrtf(6.0f), ml7::constants::pi * 2.0f/3.0f, ml7::Quaternion( 0.7071081f, 0.3535536f, -0.3535536f, 0.4999979f ) },
        { ml7::Vector3( 3.0f, 2.0f, 1.0f ) * 1.0f/::sqrtf(14.0f), ml7::constants::pi * 1.0f/2.0f, ml7::Quaternion( 0.566948f, 0.3779649f, 0.1889824f, 0.7071055f ) },
        { ml7::Vector3( 2.0f, 2.0f, 3.0f ) * 1.0f/::sqrtf(11.0f), ml7::constants::pi * 3.0f/4.0f, ml7::Quaternion( 0.4481471f, 0.4481471f, 0.6722203f, 0.3826855f ) },
        { ml7::Vector3( 2.0f, -3.0f, 4.0f ) * 1.0f/::sqrtf(29.0f), ml7::constants::pi * 1.0f/8.0f, ml7::Quaternion( 0.0724548f, -0.1086821f, 0.1449094f, 0.9807853f ) },
        { ml7::Vector3( 1.0f, 2.0f, 4.0f ) * 1.0f/::sqrtf(19.0f), ml7::constants::pi * 1.0f/5.0f, ml7::Quaternion( 0.0674332f, 0.134866f, 0.2697324f, 0.9510564f ) },
        { ml7::Vector3( 1.0f, 2.0f, -3.0f ) * 1.0f/::sqrtf(15.0f), ml7::constants::pi * 2.0f/5.0f, ml7::Quaternion( 0.1570925f, 0.3141851f, -0.4712776f, 0.8090161f ) },
        { ml7::Vector3( 4.0f, 1.0f, -2.0f ) * 1.0f/::sqrtf(21.0f), ml7::constants::pi * 7.0f/8.0f, ml7::Quaternion( 0.8560993f, 0.2140248f, -0.4280496f, 0.1950921f ) },
        { ml7::Vector3( 2.0f, 2.0f, -3.0f ) * 1.0f/::sqrtf(13.0f), ml7::constants::pi * 1.0f/7.0f, ml7::Quaternion( 0.1079385f, 0.1079385f, -0.1619078f, 0.9749279f ) },
    };

    TESTLABS_SUBCASE_BATCH( TEXT("rotation (axis with angle magnitude)"), container, entry )
    {
        ml7::Vector3 axis = entry.axis.normalized() * entry.angle;
        TESTLABS_CHECK_EQ( _::round( ml7::Quaternion::rotation( axis ), 3 ), _::round( entry.expected, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( TEXT("rotation (axis-angle)"), container, entry )
    {
        TESTLABS_CHECK_EQ( _::round( ml7::Quaternion::rotation( entry.axis, entry.angle ), 3 ), _::round( entry.expected, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( TEXT("rotation_normalized"), container, entry )
    {
        TESTLABS_CHECK_EQ( _::round( ml7::Quaternion::rotation_normalized( entry.axis.normalized(), entry.angle ), 3 ), _::round( entry.expected, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( TEXT("to_matrix3x3, to_axes, and to_axis_angle"), container, entry )
    {
        ml7::Matrix3x3 m = ml7::Matrix3x3::rotation( entry.axis, entry.angle );
        ml7::Quaternion q = ml7::Quaternion::rotation( entry.axis, entry.angle );

        TESTLABS_CHECK_EQ( _::round( q.to_matrix3x3(), 3 ), _::round( m, 3 ) );

        ml7::Vector3 x, y, z;
        q.to_axes( x, y, z );
        TESTLABS_CHECK_EQ( _::round( ml7::Matrix3x3::from_axes( x, y, z ), 2 ), _::round( m, 2 ) ); // Allow a larger rounding error than before!

        ml7::Vector3 axis;
        float angle;
        TESTLABS_CHECK_EQ( q.to_axis_angle( axis, angle ), entry.angle != 0.0f );
        TESTLABS_CHECK_EQ( _::round( ml7::Matrix3x3::rotation( axis, angle ), 2 ), _::round( m, 2 ) ); // Allow a larger rounding error than before!
    }
}



TESTLABS_CASE( TEXT("CoreLabs:  Quaternion:  transform etc.") )
{
    struct Entry
    {
        ml7::Vector3 axis;
        float angle;
        ml7::Vector3 untransformed;
        ml7::Vector3 transformed;
    } entry;

    const std::vector<Entry> container {
        { ml7::Vector3( 1.0f, 0.0f, 0.0f ), 0.0f/4.0f * ml7::constants::pi2, ml7::Vector3( 1.0f, 2.0f, 3.0f ), ml7::Vector3( 1.0f, 2.0f, 3.0f ) },
        { ml7::Vector3( 1.0f, 1.0f, 1.0f ), 1.0f/4.0f * ml7::constants::pi2, ml7::Vector3( 1.0f, 2.0f, 3.0f ), ml7::Vector3( 2.5774f, 0.8453f, 2.5774f ) },
        // ...
    };

    TESTLABS_SUBCASE_BATCH( TEXT("transform"), container, entry )
    {
        TESTLABS_CHECK_EQ( _::round( ml7::Quaternion::rotation( entry.axis, entry.angle ).transform( entry.untransformed ), 3 ), _::round( entry.transformed, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( TEXT("transform_inverted"), container, entry )
    {
        TESTLABS_CHECK_EQ( _::round( ml7::Quaternion::rotation( entry.axis, entry.angle ).transform_inverted( entry.transformed ), 3 ), _::round( entry.untransformed, 3 ) );
    }
}



#endif // F77_TESTS_ML7_QUATERNION_H

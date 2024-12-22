#pragma once
#ifndef F77_TESTS_ML7_MATRIX3X4_H
#define F77_TESTS_ML7_MATRIX3X4_H

#include <MathLabs/Matrix3x4.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



namespace tl7::internals {
    inline
    cl7::string to_string(const ml7::Matrix3x4& m) { return TEXT("{ ") + cl7::to_string(m._11) + TEXT(", ") + cl7::to_string(m._12) + TEXT(", ") + cl7::to_string(m._13) + TEXT(", ") + cl7::to_string(m._14) + TEXT("; ") + cl7::to_string(m._21) + TEXT(", ") + cl7::to_string(m._22) + TEXT(", ") + cl7::to_string(m._23) + TEXT(", ") + cl7::to_string(m._24) + TEXT("; ") + cl7::to_string(m._31) + TEXT(", ") + cl7::to_string(m._32) + TEXT(", ") + cl7::to_string(m._33) + TEXT(", ") + cl7::to_string(m._34) + TEXT(" }"); }
}



namespace ml7 {
    inline
    ml7::Matrix3x4 round(ml7::Matrix3x4 m, unsigned num_decimals) { for ( unsigned k = 0; k < 12; ++k ) m.data[k] = ml7::round(m.data[k], num_decimals); return m; }
}



TESTLABS_CASE( TEXT("CoreLabs:  Matrix3x4:  ") )
{
    TESTLABS_CHECK_EQ( ml7::Matrix3x4(), ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix3x4( ml7::Matrix3x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f ) ), ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 0.0f, 4.0f, 5.0f, 6.0f, 0.0f, 7.0f, 8.0f, 9.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( ml7::Matrix3x3( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f ), ml7::Vector3( 10.0f, 11.0f, 12.0f ) ), ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 10.0f, 4.0f, 5.0f, 6.0f, 11.0f, 7.0f, 8.0f, 9.0f, 12.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix3x4::from_axes( ml7::Vector3( 1.0f, 0.0f, 0.0f ), ml7::Vector3( 0.0f, 1.0f, 0.0f ), ml7::Vector3( 0.0f, 0.0f, 1.0f ) ), ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4::from_axes( ml7::Vector3( 1.0f, 0.0f, 0.0f ), ml7::Vector3( 0.0f, 1.0f, 0.0f ), ml7::Vector3( 0.0f, 0.0f, 1.0f ) ).transform( ml7::Vector3( 1.0f, 2.0f, 3.0f ) ), ml7::Vector3( 1.0f, 2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4::from_axes( ml7::Vector3( 0.0f, 1.0f, 0.0f ), ml7::Vector3( -1.0f, 0.0f, 0.0f ), ml7::Vector3( 0.0f, 0.0f, -1.0f ) ), ml7::Matrix3x4( 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4::from_axes( ml7::Vector3( 0.0f, 1.0f, 0.0f ), ml7::Vector3( -1.0f, 0.0f, 0.0f ), ml7::Vector3( 0.0f, 0.0f, -1.0f ) ).transform( ml7::Vector3( 1.0f, 2.0f, 3.0f ) ), ml7::Vector3( -2.0f, 1.0f, -3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4::from_axes( ml7::Vector3( 0.0f, 1.0f, 0.0f ), ml7::Vector3( -1.0f, 0.0f, 0.0f ), ml7::Vector3( 0.0f, 0.0f, -1.0f ), ml7::Vector3( 2.0f, 3.0f, 4.0f ) ), ml7::Matrix3x4( 0.0f, -1.0f, 0.0f, 2.0f, 1.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, -1.0f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4::from_axes( ml7::Vector3( 0.0f, 1.0f, 0.0f ), ml7::Vector3( -1.0f, 0.0f, 0.0f ), ml7::Vector3( 0.0f, 0.0f, -1.0f ), ml7::Vector3( 2.0f, 3.0f, 4.0f ) ).transform( ml7::Vector3( 1.0f, 2.0f, 3.0f ) ), ml7::Vector3( 0.0f, 4.0f, 1.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix3x4::scaling( -1.0f ), ml7::Matrix3x4( -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4::scaling( ml7::Vector3( 0.5f, 2.0f, -1.0f ) ), ml7::Matrix3x4( 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) );

    // rotation see below

    TESTLABS_CHECK_EQ( ml7::Matrix3x4::translation( ml7::Vector3( 10.0f, 11.0f, 12.0f ) ), ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 10.0f, 0.0f, 1.0f, 0.0f, 11.0f, 0.0f, 0.0f, 1.0f, 12.0f ) );

    // compose (and decompose) see below

    // look (and is_look) etc. see below

    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 10.0f, 4.0f, 5.0f, 6.0f, 11.0f, 7.0f, 8.0f, 9.0f, 12.0f ).is_invertible(), false );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 2.0f, 3.0f, 4.0f, 10.0f, 5.0f, 6.0f, 7.0f, 11.0f, 8.0f, 9.0f, 1.0f, 12.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 3.0f, 4.0f, 5.0f, 10.0f, 6.0f, 7.0f, 8.0f, 11.0f, 9.0f, 1.0f, 2.0f, 12.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 4.0f, 5.0f, 6.0f, 10.0f, 7.0f, 8.0f, 9.0f, 11.0f, 1.0f, 2.0f, 3.0f, 12.0f ).is_invertible(), false );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 5.0f, 6.0f, 7.0f, 10.0f, 8.0f, 9.0f, 1.0f, 11.0f, 2.0f, 3.0f, 4.0f, 12.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 6.0f, 7.0f, 8.0f, 10.0f, 9.0f, 1.0f, 2.0f, 11.0f, 3.0f, 4.0f, 5.0f, 12.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 7.0f, 8.0f, 9.0f, 10.0f, 1.0f, 2.0f, 3.0f, 11.0f, 4.0f, 5.0f, 6.0f, 12.0f ).is_invertible(), false );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 8.0f, 9.0f, 1.0f, 10.0f, 2.0f, 3.0f, 4.0f, 11.0f, 5.0f, 6.0f, 7.0f, 12.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 9.0f, 1.0f, 2.0f, 10.0f, 3.0f, 4.0f, 5.0f, 11.0f, 6.0f, 7.0f, 8.0f, 12.0f ).is_invertible(), true );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 2.0f, 1.0f, 2.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 4.0f, 1.0f, 4.0f, 0.0f ).is_invertible(), false );

    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 10.0f, 4.0f, 5.0f, 6.0f, 11.0f, 7.0f, 8.0f, 9.0f, 12.0f ).determinant(), 0.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 2.0f, 3.0f, 4.0f, 10.0f, 5.0f, 6.0f, 7.0f, 11.0f, 8.0f, 9.0f, 1.0f, 12.0f ).determinant(), 27.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 3.0f, 4.0f, 5.0f, 10.0f, 6.0f, 7.0f, 8.0f, 11.0f, 9.0f, 1.0f, 2.0f, 12.0f ).determinant(), -27.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 4.0f, 5.0f, 6.0f, 10.0f, 7.0f, 8.0f, 9.0f, 11.0f, 1.0f, 2.0f, 3.0f, 12.0f ).determinant(), 0.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 5.0f, 6.0f, 7.0f, 10.0f, 8.0f, 9.0f, 1.0f, 11.0f, 2.0f, 3.0f, 4.0f, 12.0f ).determinant(), 27.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 6.0f, 7.0f, 8.0f, 10.0f, 9.0f, 1.0f, 2.0f, 11.0f, 3.0f, 4.0f, 5.0f, 12.0f ).determinant(), -27.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 7.0f, 8.0f, 9.0f, 10.0f, 1.0f, 2.0f, 3.0f, 11.0f, 4.0f, 5.0f, 6.0f, 12.0f ).determinant(), 0.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 8.0f, 9.0f, 1.0f, 10.0f, 2.0f, 3.0f, 4.0f, 11.0f, 5.0f, 6.0f, 7.0f, 12.0f ).determinant(), 27.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 9.0f, 1.0f, 2.0f, 10.0f, 3.0f, 4.0f, 5.0f, 11.0f, 6.0f, 7.0f, 8.0f, 12.0f ).determinant(), -27.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 2.0f, 1.0f, 2.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 4.0f, 1.0f, 4.0f, 0.0f ).determinant(), 0.0f );

    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 2.0f, 1.0f, 3.0f, 10.0f, 0.0f, 2.0f, 4.0f, 11.0f, 1.0f, 1.0f, 2.0f, 12.0f ).inverted(), ml7::Matrix3x4( 0.0f, -1.0f/2.0f, 1.0f, -13.0f/2.0f, -2.0f, -1.0f/2.0f, 4.0f, -45.0f/2.0f, 1.0f, 1.0f/2.0f, -2.0f, 17.0f/2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 0.0f, -1.0f/2.0f, 1.0f, -13.0f/2.0f, -2.0f, -1.0f/2.0f, 4.0f, -45.0f/2.0f, 1.0f, 1.0f/2.0f, -2.0f, 17.0f/2.0f ).inverted(), ml7::Matrix3x4( 2.0f, 1.0f, 3.0f, 10.0f, 0.0f, 2.0f, 4.0f, 11.0f, 1.0f, 1.0f, 2.0f, 12.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f ).get_column( 0 ), ml7::Vector3( 1.0f, 5.0f, 9.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f ).get_column( 1 ), ml7::Vector3( 2.0f, 6.0f, 10.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f ).get_column( 2 ), ml7::Vector3( 3.0f, 7.0f, 11.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f ).get_column( 3 ), ml7::Vector3( 4.0f, 8.0f, 12.0f ) );

    ml7::Vector3 x, y, z, origin;
    ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f ).to_axes( x, y, z );
    TESTLABS_CHECK_EQ( x, ml7::Vector3( 1.0f, 5.0f, 9.0f ) );
    TESTLABS_CHECK_EQ( y, ml7::Vector3( 2.0f, 6.0f, 10.0f ) );
    TESTLABS_CHECK_EQ( z, ml7::Vector3( 3.0f, 7.0f, 11.0f ) );
    ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f ).to_axes( x, y, z, origin );
    TESTLABS_CHECK_EQ( x, ml7::Vector3( 1.0f, 5.0f, 9.0f ) );
    TESTLABS_CHECK_EQ( y, ml7::Vector3( 2.0f, 6.0f, 10.0f ) );
    TESTLABS_CHECK_EQ( z, ml7::Vector3( 3.0f, 7.0f, 11.0f ) );
    TESTLABS_CHECK_EQ( origin, ml7::Vector3( 4.0f, 8.0f, 12.0f ) );

    // to_axis_angle see below (with decompose)

    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f ).to_matrix3x3(), ml7::Matrix3x3( 1.0f, 2.0f, 3.0f, 5.0f, 6.0f, 7.0f, 9.0f, 10.0f, 11.0f ) );

    ml7::Matrix3x3 m3x3;
    ml7::Vector3 translation;
    ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f ).decompose( m3x3, translation );
    TESTLABS_CHECK_EQ( m3x3, ml7::Matrix3x3( 1.0f, 2.0f, 3.0f, 5.0f, 6.0f, 7.0f, 9.0f, 10.0f, 11.0f ) );
    TESTLABS_CHECK_EQ( translation, ml7::Vector3( 4.0f, 8.0f, 12.0f ) );

    // (further) decompose see below

    // is_look etc. see below

    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 2.0f, 1.0f, 3.0f, 10.0f, 0.0f, 2.0f, 4.0f, 11.0f, 1.0f, 1.0f, 2.0f, 12.0f ).transform( ml7::Vector3( 1.0f, 2.0f, 3.0f ) ), ml7::Vector3( 23.0f, 27.0f, 21.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 2.0f, 1.0f, 3.0f, 10.0f, 0.0f, 2.0f, 4.0f, 11.0f, 1.0f, 1.0f, 2.0f, 12.0f ).transform3x3( ml7::Vector3( 1.0f, 2.0f, 3.0f ) ), ml7::Vector3( 13.0f, 16.0f, 9.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 2.0f, 1.0f, 3.0f, 10.0f, 0.0f, 2.0f, 4.0f, 11.0f, 1.0f, 1.0f, 2.0f, 12.0f ).transform_inverted( ml7::Vector3( 23.0f, 27.0f, 21.0f ) ), ml7::Vector3( 1.0f, 2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 2.0f, 1.0f, 3.0f, 10.0f, 0.0f, 2.0f, 4.0f, 11.0f, 1.0f, 1.0f, 2.0f, 12.0f ).transform3x3_inverted( ml7::Vector3( 13.0f, 16.0f, 9.0f ) ), ml7::Vector3( 1.0f, 2.0f, 3.0f ) );

    // (further) transform etc. see below

    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 2.0f, 1.0f, 3.0f, 10.0f, 0.0f, 2.0f, 4.0f, 11.0f, 1.0f, 1.0f, 2.0f, 12.0f ).invert(), ml7::Matrix3x4( 0.0f, -1.0f/2.0f, 1.0f, -13.0f/2.0f, -2.0f, -1.0f/2.0f, 4.0f, -45.0f/2.0f, 1.0f, 1.0f/2.0f, -2.0f, 17.0f/2.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 0.0f, -1.0f/2.0f, 1.0f, -13.0f/2.0f, -2.0f, -1.0f/2.0f, 4.0f, -45.0f/2.0f, 1.0f, 1.0f/2.0f, -2.0f, 17.0f/2.0f ).invert(), ml7::Matrix3x4( 2.0f, 1.0f, 3.0f, 10.0f, 0.0f, 2.0f, 4.0f, 11.0f, 1.0f, 1.0f, 2.0f, 12.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f ).set_column( 0, ml7::Vector3( 13.0f, 14.0f, 15.0f ) ), ml7::Matrix3x4( 13.0f, 2.0f, 3.0f, 4.0f, 14.0f, 6.0f, 7.0f, 8.0f, 15.0f, 10.0f, 11.0f, 12.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f ).set_column( 1, ml7::Vector3( 13.0f, 14.0f, 15.0f ) ), ml7::Matrix3x4( 1.0f, 13.0f, 3.0f, 4.0f, 5.0f, 14.0f, 7.0f, 8.0f, 9.0f, 15.0f, 11.0f, 12.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f ).set_column( 2, ml7::Vector3( 13.0f, 14.0f, 15.0f ) ), ml7::Matrix3x4( 1.0f, 2.0f, 13.0f, 4.0f, 5.0f, 6.0f, 14.0f, 8.0f, 9.0f, 10.0f, 15.0f, 12.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f ).set_column( 3, ml7::Vector3( 13.0f, 14.0f, 15.0f ) ), ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 13.0f, 5.0f, 6.0f, 7.0f, 14.0f, 9.0f, 10.0f, 11.0f, 15.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 0 ][ 0 ], 1.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 0 ][ 1 ], 2.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 0 ][ 2 ], 3.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 0 ][ 3 ], 4.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 1 ][ 0 ], 5.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 1 ][ 1 ], 6.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 1 ][ 2 ], 7.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 1 ][ 3 ], 8.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 2 ][ 0 ], 9.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 2 ][ 1 ], 10.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 2 ][ 2 ], 11.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 2 ][ 3 ], 12.0f );

    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 0 ][ 0 ] = 13.0f, 13.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 0 ][ 1 ] = 13.0f, 13.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 0 ][ 2 ] = 13.0f, 13.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 0 ][ 3 ] = 13.0f, 13.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 1 ][ 0 ] = 13.0f, 13.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 1 ][ 1 ] = 13.0f, 13.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 1 ][ 2 ] = 13.0f, 13.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 1 ][ 3 ] = 13.0f, 13.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 2 ][ 0 ] = 13.0f, 13.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 2 ][ 1 ] = 13.0f, 13.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 2 ][ 2 ] = 13.0f, 13.0f );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f )[ 2 ][ 3 ] = 13.0f, 13.0f );

    TESTLABS_CHECK_EQ( +ml7::Matrix3x4( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f, 9.0f, -10.0f, 11.0f, -12.0f ), ml7::Matrix3x4( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f, 9.0f, -10.0f, 11.0f, -12.0f ) );
    TESTLABS_CHECK_EQ( -ml7::Matrix3x4( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f, 9.0f, -10.0f, 11.0f, -12.0f ), ml7::Matrix3x4( -1.0f, 2.0f, -3.0f, 4.0f, -5.0f, 6.0f, -7.0f, 8.0f, -9.0f, 10.0f, -11.0f, 12.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 5.0f, -8.0f, 12.0f, 7.0f, -9.0f, 15.0f, -1.0f, 8.0f, 4.0f, -10.0f, 3.0f, -11.0f ) + ml7::Matrix3x4( -6.0f, 9.0f, -14.0f, 3.0f, 11.0f, -7.0f, 2.0f, -4.0f, 13.0f, 16.0f, -12.0f, 5.0f ), ml7::Matrix3x4( -1.0f, 1.0f, -2.0f, 10.0f, 2.0f, 8.0f, 1.0f, 4.0f, 17.0f, 6.0f, -9.0f, -6.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 5.0f, -8.0f, 12.0f, 7.0f, -9.0f, 15.0f, -1.0f, 8.0f, 4.0f, -10.0f, 3.0f, -11.0f ) - ml7::Matrix3x4( -6.0f, 9.0f, -14.0f, 3.0f, 11.0f, -7.0f, 2.0f, -4.0f, 13.0f, 16.0f, -12.0f, 5.0f ), ml7::Matrix3x4( 11.0f, -17.0f, 26.0f, 4.0f, -20.0f, 22.0f, -3.0f, 12.0f, -9.0f, -26.0f, 15.0f, -16.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f, 9.0f, -10.0f, 11.0f, -12.0f ) * 13.0f, ml7::Matrix3x4( 13.0f, -26.0f, 39.0f, -52.0f, 65.0f, -78.0f, 91.0f, -104.0f, 117.0f, -130.0f, 143.0f, -156.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 13.0f, -26.0f, 39.0f, -52.0f, 65.0f, -78.0f, 91.0f, -104.0f, 117.0f, -130.0f, 143.0f, -156.0f ) / 13.0f, ml7::Matrix3x4( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f, 9.0f, -10.0f, 11.0f, -12.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f, 9.0f, -10.0f, 11.0f, -12.0f ) * ml7::Matrix3x4( -13.0f, 14.0f, -15.0f, 16.0f, -17.0f, 18.0f, -19.0f, 20.0f, -21.0f, 22.0f, -23.0f, 24.0f ), ml7::Matrix3x4( -42.0f, 44.0f, -46.0f, 44.0f, -110.0f, 116.0f, -122.0f, 120.0f, -178.0f, 188.0f, -198.0f, 196.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( -13.0f, 14.0f, -15.0f, 16.0f, -17.0f, 18.0f, -19.0f, 20.0f, -21.0f, 22.0f, -23.0f, 24.0f ) * ml7::Matrix3x4( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f, 9.0f, -10.0f, 11.0f, -12.0f ), ml7::Matrix3x4( -78.0f, 92.0f, -106.0f, 136.0f, -98.0f, 116.0f, -134.0f, 172.0f, -118.0f, 140.0f, -162.0f, 208.0f ) );

    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 2.0f, 1.0f, 3.0f, 10.0f, 0.0f, 2.0f, 4.0f, 11.0f, 1.0f, 1.0f, 2.0f, 12.0f ) * ml7::Vector3( 1.0f, 2.0f, 3.0f ), ml7::Vector3( 23.0f, 27.0f, 21.0f ) );

    TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f, 9.0f, -10.0f, 11.0f, -12.0f ) * ml7::Matrix3x4( -13.0f, 14.0f, -15.0f, 16.0f, -17.0f, 18.0f, -19.0f, 20.0f, -21.0f, 22.0f, -23.0f, 24.0f ) * ml7::Vector3( 1.0f, 2.0f, 3.0f ), 3 ), ml7::round( ml7::Vector3( -48.0f, -124.0f, -200.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f, 9.0f, -10.0f, 11.0f, -12.0f ) * (ml7::Matrix3x4( -13.0f, 14.0f, -15.0f, 16.0f, -17.0f, 18.0f, -19.0f, 20.0f, -21.0f, 22.0f, -23.0f, 24.0f ) * ml7::Vector3( 1.0f, 2.0f, 3.0f )), 3 ), ml7::round( ml7::Vector3( -48.0f, -124.0f, -200.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( (ml7::Matrix3x4( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f, 9.0f, -10.0f, 11.0f, -12.0f ) * ml7::Matrix3x4( -13.0f, 14.0f, -15.0f, 16.0f, -17.0f, 18.0f, -19.0f, 20.0f, -21.0f, 22.0f, -23.0f, 24.0f )) * ml7::Vector3( 1.0f, 2.0f, 3.0f ), 3 ), ml7::round( ml7::Vector3( -48.0f, -124.0f, -200.0f ), 3 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4( -42.0f, 44.0f, -46.0f, 44.0f, -110.0f, 116.0f, -122.0f, 120.0f, -178.0f, 188.0f, -198.0f, 196.0f ) * ml7::Vector3( 1.0f, 2.0f, 3.0f ), 3 ), ml7::round( ml7::Vector3( -48.0f, -124.0f, -200.0f ), 3 ) );

    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 5.0f, -8.0f, 12.0f, 7.0f, -9.0f, 15.0f, -1.0f, 8.0f, 4.0f, -10.0f, 3.0f, -11.0f ) += ml7::Matrix3x4( -6.0f, 9.0f, -14.0f, 3.0f, 11.0f, -7.0f, 2.0f, -4.0f, 13.0f, 16.0f, -12.0f, 5.0f ), ml7::Matrix3x4( -1.0f, 1.0f, -2.0f, 10.0f, 2.0f, 8.0f, 1.0f, 4.0f, 17.0f, 6.0f, -9.0f, -6.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 5.0f, -8.0f, 12.0f, 7.0f, -9.0f, 15.0f, -1.0f, 8.0f, 4.0f, -10.0f, 3.0f, -11.0f ) -= ml7::Matrix3x4( -6.0f, 9.0f, -14.0f, 3.0f, 11.0f, -7.0f, 2.0f, -4.0f, 13.0f, 16.0f, -12.0f, 5.0f ), ml7::Matrix3x4( 11.0f, -17.0f, 26.0f, 4.0f, -20.0f, 22.0f, -3.0f, 12.0f, -9.0f, -26.0f, 15.0f, -16.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f, 9.0f, -10.0f, 11.0f, -12.0f ) *= 13.0f, ml7::Matrix3x4( 13.0f, -26.0f, 39.0f, -52.0f, 65.0f, -78.0f, 91.0f, -104.0f, 117.0f, -130.0f, 143.0f, -156.0f ) );
    TESTLABS_CHECK_EQ( ml7::Matrix3x4( 13.0f, -26.0f, 39.0f, -52.0f, 65.0f, -78.0f, 91.0f, -104.0f, 117.0f, -130.0f, 143.0f, -156.0f ) /= 13.0f, ml7::Matrix3x4( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f, 9.0f, -10.0f, 11.0f, -12.0f ) );

    TESTLABS_CHECK( ml7::Matrix3x4( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f, 9.0f, -10.0f, 11.0f, -12.0f ) == ml7::Matrix3x4( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f, 9.0f, -10.0f, 11.0f, -12.0f ) );
    TESTLABS_CHECK( !(ml7::Matrix3x4( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f, 9.0f, -10.0f, 11.0f, -12.0f ) == ml7::Matrix3x4( -1.0f, 2.0f, -3.0f, 4.0f, -5.0f, 6.0f, -7.0f, 8.0f, -9.0f, 10.0f, -11.0f, 12.0f )) );

    TESTLABS_CHECK( ml7::Matrix3x4( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f, 9.0f, -10.0f, 11.0f, -12.0f ) != ml7::Matrix3x4( -1.0f, 2.0f, -3.0f, 4.0f, -5.0f, 6.0f, -7.0f, 8.0f, -9.0f, 10.0f, -11.0f, 12.0f ) );
    TESTLABS_CHECK( !(ml7::Matrix3x4( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f, 9.0f, -10.0f, 11.0f, -12.0f ) != ml7::Matrix3x4( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f, 9.0f, -10.0f, 11.0f, -12.0f )) );

    TESTLABS_CHECK_EQ( 13.0f * ml7::Matrix3x4( 1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f, 9.0f, -10.0f, 11.0f, -12.0f ) , ml7::Matrix3x4( 13.0f, -26.0f, 39.0f, -52.0f, 65.0f, -78.0f, 91.0f, -104.0f, 117.0f, -130.0f, 143.0f, -156.0f ) );
}



TESTLABS_CASE( TEXT("CoreLabs:  Matrix3x4:  rotx") )
{
    struct Entry
    {
        float angle;
        ml7::Matrix3x4 expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { 1.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f ) },
        { 2.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) },
        { 3.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f ) },
        { 4.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { -1.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f ) },
        { -2.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) },
        { -3.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f ) },
    };

    TESTLABS_SUBCASE_BATCH( TEXT("rotx"), container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4::rotx( entry.angle ), 3 ), ml7::round( entry.expected, 3 ) );
    }
}

TESTLABS_CASE( TEXT("CoreLabs:  Matrix3x4:  roty") )
{
    struct Entry
    {
        float angle;
        ml7::Matrix3x4 expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { 1.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f ) },
        { 2.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) },
        { 3.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f ) },
        { 4.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { -1.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f ) },
        { -2.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) },
        { -3.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f ) },
    };

    TESTLABS_SUBCASE_BATCH( TEXT("roty"), container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4::roty( entry.angle ), 3 ), ml7::round( entry.expected, 3 ) );
    }
}

TESTLABS_CASE( TEXT("CoreLabs:  Matrix3x4:  rotz") )
{
    struct Entry
    {
        float angle;
        ml7::Matrix3x4 expected;
    } entry;

    const std::vector<Entry> container {
        { 0.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { 1.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { 2.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { 3.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { 4.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { -1.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { -2.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { -3.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
    };

    TESTLABS_SUBCASE_BATCH( TEXT("rotz"), container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4::rotz( entry.angle ), 3 ), ml7::round( entry.expected, 3 ) );
    }
}

TESTLABS_CASE( TEXT("CoreLabs:  Matrix3x4:  rotation etc.") )
{
    struct Entry
    {
        ml7::Vector3 axis;
        float angle;
        ml7::Matrix3x4 expected;
    } entry;

    const std::vector<Entry> container {
        { ml7::Vector3::X, 0.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { ml7::Vector3::X, 1.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f ) },
        { ml7::Vector3::X, 2.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) },
        { ml7::Vector3::X, 3.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f ) },
        { ml7::Vector3::X, 4.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { ml7::Vector3::X, -1.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f ) },
        { ml7::Vector3::X, -2.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) },
        { ml7::Vector3::X, -3.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f ) },

        { ml7::Vector3::Y, 0.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { ml7::Vector3::Y, 1.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f ) },
        { ml7::Vector3::Y, 2.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) },
        { ml7::Vector3::Y, 3.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f ) },
        { ml7::Vector3::Y, 4.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { ml7::Vector3::Y, -1.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f ) },
        { ml7::Vector3::Y, -2.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) },
        { ml7::Vector3::Y, -3.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f ) },

        { ml7::Vector3::Z, 0.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { ml7::Vector3::Z, 1.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { ml7::Vector3::Z, 2.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { ml7::Vector3::Z, 3.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { ml7::Vector3::Z, 4.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { ml7::Vector3::Z, -1.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { ml7::Vector3::Z, -2.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { ml7::Vector3::Z, -3.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },

        { -ml7::Vector3::X, -0.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { -ml7::Vector3::X, -1.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f ) },
        { -ml7::Vector3::X, -2.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) },
        { -ml7::Vector3::X, -3.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f ) },
        { -ml7::Vector3::X, -4.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { -ml7::Vector3::X, 1.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f ) },
        { -ml7::Vector3::X, 2.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) },
        { -ml7::Vector3::X, 3.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f ) },

        { -ml7::Vector3::Y, -0.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { -ml7::Vector3::Y, -1.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f ) },
        { -ml7::Vector3::Y, -2.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) },
        { -ml7::Vector3::Y, -3.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f ) },
        { -ml7::Vector3::Y, -4.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { -ml7::Vector3::Y, 1.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f ) },
        { -ml7::Vector3::Y, 2.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) },
        { -ml7::Vector3::Y, 3.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f ) },

        { -ml7::Vector3::Z, -0.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { -ml7::Vector3::Z, -1.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { -ml7::Vector3::Z, -2.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { -ml7::Vector3::Z, -3.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { -ml7::Vector3::Z, -4.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { -ml7::Vector3::Z, 1.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { -ml7::Vector3::Z, 2.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { -ml7::Vector3::Z, 3.0f/4.0f * ml7::constants::pi2, ml7::Matrix3x4( 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },

        { ml7::Vector3( 1.0f, 1.0f, 1.0f ) * 1.0f/::sqrtf(3.0f), ml7::constants::pi * 1.0f/6.0f, ml7::Matrix3x4( 0.9106835f, -0.2440170f, 0.3333335f, 0.0f, 0.3333335f, 0.9106835f, -0.2440170f, 0.0f, -0.2440170f, 0.3333335f, 0.9106835f, 0.0f ) },
        { ml7::Vector3( 1.0f, 0.0f, 1.0f ) * 1.0f/::sqrtf(2.0f), ml7::constants::pi * 1.0f/4.0f, ml7::Matrix3x4( 0.8535535f, -0.4999999f, 0.1464466f, 0.0f, 0.4999999f, 0.7071069f, -0.4999999f, 0.0f, 0.1464466f, 0.4999999f, 0.8535535f, 0.0f ) },
        { ml7::Vector3( 1.0f, 2.0f, 0.0f ) * 1.0f/::sqrtf(5.0f), ml7::constants::pi * 1.0f/3.0f, ml7::Matrix3x4( 0.5999998f, 0.2000003f, 0.7745967f, 0.0f, 0.2000003f, 0.8999997f, -0.3872988f, 0.0f, -0.7745967f, 0.3872988f, 0.4999996f, 0.0f ) },
        { ml7::Vector3( 3.0f, 1.0f, 2.0f ) * 1.0f/::sqrtf(10.0f), ml7::constants::pi * 1.0f/6.0f, ml7::Matrix3x4( 0.9521518f, -0.2385526f, 0.1910485f, 0.0f, 0.2959704f, 0.8755949f, -0.3817526f, 0.0f, -0.0762130f, 0.4200311f, 0.9043038f, 0.0f ) },
        { ml7::Vector3( 2.0f, 1.0f, -1.0f ) * 1.0f/::sqrtf(6.0f), ml7::constants::pi * 2.0f/3.0f, ml7::Matrix3x4( 0.5000008f, 0.8535529f, -0.1464466f, 0.0f, 0.1464466f, -0.2500003f, -0.9571067f, 0.0f, -0.8535529f, 0.4571075f, -0.2500003f, 0.0f ) },
        { ml7::Vector3( 3.0f, 2.0f, 1.0f ) * 1.0f/::sqrtf(14.0f), ml7::constants::pi * 1.0f/2.0f, ml7::Matrix3x4( 0.6428578f, 0.1613101f, 0.7488076f, 0.0f, 0.6958322f, 0.2857141f, -0.6589272f, 0.0f, -0.3202365f, 0.9446409f, 0.0714288f, 0.0f ) },
        { ml7::Vector3( 2.0f, 2.0f, 3.0f ) * 1.0f/::sqrtf(11.0f), ml7::constants::pi * 3.0f/4.0f, ml7::Matrix3x4( -0.3054341f, -0.1128235f, 0.9455056f, 0.0f, 0.9161682f, -0.3054341f, 0.2595107f, 0.0f, 0.2595107f, 0.9455056f, 0.1966553f, 0.0f ) },
        { ml7::Vector3( 2.0f, -3.0f, 4.0f ) * 1.0f/::sqrtf(29.0f), ml7::constants::pi * 1.0f/8.0f, ml7::Matrix3x4( 0.9343789f, -0.2999991f, -0.1921889f, 0.0f, 0.2685010f, 0.9475031f, -0.1736230f, 0.0f, 0.2341864f, 0.1106267f, 0.9658771f, 0.0f ) },
        { ml7::Vector3( 1.0f, 2.0f, 4.0f ) * 1.0f/::sqrtf(19.0f), ml7::constants::pi * 1.0f/5.0f, ml7::Matrix3x4( 0.8181112f, -0.4948725f, 0.2929083f, 0.0f, 0.5312504f, 0.8453944f, -0.0555100f, 0.0f, -0.2201526f, 0.2010210f, 0.9545279f, 0.0f ) },
        { ml7::Vector3( 1.0f, 2.0f, -3.0f ) * 1.0f/::sqrtf(15.0f), ml7::constants::pi * 2.0f/5.0f, ml7::Matrix3x4( 0.3583730f, 0.8612535f, 0.3602933f, 0.0f, -0.6638297f, 0.5064408f, -0.5503161f, 0.0f, -0.6564289f, -0.0419550f, 0.7532204f, 0.0f ) },
        { ml7::Vector3( 4.0f, 1.0f, -2.0f ) * 1.0f/::sqrtf(21.0f), ml7::constants::pi * 7.0f/8.0f, ml7::Matrix3x4( 0.5419334f, 0.5334699f, -0.6493983f, 0.0f, 0.1994367f, -0.8322664f, -0.5172598f, 0.0f, -0.8164148f, 0.1508065f, -0.5574264f, 0.0f ) },
        { ml7::Vector3( 2.0f, 2.0f, -3.0f ) * 1.0f/::sqrtf(13.0f), ml7::constants::pi * 1.0f/7.0f, ml7::Matrix3x4( 0.9242703f, 0.3389983f, 0.1755124f, 0.0f, -0.2923954f, 0.9242703f, -0.2454167f, 0.0f, -0.2454167f, 0.1755124f, 0.9533971f, 0.0f ) },
    };

    TESTLABS_SUBCASE_BATCH( TEXT("rotation (axis with angle magnitude)"), container, entry )
    {
        ml7::Vector3 axis = entry.axis.normalized() * entry.angle;
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4::rotation( axis ), 3 ), ml7::round( entry.expected, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( TEXT("rotation (axis-angle)"), container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4::rotation( entry.axis, entry.angle ), 3 ), ml7::round( entry.expected, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( TEXT("rotation_normalized"), container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4::rotation_normalized( entry.axis.normalized(), entry.angle ), 3 ), ml7::round( entry.expected, 3 ) );
    }
}



TESTLABS_CASE( TEXT("CoreLabs:  Matrix3x4:  compose(float, ...) / decompose") )
{
    struct Entry
    {
        float scaling;
        ml7::Vector3 axis;
        float angle;
        ml7::Vector3 translation;
    } entry;

    std::vector<Entry> container {
    };

    //::srand( static_cast<unsigned>( ::time( nullptr ) ) );
    for ( size_t k = 0; k < 100; ++k )
    {
        float scaling = float(::rand() % 11) * 0.2f + 0.5f;
        float ax = float(::rand() % 11) * 0.4f - 2.0f;
        float ay = float(::rand() % 11) * 0.4f - 2.0f;
        float az = float(::rand() % 11) * 0.4f - 2.0f;
        ml7::Vector3 axis{ ax, ay, az };
        float angle = float(::rand() % 11) * 0.2f * ml7::constants::pi2 - ml7::constants::pi2;
        float tx = float(::rand() % 31) - 15.0f;
        float ty = float(::rand() % 31) - 15.0f;
        float tz = float(::rand() % 31) - 15.0f;
        ml7::Vector3 translation{ tx, ty, tz };
        assert( scaling > 0.0f );
        if ( !axis.lensqr() )
            continue;
        container.push_back( { scaling, axis, angle, translation } );
    }

    TESTLABS_SUBCASE_BATCH( TEXT("compose(float, ...) / decompose"), container, entry )
    {
        ml7::Matrix3x4 entry_expected = ml7::Matrix3x4::translation( entry.translation ) * ml7::Matrix3x4::rotation( entry.axis, entry.angle ) * ml7::Matrix3x4::scaling( entry.scaling );

        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4::compose( entry.scaling, entry.axis, entry.angle, entry.translation ), 3 ), ml7::round( entry_expected, 3 ) );

        ml7::Vector3 scaling;
        ml7::Vector3 axis;
        float angle;
        ml7::Vector3 translation;
        TESTLABS_CHECK( entry_expected.decompose( scaling, axis, angle, translation ) );

        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4::compose( scaling, axis, angle, translation ), 2 ), ml7::round( entry_expected, 2 ) ); // Allow a larger rounding error than before!
    }
}

TESTLABS_CASE( TEXT("CoreLabs:  Matrix3x4:  compose(Vector3, ...) / decompose (and to_axis_angle)") )
{
    struct Entry
    {
        ml7::Vector3 scaling;
        ml7::Vector3 axis;
        float angle;
        ml7::Vector3 translation;
    } entry;

    std::vector<Entry> container {
    };

    //::srand( static_cast<unsigned>( ::time( nullptr ) ) );
    for ( size_t k = 0; k < 100; ++k )
    {
        float sx = float(::rand() % 11) * 0.2f + 0.5f;
        float sy = float(::rand() % 11) * 0.2f + 0.5f;
        float sz = float(::rand() % 11) * 0.2f + 0.5f;
        ml7::Vector3 scaling{ sx, sy, sz };
        float ax = float(::rand() % 21) * 0.2f - 2.0f;
        float ay = float(::rand() % 21) * 0.2f - 2.0f;
        float az = float(::rand() % 21) * 0.2f - 2.0f;
        ml7::Vector3 axis{ ax, ay, az };
        float angle = float(::rand() % 11) * 0.2f * ml7::constants::pi2 - ml7::constants::pi2;
        float tx = float(::rand() % 31) - 15.0f;
        float ty = float(::rand() % 31) - 15.0f;
        float tz = float(::rand() % 31) - 15.0f;
        ml7::Vector3 translation{ tx, ty, tz };
        assert( scaling.x > 0.0f && scaling.y > 0.0f && scaling.z > 0.0f );
        if ( !axis.lensqr() )
            continue;
        container.push_back( { scaling, axis, angle, translation } );
    }

    TESTLABS_SUBCASE_BATCH( TEXT("compose(Vector3, ...) / decompose (and to_axis_angle)"), container, entry )
    {
        const ml7::Matrix3x4 entry_rotation = ml7::Matrix3x4::rotation( entry.axis, entry.angle );
        const ml7::Matrix3x4 entry_expected = ml7::Matrix3x4::translation( entry.translation ) * entry_rotation * ml7::Matrix3x4::scaling( entry.scaling );

        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4::compose( entry.scaling, entry.axis, entry.angle, entry.translation ), 3 ), ml7::round( entry_expected, 3 ) );

        ml7::Vector3 scaling;
        ml7::Vector3 axis;
        float angle;
        ml7::Vector3 translation;

        TESTLABS_CHECK( entry_expected.decompose( scaling, axis, angle, translation ) );
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4::compose( scaling, axis, angle, translation ), 2 ), ml7::round( entry_expected, 2 ) ); // Allow a larger rounding error than before!

        TESTLABS_CHECK( entry_rotation.to_axis_angle( axis, angle ) );
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4::rotation( axis, angle ), 2 ), ml7::round( entry_rotation, 2 ) ); // Allow a larger rounding error than before!
    }
}



TESTLABS_CASE( TEXT("CoreLabs:  Matrix3x4:  look_at / look_to / is_look etc.") )
{
    struct Entry
    {
        ml7::Vector3 position;
        ml7::Vector3 look;
        ml7::Vector3 up;
        ml7::Matrix3x4 lh;
        ml7::Matrix3x4 rh;
    } entry;

    const std::vector<Entry> container {
        { ml7::Vector3( 0.0f, 0.0f, 0.0f ), ml7::Vector3( 0.0f, 0.0f, 1.0f ), ml7::Vector3( 0.0f, 1.0f, 0.0f ), ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ), ml7::Matrix3x4( -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) },
        { ml7::Vector3( 0.0f, 0.0f, 0.0f ), ml7::Vector3( 0.0f, 0.0f, -1.0f ), ml7::Vector3( 0.0f, 1.0f, 0.0f ), ml7::Matrix3x4( -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ), ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { ml7::Vector3( 0.0f, 0.0f, 0.0f ), ml7::Vector3( 0.0f, 0.0f, 1.0f ), ml7::Vector3( 0.0f, 2.0f, 2.0f ), ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ), ml7::Matrix3x4( -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ) },
        { ml7::Vector3( 0.0f, 0.0f, 0.0f ), ml7::Vector3( 0.0f, 0.0f, -1.0f ), ml7::Vector3( 0.0f, 2.0f, 2.0f ), ml7::Matrix3x4( -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f ), ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ) },
        { ml7::Vector3( 1.0f, 2.0f, 4.0f ), ml7::Vector3( 0.0f, 0.0f, 1.0f ), ml7::Vector3( 0.0f, 2.0f, -0.5f ), ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, -2.0f, 0.0f, 0.0f, 1.0f, -4.0f ), ml7::Matrix3x4( -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -2.0f, 0.0f, 0.0f, -1.0f, 4.0f ) },
        { ml7::Vector3( 1.0f, 2.0f, 4.0f ), ml7::Vector3( 0.0f, 0.0f, -1.0f ), ml7::Vector3( 0.0f, 2.0f, -0.5f ), ml7::Matrix3x4( -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -2.0f, 0.0f, 0.0f, -1.0f, 4.0f ), ml7::Matrix3x4( 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, -2.0f, 0.0f, 0.0f, 1.0f, -4.0f ) },
        // ...
    };

    TESTLABS_SUBCASE_BATCH( TEXT("look_at_lh / look_to_lh / is_look_lh"), container, entry )
    {
        const ml7::Vector3 entry_look_at = entry.position + entry.look;
        const ml7::Vector3 entry_look_to = entry.look;

        const ml7::Vector3 expected_position = entry.position;
        const ml7::Vector3 expected_look = entry.look.normalized();
        const ml7::Vector3 expected_up = (entry.up - expected_look.dot( entry.up ) * expected_look).normalized();

        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4::look_at_lh( entry.position, entry_look_at, entry.up ), 3 ), ml7::round( entry.lh, 3 ) );
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4::look_to_lh( entry.position, entry_look_to, entry.up ), 3 ), ml7::round( entry.lh, 3 ) );

        ml7::Vector3 position;
        ml7::Vector3 look;
        ml7::Vector3 up;
        TESTLABS_CHECK( entry.lh.is_look_lh( position, look, up ) );
        TESTLABS_CHECK_EQ( ml7::round( position, 3 ), ml7::round( expected_position, 3 ) );
        TESTLABS_CHECK_EQ( ml7::round( look, 3 ), ml7::round( expected_look, 3 ) );
        TESTLABS_CHECK_EQ( ml7::round( up, 3 ), ml7::round( expected_up, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( TEXT("look_at_rh / look_to_rh / is_look_rh"), container, entry )
    {
        const ml7::Vector3 entry_look_at = entry.position + entry.look;
        const ml7::Vector3 entry_look_to = entry.look;

        const ml7::Vector3 expected_position = entry.position;
        const ml7::Vector3 expected_look = entry.look.normalized();
        const ml7::Vector3 expected_up = (entry.up - expected_look.dot( entry.up ) * expected_look).normalized();

        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4::look_at_rh( entry.position, entry_look_at, entry.up ), 3 ), ml7::round( entry.rh, 3 ) );
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4::look_to_rh( entry.position, entry_look_to, entry.up ), 3 ), ml7::round( entry.rh, 3 ) );

        ml7::Vector3 position;
        ml7::Vector3 look;
        ml7::Vector3 up;
        TESTLABS_CHECK( entry.rh.is_look_rh( position, look, up ) );
        TESTLABS_CHECK_EQ( ml7::round( position, 3 ), ml7::round( expected_position, 3 ) );
        TESTLABS_CHECK_EQ( ml7::round( look, 3 ), ml7::round( expected_look, 3 ) );
        TESTLABS_CHECK_EQ( ml7::round( up, 3 ), ml7::round( expected_up, 3 ) );
    }
}



TESTLABS_CASE( TEXT("CoreLabs:  Matrix3x4:  transform etc.") )
{
    struct Entry
    {
        ml7::Vector3 scaling;
        ml7::Vector3 axis;
        float angle;
        ml7::Vector3 translation;
        ml7::Vector3 untransformed;
        ml7::Vector3 transformed;
        ml7::Vector3 transformed_3x3;
    } entry;

    const std::vector<Entry> container {
        { ml7::Vector3( 1.0f, 1.0f, 1.0f ), ml7::Vector3( 1.0f, 0.0f, 0.0f ), 0.0f/4.0f * ml7::constants::pi2, ml7::Vector3( 10.0f, 11.0f, 12.0f ), ml7::Vector3( 1.0f, 2.0f, 3.0f ), ml7::Vector3( 11.0f, 13.0f, 15.0f ), ml7::Vector3( 1.0f, 2.0f, 3.0f ) },
        { ml7::Vector3( 1.0f, 1.0f, 1.0f ), ml7::Vector3( 1.0f, 1.0f, 1.0f ), 1.0f/4.0f * ml7::constants::pi2, ml7::Vector3( 10.0f, 11.0f, 12.0f ), ml7::Vector3( 1.0f, 2.0f, 3.0f ), ml7::Vector3( 12.5774f, 11.8453f, 14.5774f ), ml7::Vector3( 2.5774f, 0.8453f, 2.5774f ) },
        { ml7::Vector3( 1.0f, 2.0f, 0.5f ), ml7::Vector3( 1.0f, 1.0f, 1.0f ), 1.0f/4.0f * ml7::constants::pi2, ml7::Vector3( 10.0f, 11.0f, 12.0f ), ml7::Vector3( 1.0f, 2.0f, 3.0f ), ml7::Vector3( 10.7233f, 12.8780f, 15.8987f ), ml7::Vector3( 0.7233f, 1.8780f, 3.8987f ) },
        // ...
    };

    TESTLABS_SUBCASE_BATCH( TEXT("transform"), container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4::compose( entry.scaling, entry.axis, entry.angle, entry.translation ).transform( entry.untransformed ), 3 ), ml7::round( entry.transformed, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( TEXT("transform3x3"), container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4::compose( entry.scaling, entry.axis, entry.angle, entry.translation ).transform3x3( entry.untransformed ), 3 ), ml7::round( entry.transformed_3x3, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( TEXT("transform_inverted"), container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4::compose( entry.scaling, entry.axis, entry.angle, entry.translation ).transform_inverted( entry.transformed ), 3 ), ml7::round( entry.untransformed, 3 ) );
    }

    TESTLABS_SUBCASE_BATCH( TEXT("transform3x3_inverted"), container, entry )
    {
        TESTLABS_CHECK_EQ( ml7::round( ml7::Matrix3x4::compose( entry.scaling, entry.axis, entry.angle, entry.translation ).transform3x3_inverted( entry.transformed_3x3 ), 3 ), ml7::round( entry.untransformed, 3 ) );
    }
}



#endif // F77_TESTS_ML7_MATRIX3X4_H

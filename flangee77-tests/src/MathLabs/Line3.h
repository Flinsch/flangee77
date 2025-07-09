#ifndef F77_TESTS_ML7_LINE3_H
#define F77_TESTS_ML7_LINE3_H

#include <MathLabs/Line3.h>

#include <TestLabs/TestSuite.h>

#include "./shared.h"



namespace tl7::internals {
    inline
    cl7::u8string to_string(const ml7::Line3& line) { return u8"{ " + to_string(line.point) + u8", " + to_string(line.direction) + u8" }"; }
}



TESTLABS_CASE( u8"CoreLabs:  Line3:  " )
{
    TESTLABS_CHECK_EQ( ml7::Line3(), ml7::Line3( ml7::Vector3( 0.0f, 0.0f, 0.0f ), ml7::Vector3( 0.0f, 0.0f, 0.0f ) ) );

    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).point_at( -1.0f ), ml7::Vector3( 7.0f, -7.0f, 7.0f ) );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).point_at( 0.0f ), ml7::Vector3( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).point_at( 0.5f ), ml7::Vector3( -2.0f, 0.5f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).point_at( 1.0f ), ml7::Vector3( -5.0f, 3.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).point_at( 2.0f ), ml7::Vector3( -11.0f, 8.0f, -5.0f ) );

    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).parameter( ml7::Vector3( 7.0f, -7.0f, 7.0f ) ), -1.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).parameter( ml7::Vector3( 1.0f, -2.0f, 3.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).parameter( ml7::Vector3( -2.0f, 0.5f, 1.0f ) ), 0.5f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).parameter( ml7::Vector3( -5.0f, 3.0f, -1.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).parameter( ml7::Vector3( -11.0f, 8.0f, -5.0f ) ), 2.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).parameter( ml7::Vector3( 11.0f, -3.0f, 6.0f ) ), -1.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).parameter( ml7::Vector3( 5.0f, 2.0f, 2.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).parameter( ml7::Vector3( 2.0f, 4.5f, 0.0f ) ), 0.5f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).parameter( ml7::Vector3( -1.0f, 7.0f, -2.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).parameter( ml7::Vector3( -7.0f, 12.0f, -6.0f ) ), 2.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).parameter( ml7::Vector3( 3.0f, -11.0f, 8.0f ) ), -1.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).parameter( ml7::Vector3( -3.0f, -6.0f, 4.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).parameter( ml7::Vector3( -6.0f, -3.5f, 2.0f ) ), 0.5f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).parameter( ml7::Vector3( -9.0f, -1.0f, 0.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).parameter( ml7::Vector3( -15.0f, 4.0f, -4.0f ) ), 2.0f );

    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).closest_point( ml7::Vector3( 7.0f, -7.0f, 7.0f ) ), ml7::Vector3( 7.0f, -7.0f, 7.0f ) );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).closest_point( ml7::Vector3( 1.0f, -2.0f, 3.0f ) ), ml7::Vector3( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).closest_point( ml7::Vector3( -2.0f, 0.5f, 1.0f ) ), ml7::Vector3( -2.0f, 0.5f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).closest_point( ml7::Vector3( -5.0f, 3.0f, -1.0f ) ), ml7::Vector3( -5.0f, 3.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).closest_point( ml7::Vector3( -11.0f, 8.0f, -5.0f ) ), ml7::Vector3( -11.0f, 8.0f, -5.0f ) );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).closest_point( ml7::Vector3( 11.0f, -3.0f, 6.0f ) ), ml7::Vector3( 7.0f, -7.0f, 7.0f ) );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).closest_point( ml7::Vector3( 5.0f, 2.0f, 2.0f ) ), ml7::Vector3( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).closest_point( ml7::Vector3( 2.0f, 4.5f, 0.0f ) ), ml7::Vector3( -2.0f, 0.5f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).closest_point( ml7::Vector3( -1.0f, 7.0f, -2.0f ) ), ml7::Vector3( -5.0f, 3.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).closest_point( ml7::Vector3( -7.0f, 12.0f, -6.0f ) ), ml7::Vector3( -11.0f, 8.0f, -5.0f ) );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).closest_point( ml7::Vector3( 3.0f, -11.0f, 8.0f ) ), ml7::Vector3( 7.0f, -7.0f, 7.0f ) );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).closest_point( ml7::Vector3( -3.0f, -6.0f, 4.0f ) ), ml7::Vector3( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).closest_point( ml7::Vector3( -6.0f, -3.5f, 2.0f ) ), ml7::Vector3( -2.0f, 0.5f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).closest_point( ml7::Vector3( -9.0f, -1.0f, 0.0f ) ), ml7::Vector3( -5.0f, 3.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).closest_point( ml7::Vector3( -15.0f, 4.0f, -4.0f ) ), ml7::Vector3( -11.0f, 8.0f, -5.0f ) );

    TESTLABS_CHECK_EQ_FLT( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distance( ml7::Vector3( 7.0f, -7.0f, 7.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_FLT( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distance( ml7::Vector3( 1.0f, -2.0f, 3.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_FLT( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distance( ml7::Vector3( -2.0f, 0.5f, 1.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_FLT( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distance( ml7::Vector3( -5.0f, 3.0f, -1.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_FLT( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distance( ml7::Vector3( -11.0f, 8.0f, -5.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_FLT( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distance( ml7::Vector3( 11.0f, -3.0f, 6.0f ) ), 5.7445626f );
    TESTLABS_CHECK_EQ_FLT( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distance( ml7::Vector3( 5.0f, 2.0f, 2.0f ) ), 5.7445626f );
    TESTLABS_CHECK_EQ_FLT( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distance( ml7::Vector3( 2.0f, 4.5f, 0.0f ) ), 5.7445626f );
    TESTLABS_CHECK_EQ_FLT( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distance( ml7::Vector3( -1.0f, 7.0f, -2.0f ) ), 5.7445626f );
    TESTLABS_CHECK_EQ_FLT( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distance( ml7::Vector3( -7.0f, 12.0f, -6.0f ) ), 5.7445626f );
    TESTLABS_CHECK_EQ_FLT( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distance( ml7::Vector3( 3.0f, -11.0f, 8.0f ) ), 5.7445626f );
    TESTLABS_CHECK_EQ_FLT( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distance( ml7::Vector3( -3.0f, -6.0f, 4.0f ) ), 5.7445626f );
    TESTLABS_CHECK_EQ_FLT( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distance( ml7::Vector3( -6.0f, -3.5f, 2.0f ) ), 5.7445626f );
    TESTLABS_CHECK_EQ_FLT( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distance( ml7::Vector3( -9.0f, -1.0f, 0.0f ) ), 5.7445626f );
    TESTLABS_CHECK_EQ_FLT( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distance( ml7::Vector3( -15.0f, 4.0f, -4.0f ) ), 5.7445626f );

    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distsqr( ml7::Vector3( 7.0f, -7.0f, 7.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distsqr( ml7::Vector3( 1.0f, -2.0f, 3.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distsqr( ml7::Vector3( -2.0f, 0.5f, 1.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distsqr( ml7::Vector3( -5.0f, 3.0f, -1.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distsqr( ml7::Vector3( -11.0f, 8.0f, -5.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distsqr( ml7::Vector3( 11.0f, -3.0f, 6.0f ) ), 33.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distsqr( ml7::Vector3( 5.0f, 2.0f, 2.0f ) ), 33.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distsqr( ml7::Vector3( 2.0f, 4.5f, 0.0f ) ), 33.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distsqr( ml7::Vector3( -1.0f, 7.0f, -2.0f ) ), 33.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distsqr( ml7::Vector3( -7.0f, 12.0f, -6.0f ) ), 33.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distsqr( ml7::Vector3( 3.0f, -11.0f, 8.0f ) ), 33.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distsqr( ml7::Vector3( -3.0f, -6.0f, 4.0f ) ), 33.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distsqr( ml7::Vector3( -6.0f, -3.5f, 2.0f ) ), 33.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distsqr( ml7::Vector3( -9.0f, -1.0f, 0.0f ) ), 33.0f );
    TESTLABS_CHECK_EQ( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -6.0f, 5.0f, -4.0f ) ).distsqr( ml7::Vector3( -15.0f, 4.0f, -4.0f ) ), 33.0f );

    TESTLABS_CHECK( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -4.0f, 5.0f, -6.0f ) ) == ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -4.0f, 5.0f, -6.0f ) ) );
    TESTLABS_CHECK( !(ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -4.0f, 5.0f, -6.0f ) ) == ml7::Line3( ml7::Vector3( -1.0f, 2.0f, -3.0f ), ml7::Vector3( 4.0f, -5.0f, 6.0f ) )) );

    TESTLABS_CHECK( ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -4.0f, 5.0f, -6.0f ) ) != ml7::Line3( ml7::Vector3( -1.0f, 2.0f, -3.0f ), ml7::Vector3( 4.0f, -5.0f, 6.0f ) ) );
    TESTLABS_CHECK( !(ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -4.0f, 5.0f, -6.0f ) ) != ml7::Line3( ml7::Vector3( 1.0f, -2.0f, 3.0f ), ml7::Vector3( -4.0f, 5.0f, -6.0f ) )) );
}



#endif // F77_TESTS_ML7_LINE3_H

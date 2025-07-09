#ifndef F77_TESTS_ML7_LINESEGMENT2_H
#define F77_TESTS_ML7_LINESEGMENT2_H

#include <MathLabs/LineSegment2.h>

#include <TestLabs/TestSuite.h>

#include "./shared.h"



namespace tl7::internals {
    inline
    cl7::u8string to_string(const ml7::LineSegment2& line_segment) { return u8"{ " + to_string(line_segment.start) + u8", " + to_string(line_segment.end) + u8" }"; }
}



TESTLABS_CASE( u8"CoreLabs:  LineSegment2:  " )
{
    TESTLABS_CHECK_EQ( ml7::LineSegment2(), ml7::LineSegment2( ml7::Vector2( 0.0f, 0.0f ), ml7::Vector2( 0.0f, 0.0f ) ) );

    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).point_at( -1.0f ), ml7::Vector2( 4.0f, -2.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).point_at( 0.0f ), ml7::Vector2( 1.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).point_at( 0.5f ), ml7::Vector2( -0.5f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).point_at( 1.0f ), ml7::Vector2( -2.0f, 6.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).point_at( 2.0f ), ml7::Vector2( -5.0f, 10.0f ) );

    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).parameter( ml7::Vector2( 4.0f, -2.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).parameter( ml7::Vector2( 1.0f, 2.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).parameter( ml7::Vector2( -0.5f, 4.0f ) ), 0.5f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).parameter( ml7::Vector2( -2.0f, 6.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).parameter( ml7::Vector2( -5.0f, 10.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).parameter( ml7::Vector2( 0.0f, -5.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).parameter( ml7::Vector2( -3.0f, -1.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).parameter( ml7::Vector2( -4.5f, 1.0f ) ), 0.5f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).parameter( ml7::Vector2( -6.0f, 3.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).parameter( ml7::Vector2( -9.0f, 7.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).parameter( ml7::Vector2( 8.0f, 1.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).parameter( ml7::Vector2( 5.0f, 5.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).parameter( ml7::Vector2( 3.5f, 7.0f ) ), 0.5f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).parameter( ml7::Vector2( 2.0f, 9.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).parameter( ml7::Vector2( -1.0f, 13.0f ) ), 1.0f );

    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).closest_point( ml7::Vector2( 4.0f, -2.0f ) ), ml7::Vector2( 1.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).closest_point( ml7::Vector2( 1.0f, 2.0f ) ), ml7::Vector2( 1.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).closest_point( ml7::Vector2( -0.5f, 4.0f ) ), ml7::Vector2( -0.5f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).closest_point( ml7::Vector2( -2.0f, 6.0f ) ), ml7::Vector2( -2.0f, 6.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).closest_point( ml7::Vector2( -5.0f, 10.0f ) ), ml7::Vector2( -2.0f, 6.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).closest_point( ml7::Vector2( 0.0f, -5.0f ) ), ml7::Vector2( 1.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).closest_point( ml7::Vector2( -3.0f, -1.0f ) ), ml7::Vector2( 1.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).closest_point( ml7::Vector2( -4.5f, 1.0f ) ), ml7::Vector2( -0.5f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).closest_point( ml7::Vector2( -6.0f, 3.0f ) ), ml7::Vector2( -2.0f, 6.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).closest_point( ml7::Vector2( -9.0f, 7.0f ) ), ml7::Vector2( -2.0f, 6.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).closest_point( ml7::Vector2( 8.0f, 1.0f ) ), ml7::Vector2( 1.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).closest_point( ml7::Vector2( 5.0f, 5.0f ) ), ml7::Vector2( 1.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).closest_point( ml7::Vector2( 3.5f, 7.0f ) ), ml7::Vector2( -0.5f, 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).closest_point( ml7::Vector2( 2.0f, 9.0f ) ), ml7::Vector2( -2.0f, 6.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).closest_point( ml7::Vector2( -1.0f, 13.0f ) ), ml7::Vector2( -2.0f, 6.0f ) );

    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distance( ml7::Vector2( 4.0f, -2.0f ) ), 5.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distance( ml7::Vector2( 1.0f, 2.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distance( ml7::Vector2( -0.5f, 4.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distance( ml7::Vector2( -2.0f, 6.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distance( ml7::Vector2( -5.0f, 10.0f ) ), 5.0f );
    TESTLABS_CHECK_EQ_FLT( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distance( ml7::Vector2( 0.0f, -5.0f ) ), 7.071068f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distance( ml7::Vector2( -3.0f, -1.0f ) ), 5.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distance( ml7::Vector2( -4.5f, 1.0f ) ), 5.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distance( ml7::Vector2( -6.0f, 3.0f ) ), 5.0f );
    TESTLABS_CHECK_EQ_FLT( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distance( ml7::Vector2( -9.0f, 7.0f ) ), 7.071068f );
    TESTLABS_CHECK_EQ_FLT( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distance( ml7::Vector2( 8.0f, 1.0f ) ), 7.071068f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distance( ml7::Vector2( 5.0f, 5.0f ) ), 5.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distance( ml7::Vector2( 3.5f, 7.0f ) ), 5.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distance( ml7::Vector2( 2.0f, 9.0f ) ), 5.0f );
    TESTLABS_CHECK_EQ_FLT( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distance( ml7::Vector2( -1.0f, 13.0f ) ), 7.071068f );

    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distsqr( ml7::Vector2( 4.0f, -2.0f ) ), 25.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distsqr( ml7::Vector2( 1.0f, 2.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distsqr( ml7::Vector2( -0.5f, 4.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distsqr( ml7::Vector2( -2.0f, 6.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distsqr( ml7::Vector2( -5.0f, 10.0f ) ), 25.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distsqr( ml7::Vector2( 0.0f, -5.0f ) ), 50.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distsqr( ml7::Vector2( -3.0f, -1.0f ) ), 25.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distsqr( ml7::Vector2( -4.5f, 1.0f ) ), 25.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distsqr( ml7::Vector2( -6.0f, 3.0f ) ), 25.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distsqr( ml7::Vector2( -9.0f, 7.0f ) ), 50.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distsqr( ml7::Vector2( 8.0f, 1.0f ) ), 50.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distsqr( ml7::Vector2( 5.0f, 5.0f ) ), 25.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distsqr( ml7::Vector2( 3.5f, 7.0f ) ), 25.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distsqr( ml7::Vector2( 2.0f, 9.0f ) ), 25.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment2( ml7::Vector2( 1.0f, 2.0f ), ml7::Vector2( -2.0f, 6.0f ) ).distsqr( ml7::Vector2( -1.0f, 13.0f ) ), 50.0f );

    TESTLABS_CHECK( ml7::LineSegment2( ml7::Vector2( 1.0f, -2.0f ), ml7::Vector2( -3.0f, 4.0f ) ) == ml7::LineSegment2( ml7::Vector2( 1.0f, -2.0f ), ml7::Vector2( -3.0f, 4.0f ) ) );
    TESTLABS_CHECK( !(ml7::LineSegment2( ml7::Vector2( 1.0f, -2.0f ), ml7::Vector2( -3.0f, 4.0f ) ) == ml7::LineSegment2( ml7::Vector2( -1.0f, 2.0f ), ml7::Vector2( 3.0f, -4.0f ) )) );

    TESTLABS_CHECK( ml7::LineSegment2( ml7::Vector2( 1.0f, -2.0f ), ml7::Vector2( -3.0f, 4.0f ) ) != ml7::LineSegment2( ml7::Vector2( -1.0f, 2.0f ), ml7::Vector2( 3.0f, -4.0f ) ) );
    TESTLABS_CHECK( !(ml7::LineSegment2( ml7::Vector2( 1.0f, -2.0f ), ml7::Vector2( -3.0f, 4.0f ) ) != ml7::LineSegment2( ml7::Vector2( 1.0f, -2.0f ), ml7::Vector2( -3.0f, 4.0f ) )) );
}



#endif // F77_TESTS_ML7_LINESEGMENT2_H

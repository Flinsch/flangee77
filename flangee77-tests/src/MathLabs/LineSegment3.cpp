
#include <TestLabs/TestSuite.h>

#include <MathLabs/LineSegment3.h>

#include "./shared.h"



namespace tl7::internals {
    inline
    cl7::u8string to_string(const ml7::LineSegment3f& line_segment) { return u8"{ " + to_string(line_segment.start) + u8", " + to_string(line_segment.end) + u8" }"; }
}



TESTLABS_CASE( u8"CoreLabs:  LineSegment3:  " )
{
    TESTLABS_CHECK_EQ( ml7::LineSegment3f(), ml7::LineSegment3f( ml7::Vector3f( 0.0f, 0.0f, 0.0f ), ml7::Vector3f( 0.0f, 0.0f, 0.0f ) ) );

    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).point_at( -1.0f ), ml7::Vector3f( 7.0f, -7.0f, 7.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).point_at( 0.0f ), ml7::Vector3f( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).point_at( 0.5f ), ml7::Vector3f( -2.0f, 0.5f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).point_at( 1.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).point_at( 2.0f ), ml7::Vector3f( -11.0f, 8.0f, -5.0f ) );

    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).parameter( ml7::Vector3f( 7.0f, -7.0f, 7.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).parameter( ml7::Vector3f( 1.0f, -2.0f, 3.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).parameter( ml7::Vector3f( -2.0f, 0.5f, 1.0f ) ), 0.5f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).parameter( ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).parameter( ml7::Vector3f( -11.0f, 8.0f, -5.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).parameter( ml7::Vector3f( 11.0f, -3.0f, 6.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).parameter( ml7::Vector3f( 5.0f, 2.0f, 2.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).parameter( ml7::Vector3f( 2.0f, 4.5f, 0.0f ) ), 0.5f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).parameter( ml7::Vector3f( -1.0f, 7.0f, -2.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).parameter( ml7::Vector3f( -7.0f, 12.0f, -6.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).parameter( ml7::Vector3f( 3.0f, -11.0f, 8.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).parameter( ml7::Vector3f( -3.0f, -6.0f, 4.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).parameter( ml7::Vector3f( -6.0f, -3.5f, 2.0f ) ), 0.5f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).parameter( ml7::Vector3f( -9.0f, -1.0f, 0.0f ) ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).parameter( ml7::Vector3f( -15.0f, 4.0f, -4.0f ) ), 1.0f );

    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).closest_point( ml7::Vector3f( 7.0f, -7.0f, 7.0f ) ), ml7::Vector3f( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).closest_point( ml7::Vector3f( 1.0f, -2.0f, 3.0f ) ), ml7::Vector3f( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).closest_point( ml7::Vector3f( -2.0f, 0.5f, 1.0f ) ), ml7::Vector3f( -2.0f, 0.5f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).closest_point( ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).closest_point( ml7::Vector3f( -11.0f, 8.0f, -5.0f ) ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).closest_point( ml7::Vector3f( 11.0f, -3.0f, 6.0f ) ), ml7::Vector3f( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).closest_point( ml7::Vector3f( 5.0f, 2.0f, 2.0f ) ), ml7::Vector3f( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).closest_point( ml7::Vector3f( 2.0f, 4.5f, 0.0f ) ), ml7::Vector3f( -2.0f, 0.5f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).closest_point( ml7::Vector3f( -1.0f, 7.0f, -2.0f ) ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).closest_point( ml7::Vector3f( -7.0f, 12.0f, -6.0f ) ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).closest_point( ml7::Vector3f( 3.0f, -11.0f, 8.0f ) ), ml7::Vector3f( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).closest_point( ml7::Vector3f( -3.0f, -6.0f, 4.0f ) ), ml7::Vector3f( 1.0f, -2.0f, 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).closest_point( ml7::Vector3f( -6.0f, -3.5f, 2.0f ) ), ml7::Vector3f( -2.0f, 0.5f, 1.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).closest_point( ml7::Vector3f( -9.0f, -1.0f, 0.0f ) ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).closest_point( ml7::Vector3f( -15.0f, 4.0f, -4.0f ) ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) );

    TESTLABS_CHECK_EQ_FLT( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance( ml7::Vector3f( 7.0f, -7.0f, 7.0f ) ), 8.7749644 );
    TESTLABS_CHECK_EQ_FLT( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance( ml7::Vector3f( 1.0f, -2.0f, 3.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_FLT( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance( ml7::Vector3f( -2.0f, 0.5f, 1.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_FLT( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance( ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ_FLT( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance( ml7::Vector3f( -11.0f, 8.0f, -5.0f ) ), 8.7749644 );
    TESTLABS_CHECK_EQ_FLT( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance( ml7::Vector3f( 11.0f, -3.0f, 6.0f ) ), 10.488089f );
    TESTLABS_CHECK_EQ_FLT( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance( ml7::Vector3f( 5.0f, 2.0f, 2.0f ) ), 5.7445626f );
    TESTLABS_CHECK_EQ_FLT( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance( ml7::Vector3f( 2.0f, 4.5f, 0.0f ) ), 5.7445626f );
    TESTLABS_CHECK_EQ_FLT( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance( ml7::Vector3f( -1.0f, 7.0f, -2.0f ) ), 5.7445626f );
    TESTLABS_CHECK_EQ_FLT( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance( ml7::Vector3f( -7.0f, 12.0f, -6.0f ) ), 10.488089f );
    TESTLABS_CHECK_EQ_FLT( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance( ml7::Vector3f( 3.0f, -11.0f, 8.0f ) ), 10.488089f );
    TESTLABS_CHECK_EQ_FLT( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance( ml7::Vector3f( -3.0f, -6.0f, 4.0f ) ), 5.7445626f );
    TESTLABS_CHECK_EQ_FLT( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance( ml7::Vector3f( -6.0f, -3.5f, 2.0f ) ), 5.7445626f );
    TESTLABS_CHECK_EQ_FLT( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance( ml7::Vector3f( -9.0f, -1.0f, 0.0f ) ), 5.7445626f );
    TESTLABS_CHECK_EQ_FLT( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance( ml7::Vector3f( -15.0f, 4.0f, -4.0f ) ), 10.488089f );

    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance_squared( ml7::Vector3f( 7.0f, -7.0f, 7.0f ) ), 77.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance_squared( ml7::Vector3f( 1.0f, -2.0f, 3.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance_squared( ml7::Vector3f( -2.0f, 0.5f, 1.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance_squared( ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance_squared( ml7::Vector3f( -11.0f, 8.0f, -5.0f ) ), 77.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance_squared( ml7::Vector3f( 11.0f, -3.0f, 6.0f ) ), 110.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance_squared( ml7::Vector3f( 5.0f, 2.0f, 2.0f ) ), 33.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance_squared( ml7::Vector3f( 2.0f, 4.5f, 0.0f ) ), 33.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance_squared( ml7::Vector3f( -1.0f, 7.0f, -2.0f ) ), 33.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance_squared( ml7::Vector3f( -7.0f, 12.0f, -6.0f ) ), 110.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance_squared( ml7::Vector3f( 3.0f, -11.0f, 8.0f ) ), 110.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance_squared( ml7::Vector3f( -3.0f, -6.0f, 4.0f ) ), 33.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance_squared( ml7::Vector3f( -6.0f, -3.5f, 2.0f ) ), 33.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance_squared( ml7::Vector3f( -9.0f, -1.0f, 0.0f ) ), 33.0f );
    TESTLABS_CHECK_EQ( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -5.0f, 3.0f, -1.0f ) ).distance_squared( ml7::Vector3f( -15.0f, 4.0f, -4.0f ) ), 110.0f );

    TESTLABS_CHECK( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -4.0f, 5.0f, -6.0f ) ) == ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -4.0f, 5.0f, -6.0f ) ) );
    TESTLABS_CHECK( !(ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -4.0f, 5.0f, -6.0f ) ) == ml7::LineSegment3f( ml7::Vector3f( -1.0f, 2.0f, -3.0f ), ml7::Vector3f( 4.0f, -5.0f, 6.0f ) )) );

    TESTLABS_CHECK( ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -4.0f, 5.0f, -6.0f ) ) != ml7::LineSegment3f( ml7::Vector3f( -1.0f, 2.0f, -3.0f ), ml7::Vector3f( 4.0f, -5.0f, 6.0f ) ) );
    TESTLABS_CHECK( !(ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -4.0f, 5.0f, -6.0f ) ) != ml7::LineSegment3f( ml7::Vector3f( 1.0f, -2.0f, 3.0f ), ml7::Vector3f( -4.0f, 5.0f, -6.0f ) )) );
}

#ifndef F77_TESTS_ML7_ANGLE_H
#define F77_TESTS_ML7_ANGLE_H

#include <MathLabs/Angle.h>

#include <TestLabs/TestSuite.h>

#include "./shared.h"



namespace cl7 {
    inline
    cl7::u8string to_string(const ml7::AngleF& a) { return cl7::to_string(a.radians); }
}



namespace ml7 {
    inline
    ml7::AngleF round(ml7::AngleF a, unsigned num_decimals) { a.radians = ml7::round(a.radians, num_decimals); return a; }
}



TESTLABS_CASE( u8"CoreLabs:  Angle:  " )
{
    TESTLABS_CHECK_EQ( ml7::AngleF(), ml7::AngleF( 0.0f ) );

    TESTLABS_CHECK_EQ( ml7::AngleF::from_radians( -3.1415927f ), ml7::AngleF( -3.1415927f ) );
    TESTLABS_CHECK_EQ( ml7::AngleF::from_degrees( -180.0f ), ml7::AngleF( -3.1415927f ) );
    TESTLABS_CHECK_EQ( ml7::AngleF::from_half_cycle( -128.0f, 128.0f ), ml7::AngleF( -3.1415927f ) );
    TESTLABS_CHECK_EQ( ml7::AngleF::from_cycle( -50.0f, 100.0f ), ml7::AngleF( -3.1415927f ) );
    TESTLABS_CHECK_EQ( ml7::AngleF::from_half_cycle<128.0f>( -128.0f ), ml7::AngleF( -3.1415927f ) );
    TESTLABS_CHECK_EQ( ml7::AngleF::from_cycle<100.0f>( -50.0f ), ml7::AngleF( -3.1415927f ) );

    TESTLABS_CHECK_EQ( ml7::AngleF( -3.1415927f ).to_radians(), -3.1415927f );
    TESTLABS_CHECK_EQ( ml7::AngleF( -3.1415927f ).to_degrees(), -180.0f );
    TESTLABS_CHECK_EQ( ml7::AngleF( -3.1415927f ).to_half_cycle( 128.0f ), -128.0f );
    TESTLABS_CHECK_EQ( ml7::AngleF( -3.1415927f ).to_cycle( 100.0f ), -50.0f );
    TESTLABS_CHECK_EQ( ml7::AngleF( -3.1415927f ).to_half_cycle<128.0f>(), -128.0f );
    TESTLABS_CHECK_EQ( ml7::AngleF( -3.1415927f ).to_cycle<100.0f>(), -50.0f );

    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -4.0f * 3.1415927f ).normalized_symmetric(), 5 ), ml7::round( ml7::AngleF( 0.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -3.5f * 3.1415927f ).normalized_symmetric(), 5 ), ml7::round( ml7::AngleF( 0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -3.0f * 3.1415927f ).normalized_symmetric(), 5 ), ml7::round( ml7::AngleF( -1.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -2.5f * 3.1415927f ).normalized_symmetric(), 5 ), ml7::round( ml7::AngleF( -0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -2.0f * 3.1415927f ).normalized_symmetric(), 5 ), ml7::round( ml7::AngleF( 0.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -1.5f * 3.1415927f ).normalized_symmetric(), 5 ), ml7::round( ml7::AngleF( 0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -1.0f * 3.1415927f ).normalized_symmetric(), 5 ), ml7::round( ml7::AngleF( -1.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -0.5f * 3.1415927f ).normalized_symmetric(), 5 ), ml7::round( ml7::AngleF( -0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 0.0f * 3.1415927f ).normalized_symmetric(), 5 ), ml7::round( ml7::AngleF( 0.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 0.5f * 3.1415927f ).normalized_symmetric(), 5 ), ml7::round( ml7::AngleF( 0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 1.0f * 3.1415927f ).normalized_symmetric(), 5 ), ml7::round( ml7::AngleF( 1.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 1.5f * 3.1415927f ).normalized_symmetric(), 5 ), ml7::round( ml7::AngleF( -0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 2.0f * 3.1415927f ).normalized_symmetric(), 5 ), ml7::round( ml7::AngleF( 0.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 2.5f * 3.1415927f ).normalized_symmetric(), 5 ), ml7::round( ml7::AngleF( 0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 3.0f * 3.1415927f ).normalized_symmetric(), 5 ), ml7::round( ml7::AngleF( -1.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 3.5f * 3.1415927f ).normalized_symmetric(), 5 ), ml7::round( ml7::AngleF( -0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 4.0f * 3.1415927f ).normalized_symmetric(), 5 ), ml7::round( ml7::AngleF( 0.0f * 3.1415927f ), 5 ) );

    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -4.0f * 3.1415927f ).normalized_asymmetric(), 5 ), ml7::round( ml7::AngleF( 2.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -3.5f * 3.1415927f ).normalized_asymmetric(), 5 ), ml7::round( ml7::AngleF( 0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -3.0f * 3.1415927f ).normalized_asymmetric(), 5 ), ml7::round( ml7::AngleF( 1.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -2.5f * 3.1415927f ).normalized_asymmetric(), 5 ), ml7::round( ml7::AngleF( 1.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -2.0f * 3.1415927f ).normalized_asymmetric(), 5 ), ml7::round( ml7::AngleF( 2.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -1.5f * 3.1415927f ).normalized_asymmetric(), 5 ), ml7::round( ml7::AngleF( 0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -1.0f * 3.1415927f ).normalized_asymmetric(), 5 ), ml7::round( ml7::AngleF( 1.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -0.5f * 3.1415927f ).normalized_asymmetric(), 5 ), ml7::round( ml7::AngleF( 1.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 0.0f * 3.1415927f ).normalized_asymmetric(), 5 ), ml7::round( ml7::AngleF( 0.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 0.5f * 3.1415927f ).normalized_asymmetric(), 5 ), ml7::round( ml7::AngleF( 0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 1.0f * 3.1415927f ).normalized_asymmetric(), 5 ), ml7::round( ml7::AngleF( 1.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 1.5f * 3.1415927f ).normalized_asymmetric(), 5 ), ml7::round( ml7::AngleF( 1.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 2.0f * 3.1415927f ).normalized_asymmetric(), 5 ), ml7::round( ml7::AngleF( 2.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 2.5f * 3.1415927f ).normalized_asymmetric(), 5 ), ml7::round( ml7::AngleF( 0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 3.0f * 3.1415927f ).normalized_asymmetric(), 5 ), ml7::round( ml7::AngleF( 1.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 3.5f * 3.1415927f ).normalized_asymmetric(), 5 ), ml7::round( ml7::AngleF( 1.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 4.0f * 3.1415927f ).normalized_asymmetric(), 5 ), ml7::round( ml7::AngleF( 0.0f * 3.1415927f ), 5 ) );

    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -4.0f * 3.1415927f ).normalize_symmetric(), 5 ), ml7::round( ml7::AngleF( 0.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -3.5f * 3.1415927f ).normalize_symmetric(), 5 ), ml7::round( ml7::AngleF( 0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -3.0f * 3.1415927f ).normalize_symmetric(), 5 ), ml7::round( ml7::AngleF( -1.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -2.5f * 3.1415927f ).normalize_symmetric(), 5 ), ml7::round( ml7::AngleF( -0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -2.0f * 3.1415927f ).normalize_symmetric(), 5 ), ml7::round( ml7::AngleF( 0.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -1.5f * 3.1415927f ).normalize_symmetric(), 5 ), ml7::round( ml7::AngleF( 0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -1.0f * 3.1415927f ).normalize_symmetric(), 5 ), ml7::round( ml7::AngleF( -1.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -0.5f * 3.1415927f ).normalize_symmetric(), 5 ), ml7::round( ml7::AngleF( -0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 0.0f * 3.1415927f ).normalize_symmetric(), 5 ), ml7::round( ml7::AngleF( 0.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 0.5f * 3.1415927f ).normalize_symmetric(), 5 ), ml7::round( ml7::AngleF( 0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 1.0f * 3.1415927f ).normalize_symmetric(), 5 ), ml7::round( ml7::AngleF( 1.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 1.5f * 3.1415927f ).normalize_symmetric(), 5 ), ml7::round( ml7::AngleF( -0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 2.0f * 3.1415927f ).normalize_symmetric(), 5 ), ml7::round( ml7::AngleF( 0.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 2.5f * 3.1415927f ).normalize_symmetric(), 5 ), ml7::round( ml7::AngleF( 0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 3.0f * 3.1415927f ).normalize_symmetric(), 5 ), ml7::round( ml7::AngleF( -1.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 3.5f * 3.1415927f ).normalize_symmetric(), 5 ), ml7::round( ml7::AngleF( -0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 4.0f * 3.1415927f ).normalize_symmetric(), 5 ), ml7::round( ml7::AngleF( 0.0f * 3.1415927f ), 5 ) );

    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -4.0f * 3.1415927f ).normalize_asymmetric(), 5 ), ml7::round( ml7::AngleF( 2.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -3.5f * 3.1415927f ).normalize_asymmetric(), 5 ), ml7::round( ml7::AngleF( 0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -3.0f * 3.1415927f ).normalize_asymmetric(), 5 ), ml7::round( ml7::AngleF( 1.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -2.5f * 3.1415927f ).normalize_asymmetric(), 5 ), ml7::round( ml7::AngleF( 1.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -2.0f * 3.1415927f ).normalize_asymmetric(), 5 ), ml7::round( ml7::AngleF( 2.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -1.5f * 3.1415927f ).normalize_asymmetric(), 5 ), ml7::round( ml7::AngleF( 0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -1.0f * 3.1415927f ).normalize_asymmetric(), 5 ), ml7::round( ml7::AngleF( 1.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( -0.5f * 3.1415927f ).normalize_asymmetric(), 5 ), ml7::round( ml7::AngleF( 1.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 0.0f * 3.1415927f ).normalize_asymmetric(), 5 ), ml7::round( ml7::AngleF( 0.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 0.5f * 3.1415927f ).normalize_asymmetric(), 5 ), ml7::round( ml7::AngleF( 0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 1.0f * 3.1415927f ).normalize_asymmetric(), 5 ), ml7::round( ml7::AngleF( 1.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 1.5f * 3.1415927f ).normalize_asymmetric(), 5 ), ml7::round( ml7::AngleF( 1.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 2.0f * 3.1415927f ).normalize_asymmetric(), 5 ), ml7::round( ml7::AngleF( 2.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 2.5f * 3.1415927f ).normalize_asymmetric(), 5 ), ml7::round( ml7::AngleF( 0.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 3.0f * 3.1415927f ).normalize_asymmetric(), 5 ), ml7::round( ml7::AngleF( 1.0f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 3.5f * 3.1415927f ).normalize_asymmetric(), 5 ), ml7::round( ml7::AngleF( 1.5f * 3.1415927f ), 5 ) );
    TESTLABS_CHECK_EQ( ml7::round( ml7::AngleF( 4.0f * 3.1415927f ).normalize_asymmetric(), 5 ), ml7::round( ml7::AngleF( 0.0f * 3.1415927f ), 5 ) );

    TESTLABS_CHECK_EQ( +ml7::AngleF( 1.0f ), ml7::AngleF( 1.0f ) );
    TESTLABS_CHECK_EQ( -ml7::AngleF( 1.0f ), ml7::AngleF( -1.0f ) );
    TESTLABS_CHECK_EQ( ml7::AngleF( 1.0f ) + ml7::AngleF( 3.0f ), ml7::AngleF( 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::AngleF( 1.0f ) - ml7::AngleF( 3.0f ), ml7::AngleF( -2.0f ) );
    TESTLABS_CHECK_EQ( ml7::AngleF( 1.0f ) * 3.0f, ml7::AngleF( 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::AngleF( 3.0f ) / 3.0f, ml7::AngleF( 1.0f ) );
    TESTLABS_CHECK_EQ( 3.0f * ml7::AngleF( 1.0f ), ml7::AngleF( 3.0f ) );

    TESTLABS_CHECK_EQ( ml7::AngleF( 1.0f ) += ml7::AngleF( 3.0f ), ml7::AngleF( 4.0f ) );
    TESTLABS_CHECK_EQ( ml7::AngleF( 1.0f ) -= ml7::AngleF( 3.0f ), ml7::AngleF( -2.0f ) );
    TESTLABS_CHECK_EQ( ml7::AngleF( 1.0f ) *= 3.0f, ml7::AngleF( 3.0f ) );
    TESTLABS_CHECK_EQ( ml7::AngleF( 3.0f ) /= 3.0f, ml7::AngleF( 1.0f ) );

    TESTLABS_CHECK( ml7::AngleF( 1.0f ) == ml7::AngleF( 1.0f ) );
    TESTLABS_CHECK( !(ml7::AngleF( 1.0f ) == ml7::AngleF( 0.0f )) );

    TESTLABS_CHECK( ml7::AngleF( 1.0f ) != ml7::AngleF( 0.0f ) );
    TESTLABS_CHECK( !(ml7::AngleF( 1.0f ) != ml7::AngleF( 1.0f )) );

    TESTLABS_CHECK( ml7::AngleF( 0.0f ) < ml7::AngleF( 1.0f ) );
    TESTLABS_CHECK( !(ml7::AngleF( 1.0f ) < ml7::AngleF( 0.0f )) );

    TESTLABS_CHECK( ml7::AngleF( 1.0f ) > ml7::AngleF( 0.0f ) );
    TESTLABS_CHECK( !(ml7::AngleF( 0.0f ) > ml7::AngleF( 1.0f )) );

    TESTLABS_CHECK( ml7::AngleF( 0.0f ) <= ml7::AngleF( 1.0f ) );
    TESTLABS_CHECK( ml7::AngleF( 1.0f ) <= ml7::AngleF( 1.0f ) );
    TESTLABS_CHECK( !(ml7::AngleF( 1.0f ) <= ml7::AngleF( 0.0f )) );

    TESTLABS_CHECK( ml7::AngleF( 1.0f ) >= ml7::AngleF( 0.0f ) );
    TESTLABS_CHECK( ml7::AngleF( 1.0f ) >= ml7::AngleF( 1.0f ) );
    TESTLABS_CHECK( !(ml7::AngleF( 0.0f ) >= ml7::AngleF( 1.0f )) );
}



#endif // F77_TESTS_ML7_ANGLE_H

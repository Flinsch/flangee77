#ifndef F77_TESTS_ML7_OPS_H
#define F77_TESTS_ML7_OPS_H

#include <MathLabs/ops.h>

#include <TestLabs/TestSuite.h>

#include "./shared.h"



TESTLABS_CASE( u8"MathLabs:  functions:  clamp" )
{
    TESTLABS_CHECK_EQ( (ml7::ops::clamp<0.0f, 0.0f>{}( 0.0f )), 0.0f );
    TESTLABS_CHECK_EQ( (ml7::ops::clamp<2.0f, 3.0f>{}( 1.0f )), 2.0f );
    TESTLABS_CHECK_EQ( (ml7::ops::clamp<-1.0f, 0.5f>{}( 1.0f )), 0.5f );
    TESTLABS_CHECK_EQ( (ml7::ops::clamp<1.5f, -1.0f>{}( 1.0f )), 1.5f );
}

TESTLABS_CASE( u8"MathLabs:  functions:  clamp01" )
{
    TESTLABS_CHECK_EQ( ml7::ops::clamp01{}( -0.5f ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::ops::clamp01{}( 0.0f ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::ops::clamp01{}( 0.5f ), 0.5f );
    TESTLABS_CHECK_EQ( ml7::ops::clamp01{}( 1.0f ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::ops::clamp01{}( 1.5f ), 1.0f );
}



TESTLABS_CASE( u8"MathLabs:  functions:  ramp" )
{
    TESTLABS_CHECK_EQ( (ml7::ops::ramp<1.0f, 1.0f>{}( 2.0f )), 1.0f );
    TESTLABS_CHECK_EQ( (ml7::ops::ramp<1.0f, 1.0f>{}( 1.0f )), 0.0f );
    TESTLABS_CHECK_EQ( (ml7::ops::ramp<1.0f, 1.0f>{}( 0.5f )), 0.0f );
    TESTLABS_CHECK_EQ( (ml7::ops::ramp<1.0f, 2.0f>{}( 3.0f )), 4.0f );
    TESTLABS_CHECK_EQ( (ml7::ops::ramp<1.0f, 0.5f>{}( 1.5f )), 0.25f );
    TESTLABS_CHECK_EQ( (ml7::ops::ramp<-2.0f, 1.0f>{}( -1.0f )), 1.0f );
    TESTLABS_CHECK_EQ( (ml7::ops::ramp<-2.0f, 1.0f>{}( -3.0f )), 0.0f );
    TESTLABS_CHECK_EQ( (ml7::ops::ramp<1.0f, -1.0f>{}( 2.0f )), -1.0f );
    TESTLABS_CHECK_EQ( (ml7::ops::ramp<1.0f, -2.0f>{}( 0.5f )), 0.0f );
}

TESTLABS_CASE( u8"MathLabs:  functions:  (unit) ramp" )
{
    TESTLABS_CHECK_EQ( ml7::ops::ramp{}( -0.5f ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::ops::ramp{}( 0.0f ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::ops::ramp{}( 0.5f ), 0.5f );
    TESTLABS_CHECK_EQ( ml7::ops::ramp{}( 1.0f ), 1.0f );
    TESTLABS_CHECK_EQ( ml7::ops::ramp{}( 1.5f ), 1.5f );
}



#endif // F77_TESTS_ML7_OPS_H

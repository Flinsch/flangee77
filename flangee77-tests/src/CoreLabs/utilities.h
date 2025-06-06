#ifndef F77_TESTS_CL7_UTILITIES_H
#define F77_TESTS_CL7_UTILITIES_H

#include <CoreLabs/utilities.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



TESTLABS_CASE( u8"CoreLabs:  utilities:  coalesce" )
{
    TESTLABS_ASSERT_EQ( cl7::coalesce(0, 0), 0 );
    TESTLABS_ASSERT_EQ( cl7::coalesce(1, 0), 1 );
    TESTLABS_ASSERT_EQ( cl7::coalesce(0, 2), 2 );
    TESTLABS_ASSERT_EQ( cl7::coalesce(1, 2), 1 );
    TESTLABS_ASSERT_EQ( cl7::coalesce(2, 1), 2 );
    TESTLABS_ASSERT_EQ( cl7::coalesce(-1, 1), -1 );
    TESTLABS_ASSERT_EQ( cl7::coalesce(1, -1), 1 );
}



#endif // F77_TESTS_CL7_UTILITIES_H

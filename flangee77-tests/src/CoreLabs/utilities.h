#ifndef F77_TESTS_CL7_UTILITIES_H
#define F77_TESTS_CL7_UTILITIES_H

#include <CoreLabs/utilities.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



TESTLABS_CASE( u8"CoreLabs:  utilities:  coalesce" )
{
    TESTLABS_ASSERT_EQ( cl7::coalesce( 0, 0 ), 0 );
    TESTLABS_ASSERT_EQ( cl7::coalesce( 1, 0 ), 1 );
    TESTLABS_ASSERT_EQ( cl7::coalesce( 0, 2 ), 2 );
    TESTLABS_ASSERT_EQ( cl7::coalesce( 1, 2 ), 1 );
    TESTLABS_ASSERT_EQ( cl7::coalesce( 2, 1 ), 2 );
    TESTLABS_ASSERT_EQ( cl7::coalesce( -1, 1 ), -1 );
    TESTLABS_ASSERT_EQ( cl7::coalesce( 1, -1 ), 1 );
}



TESTLABS_CASE( u8"CoreLabs:  utilities:  unordered_equal" )
{
    TESTLABS_ASSERT_EQ( cl7::unordered_equal( std::vector<int>{}, std::vector<int>{} ), true );
    TESTLABS_ASSERT_EQ( cl7::unordered_equal( std::vector<int>{ 1, 2, 2, 3 }, std::vector<int>{ 3, 2, 1, 2 } ), true );
    TESTLABS_ASSERT_EQ( cl7::unordered_equal( std::vector<int>{ 1, 2, 2, 3 }, std::vector<int>{ 1, 2, 3, 3 } ), false );
    TESTLABS_ASSERT_EQ( cl7::unordered_equal( std::vector<int>{ 1, 2, 3, 2 }, std::vector<int>{ 1, 2, 3 } ), false );
    TESTLABS_ASSERT_EQ( cl7::unordered_equal( std::vector<int>{ 1, 2, 3 }, std::vector<int>{ 1, 2, 3, 2 } ), false );

    TESTLABS_ASSERT_EQ( cl7::unordered_equal( std::vector<std::shared_ptr<int>>{ std::make_shared<int>(1), std::make_shared<int>(2), std::make_shared<int>(2), std::make_shared<int>(3) }, std::vector<std::shared_ptr<int>>{ std::make_shared<int>(3), std::make_shared<int>(2), std::make_shared<int>(1), std::make_shared<int>(2) }, [](const auto& p) { return *p; } ), true );
    TESTLABS_ASSERT_EQ( cl7::unordered_equal( std::vector<std::shared_ptr<int>>{ std::make_shared<int>(1), std::make_shared<int>(2), std::make_shared<int>(2), std::make_shared<int>(3) }, std::vector<std::shared_ptr<int>>{ std::make_shared<int>(1), std::make_shared<int>(2), std::make_shared<int>(3), std::make_shared<int>(3) }, [](const auto& p) { return *p; } ), false );
}



#endif // F77_TESTS_CL7_UTILITIES_H

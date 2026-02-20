
#include <TestLabs/TestSuite.h>

#include <CoreLabs/utilities.h>

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



TESTLABS_CASE( u8"CoreLabs:  utilities:  flip_if" )
{
    TESTLABS_ASSERT( cl7::flip_if( false, 1.0f, 2.5f ) == std::make_pair( 1.0f, 2.5f ) );
    TESTLABS_ASSERT( cl7::flip_if( true, 1.0f, 2.5f ) == std::make_pair( 2.5f, 1.0f ) );
}

TESTLABS_CASE( u8"CoreLabs:  utilities:  flip_unless" )
{
    TESTLABS_ASSERT( cl7::flip_unless( true, 1.0f, 2.5f ) == std::make_pair( 1.0f, 2.5f ) );
    TESTLABS_ASSERT( cl7::flip_unless( false, 1.0f, 2.5f ) == std::make_pair( 2.5f, 1.0f ) );
}



TESTLABS_CASE( u8"CoreLabs:  utilities:  unordered_equal" )
{
    TESTLABS_ASSERT_EQ( cl7::unordered_equal( std::vector<int>{}, std::vector<int>{} ), true );
    TESTLABS_ASSERT_EQ( cl7::unordered_equal( std::vector<int>{ 1, 2, 2, 3 }, std::vector<int>{ 3, 2, 1, 2 } ), true );
    TESTLABS_ASSERT_EQ( cl7::unordered_equal( std::vector<int>{ 1, 2, 2, 3 }, std::vector<int>{ 1, 2, 3, 3 } ), false );
    TESTLABS_ASSERT_EQ( cl7::unordered_equal( std::vector<int>{ 1, 2, 3, 2 }, std::vector<int>{ 1, 2, 3 } ), false );
    TESTLABS_ASSERT_EQ( cl7::unordered_equal( std::vector<int>{ 1, 2, 3 }, std::vector<int>{ 1, 2, 3, 2 } ), false );

    TESTLABS_ASSERT_EQ( cl7::unordered_equal( std::vector<std::shared_ptr<int>>{ std::make_shared<int>(1), std::make_shared<int>(2), std::make_shared<int>(2), std::make_shared<int>(3) }, std::vector<std::shared_ptr<int>>{ std::make_shared<int>(3), std::make_shared<int>(2), std::make_shared<int>(1), std::make_shared<int>(2) }, [](const auto& a, const auto& b) { return *a == *b; } ), true );
    TESTLABS_ASSERT_EQ( cl7::unordered_equal( std::vector<std::shared_ptr<int>>{ std::make_shared<int>(1), std::make_shared<int>(2), std::make_shared<int>(2), std::make_shared<int>(3) }, std::vector<std::shared_ptr<int>>{ std::make_shared<int>(1), std::make_shared<int>(2), std::make_shared<int>(3), std::make_shared<int>(3) }, [](const auto& a, const auto& b) { return *a == *b; } ), false );

    TESTLABS_ASSERT_EQ( cl7::unordered_equal( std::vector<std::shared_ptr<int>>{ std::make_shared<int>(1), std::make_shared<int>(2), std::make_shared<int>(2), std::make_shared<int>(3) }, std::vector<std::shared_ptr<int>>{ std::make_shared<int>(3), std::make_shared<int>(2), std::make_shared<int>(1), std::make_shared<int>(2) }, {}, [](const auto& p) { return *p; }, [](const auto& p) { return *p; } ), true );
    TESTLABS_ASSERT_EQ( cl7::unordered_equal( std::vector<std::shared_ptr<int>>{ std::make_shared<int>(1), std::make_shared<int>(2), std::make_shared<int>(2), std::make_shared<int>(3) }, std::vector<std::shared_ptr<int>>{ std::make_shared<int>(1), std::make_shared<int>(2), std::make_shared<int>(3), std::make_shared<int>(3) }, {}, [](const auto& p) { return *p; }, [](const auto& p) { return *p; } ), false );
}

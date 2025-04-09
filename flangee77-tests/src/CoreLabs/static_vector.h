#ifndef F77_TESTS_CL7_STATICVECTOR_H
#define F77_TESTS_CL7_STATICVECTOR_H

#include <CoreLabs/static_vector.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



TESTLABS_CASE( u8"CoreLabs:  static_vector::static_vector()" )
{
    cl7::static_vector<int, 10> static_vector;

    TESTLABS_CHECK( static_vector.empty() );
    TESTLABS_CHECK_EQ( static_vector.size(), 0 );
}

TESTLABS_CASE( u8"CoreLabs:  static_vector::static_vector(It first, It last)" )
{
    std::vector<int> vector = {
        { 1 },
        { 2 },
        { 3 },
        { 4 },
    };

    cl7::static_vector<int, 10> static_vector{ vector.begin() + 1, vector.end() - 1 };

    TESTLABS_CHECK( !static_vector.empty() );
    TESTLABS_CHECK_EQ( static_vector.size(), 2 );

    TESTLABS_CHECK_EQ( static_vector[0], 2 );
    TESTLABS_CHECK_EQ( static_vector[1], 3 );

    TESTLABS_CHECK_EQ( static_vector.at(0), 2 );
    TESTLABS_CHECK_EQ( static_vector.at(1), 3 );
}

TESTLABS_CASE( u8"CoreLabs:  static_vector::static_vector(const static_vector&)" )
{
    std::vector<int> vector = {
        { 1 },
        { 2 },
        { 3 },
        { 4 },
    };

    cl7::static_vector<int, 10> other{ vector.begin() + 1, vector.end() - 1 };

    cl7::static_vector<int, 10> static_vector{ other };

    TESTLABS_CHECK( !static_vector.empty() );
    TESTLABS_CHECK_EQ( static_vector.size(), 2 );

    TESTLABS_CHECK_EQ( static_vector[0], 2 );
    TESTLABS_CHECK_EQ( static_vector[1], 3 );

    TESTLABS_CHECK( static_vector == other );
}

TESTLABS_CASE( u8"CoreLabs:  static_vector::static_vector(static_vector&&)" )
{
    std::vector<int> vector = {
        { 1 },
        { 2 },
        { 3 },
        { 4 },
    };

    cl7::static_vector<int, 10> other{ vector.begin() + 1, vector.end() - 1 };

    cl7::static_vector<int, 10> static_vector{ std::move(other) };

    TESTLABS_CHECK( !static_vector.empty() );
    TESTLABS_CHECK_EQ( static_vector.size(), 2 );

    TESTLABS_CHECK_EQ( static_vector[0], 2 );
    TESTLABS_CHECK_EQ( static_vector[1], 3 );
}

TESTLABS_CASE( u8"CoreLabs:  static_vector::static_vector(initializer_list)" )
{
    cl7::static_vector<int, 10> static_vector{
        { 1 },
        { 2 },
        { 3 },
        { 4 },
    };

    TESTLABS_CHECK( !static_vector.empty() );
    TESTLABS_CHECK_EQ( static_vector.size(), 4 );

    TESTLABS_CHECK_EQ( static_vector[0], 1 );
    TESTLABS_CHECK_EQ( static_vector[1], 2 );
    TESTLABS_CHECK_EQ( static_vector[2], 3 );
    TESTLABS_CHECK_EQ( static_vector[3], 4 );
}

TESTLABS_CASE( u8"CoreLabs:  static_vector::operator=(initializer_list)" )
{
    cl7::static_vector<int, 10> static_vector{
        { -1 },
        { -3 },
    };
    static_vector = {
        { 1 },
        { 2 },
        { 3 },
        { 4 },
    };

    TESTLABS_CHECK( !static_vector.empty() );
    TESTLABS_CHECK_EQ( static_vector.size(), 4 );

    TESTLABS_CHECK_EQ( static_vector[0], 1 );
    TESTLABS_CHECK_EQ( static_vector[1], 2 );
    TESTLABS_CHECK_EQ( static_vector[2], 3 );
    TESTLABS_CHECK_EQ( static_vector[3], 4 );
}

TESTLABS_CASE( u8"CoreLabs:  static_vector::swap(static_vector&)" )
{
    std::vector<int> vector = {
        { 1 },
        { 2 },
        { 3 },
        { 4 },
    };

    cl7::static_vector<int, 10> static_vector{ vector.begin() + 0, vector.begin() + 1 };
    cl7::static_vector<int, 10> other{ vector.begin() + 1, vector.begin() + 4 };

    TESTLABS_CHECK_EQ( static_vector.size(), 1 );
    TESTLABS_CHECK_EQ( other.size(), 3 );

    TESTLABS_CHECK_EQ( static_vector[0], 1 );
    TESTLABS_CHECK_EQ( other[0], 2 );
    TESTLABS_CHECK_EQ( other[1], 3 );
    TESTLABS_CHECK_EQ( other[2], 4 );

    static_vector.swap(other);

    TESTLABS_CHECK_EQ( static_vector.size(), 3 );
    TESTLABS_CHECK_EQ( other.size(), 1 );

    TESTLABS_CHECK_EQ( static_vector[0], 2 );
    TESTLABS_CHECK_EQ( static_vector[1], 3 );
    TESTLABS_CHECK_EQ( static_vector[2], 4 );
    TESTLABS_CHECK_EQ( other[0], 1 );
}



TESTLABS_CASE( u8"CoreLabs:  static_vector:  clear" )
{
    cl7::static_vector<int, 10> static_vector{
        { 1 },
        { 2 },
        { 3 },
        { 4 },
    };

    TESTLABS_CHECK_EQ( static_vector.size(), 4 );
    static_vector.clear();
    TESTLABS_CHECK( static_vector.empty() );
}



TESTLABS_CASE( u8"CoreLabs:  static_vector:  insert" )
{
    cl7::static_vector<int, 10> static_vector;

    auto it = static_vector.insert( static_vector.end(), 6 );
    TESTLABS_CHECK_EQ( *it, 6 );

    it = static_vector.insert( static_vector.begin(), 7 );
    TESTLABS_CHECK_EQ( *it, 7 );

    TESTLABS_CHECK_EQ( static_vector.size(), 2 );

    TESTLABS_CHECK_EQ( static_vector[0], 7 );
    TESTLABS_CHECK_EQ( static_vector[1], 6 );

    it = static_vector.insert( static_vector.begin() + 1, 5 );
    TESTLABS_CHECK_EQ( *it, 5 );

    TESTLABS_CHECK_EQ( static_vector.size(), 3 );

    TESTLABS_CHECK_EQ( static_vector[0], 7 );
    TESTLABS_CHECK_EQ( static_vector[1], 5 );
    TESTLABS_CHECK_EQ( static_vector[2], 6 );
}



TESTLABS_CASE( u8"CoreLabs:  static_vector:  emplace" )
{
    cl7::static_vector<int, 10> static_vector;

    static_vector.emplace_back( 6 );
    static_vector.emplace_back( 7 );

    TESTLABS_CHECK_EQ( static_vector[0], 6 );
    TESTLABS_CHECK_EQ( static_vector[1], 7 );
}



TESTLABS_CASE( u8"CoreLabs:  static_vector:  erase" )
{
    cl7::static_vector<int, 10> static_vector{
        { 1 },
        { 2 },
        { 3 },
        { 4 },
    };

    TESTLABS_CHECK_EQ( static_vector.size(), 4 );

    auto it = static_vector.erase( std::next(static_vector.begin()) );
    TESTLABS_CHECK_EQ( static_vector.size(), 3 );
    TESTLABS_CHECK_EQ( static_vector[0], 1 );
    TESTLABS_CHECK_EQ( static_vector[1], 3 );
    TESTLABS_CHECK_EQ( static_vector[2], 4 );

    it = static_vector.erase( static_vector.cbegin() + 1 );
    TESTLABS_CHECK_EQ( static_vector.size(), 2 );
    TESTLABS_CHECK_EQ( static_vector[0], 1 );
    TESTLABS_CHECK_EQ( static_vector[1], 4 );

    it = static_vector.erase( static_vector.begin() );
    TESTLABS_CHECK_EQ( static_vector.size(), 1 );
    TESTLABS_CHECK( *it, 4 );
}



#endif // F77_TESTS_CL7_STATICVECTOR_H

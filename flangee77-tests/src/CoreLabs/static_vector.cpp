
#include <TestLabs/TestSuite.h>

#include <CoreLabs/static_vector.h>

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
        1,
        2,
        3,
        4,
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
        1,
        2,
        3,
        4,
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
        1,
        2,
        3,
        4,
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
        1,
        2,
        3,
        4,
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
        -1,
        -3,
    };
    static_vector = {
        1,
        2,
        3,
        4,
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
        1,
        2,
        3,
        4,
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
        1,
        2,
        3,
        4,
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
        1,
        2,
        3,
        4,
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

    it = static_vector.erase( static_vector.end() - 2 );
    TESTLABS_CHECK_EQ( static_vector.size(), 1 );
    TESTLABS_CHECK_EQ( *it, 4 );
}



TESTLABS_CASE( u8"CoreLabs:  static_vector:  iterate / \"random\" access" )
{
    cl7::static_vector<int, 10> static_vector{
        1,
        2,
        3,
        4,
    };

    TESTLABS_CHECK_EQ( static_vector.size(), 4 );

    size_t i = 0;

    for (const auto& a : static_vector)
    {
        const auto& b = static_vector[i++];
        TESTLABS_CHECK_EQ( a, b );
        TESTLABS_CHECK_EQ( a, i );
        if (i > 0)
            TESTLABS_CHECK_EQ( (static_vector.begin() + i)[-1], *std::prev(static_vector.begin() + i) );
    }

    TESTLABS_CHECK_EQ( i, static_vector.size() );

    for (auto it = static_vector.rbegin(); it != static_vector.rend(); ++it)
    {
        const auto& a = *it;
        const auto& b = static_vector[--i];
        TESTLABS_CHECK_EQ( a, b );
        TESTLABS_CHECK_EQ( it[0], b );
        TESTLABS_CHECK_EQ( a, i + 1 );
        if (i > 0)
            TESTLABS_CHECK_EQ( it[1], *std::next(it) );
    }

    TESTLABS_CHECK_EQ( i, 0 );
}

#ifndef F77_TESTS_CL7_ORDEREDMAP_H
#define F77_TESTS_CL7_ORDEREDMAP_H

#include <CoreLabs/ordered_map.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



TESTLABS_CASE( u8"CoreLabs:  ordered_map::ordered_map()" )
{
    cl7::ordered_map<int, int> ordered_map;

    TESTLABS_CHECK( ordered_map.empty() );
    TESTLABS_CHECK_EQ( ordered_map.size(), 0 );
}

TESTLABS_CASE( u8"CoreLabs:  ordered_map::ordered_map(It first, It last)" )
{
    std::vector<std::pair<int, int>> vector = {
        { 1, 5 },
        { 2, 6 },
        { 3, 7 },
        { 4, 8 },
    };

    cl7::ordered_map<int, int> ordered_map{ vector.begin() + 1, vector.end() - 1 };

    TESTLABS_CHECK( !ordered_map.empty() );
    TESTLABS_CHECK_EQ( ordered_map.size(), 2 );

    TESTLABS_CHECK_EQ( ordered_map[2], 6 );
    TESTLABS_CHECK_EQ( ordered_map[3], 7 );

    TESTLABS_CHECK_EQ( ordered_map.at(2), 6 );
    TESTLABS_CHECK_EQ( ordered_map.at(3), 7 );
}

TESTLABS_CASE( u8"CoreLabs:  ordered_map::ordered_map(const ordered_map&)" )
{
    std::vector<std::pair<int, int>> vector = {
        { 1, 5 },
        { 2, 6 },
        { 3, 7 },
        { 4, 8 },
    };

    cl7::ordered_map<int, int> other{ vector.begin() + 1, vector.end() - 1 };

    cl7::ordered_map<int, int> ordered_map{ other };

    TESTLABS_CHECK( !ordered_map.empty() );
    TESTLABS_CHECK_EQ( ordered_map.size(), 2 );

    TESTLABS_CHECK_EQ( ordered_map[2], 6 );
    TESTLABS_CHECK_EQ( ordered_map[3], 7 );

    TESTLABS_CHECK( ordered_map == other );
}

TESTLABS_CASE( u8"CoreLabs:  ordered_map::ordered_map(ordered_map&&)" )
{
    std::vector<std::pair<int, int>> vector = {
        { 1, 5 },
        { 2, 6 },
        { 3, 7 },
        { 4, 8 },
    };

    cl7::ordered_map<int, int> other{ vector.begin() + 1, vector.end() - 1 };

    cl7::ordered_map<int, int> ordered_map{ std::move(other) };

    TESTLABS_CHECK( !ordered_map.empty() );
    TESTLABS_CHECK_EQ( ordered_map.size(), 2 );

    TESTLABS_CHECK_EQ( ordered_map[2], 6 );
    TESTLABS_CHECK_EQ( ordered_map[3], 7 );
}

TESTLABS_CASE( u8"CoreLabs:  ordered_map::ordered_map(initializer_list)" )
{
    cl7::ordered_map<int, int> ordered_map{
        { 1, 5 },
        { 2, 6 },
        { 3, 7 },
        { 4, 8 },
    };

    TESTLABS_CHECK( !ordered_map.empty() );
    TESTLABS_CHECK_EQ( ordered_map.size(), 4 );

    TESTLABS_CHECK_EQ( ordered_map[1], 5 );
    TESTLABS_CHECK_EQ( ordered_map[2], 6 );
    TESTLABS_CHECK_EQ( ordered_map[3], 7 );
    TESTLABS_CHECK_EQ( ordered_map[4], 8 );
}

TESTLABS_CASE( u8"CoreLabs:  ordered_map::operator=(initializer_list)" )
{
    cl7::ordered_map<int, int> ordered_map{
        { -1, -2 },
        { -3, -4 },
    };
    ordered_map = {
        { 1, 5 },
        { 2, 6 },
        { 3, 7 },
        { 4, 8 },
    };

    TESTLABS_CHECK( !ordered_map.empty() );
    TESTLABS_CHECK_EQ( ordered_map.size(), 4 );

    TESTLABS_CHECK_EQ( ordered_map[1], 5 );
    TESTLABS_CHECK_EQ( ordered_map[2], 6 );
    TESTLABS_CHECK_EQ( ordered_map[3], 7 );
    TESTLABS_CHECK_EQ( ordered_map[4], 8 );
}

TESTLABS_CASE( u8"CoreLabs:  ordered_map::swap(ordered_map&)" )
{
    std::vector<std::pair<int, int>> vector = {
        { 1, 5 },
        { 2, 6 },
        { 3, 7 },
        { 4, 8 },
    };

    cl7::ordered_map<int, int> ordered_map{ vector.begin() + 0, vector.begin() + 1 };
    cl7::ordered_map<int, int> other{ vector.begin() + 1, vector.begin() + 4 };

    TESTLABS_CHECK_EQ( ordered_map.size(), 1 );
    TESTLABS_CHECK_EQ( other.size(), 3 );

    TESTLABS_CHECK_EQ( ordered_map[1], 5 );
    TESTLABS_CHECK_EQ( other[2], 6 );
    TESTLABS_CHECK_EQ( other[3], 7 );
    TESTLABS_CHECK_EQ( other[4], 8 );

    ordered_map.swap(other);

    TESTLABS_CHECK_EQ( ordered_map.size(), 3 );
    TESTLABS_CHECK_EQ( other.size(), 1 );

    TESTLABS_CHECK_EQ( ordered_map[2], 6 );
    TESTLABS_CHECK_EQ( ordered_map[3], 7 );
    TESTLABS_CHECK_EQ( ordered_map[4], 8 );
    TESTLABS_CHECK_EQ( other[1], 5 );
}



TESTLABS_CASE( u8"CoreLabs:  ordered_map:  find / contains" )
{
    cl7::ordered_map<int, int> ordered_map{
        { 1, 5 },
        { 2, 6 },
        { 3, 7 },
        { 4, 8 },
    };

    TESTLABS_CHECK_EQ( ordered_map.size(), 4 );
    TESTLABS_CHECK( ordered_map.find(-2) == ordered_map.end() );
    TESTLABS_CHECK( ordered_map.find(2) == std::next(ordered_map.begin()) );
    TESTLABS_CHECK( !ordered_map.contains(-2) );
    TESTLABS_CHECK( ordered_map.contains(2) );
}



TESTLABS_CASE( u8"CoreLabs:  ordered_map:  clear" )
{
    cl7::ordered_map<int, int> ordered_map{
        { 1, 5 },
        { 2, 6 },
        { 3, 7 },
        { 4, 8 },
    };

    TESTLABS_CHECK_EQ( ordered_map.size(), 4 );
    ordered_map.clear();
    TESTLABS_CHECK( ordered_map.empty() );
}



TESTLABS_CASE( u8"CoreLabs:  ordered_map:  insert" )
{
    cl7::ordered_map<int, int> ordered_map;

    auto it = ordered_map.insert( 2, 6 );
    TESTLABS_CHECK( it.second );
    TESTLABS_CHECK_EQ( it.first->first, 2 );
    TESTLABS_CHECK_EQ( it.first->second, 6 );

    it = ordered_map.insert( { 3, 7 } );
    TESTLABS_CHECK( it.second );
    TESTLABS_CHECK_EQ( it.first->first, 3 );
    TESTLABS_CHECK_EQ( it.first->second, 7 );

    std::pair<int, int> pair{ 4, 8 };

    it = ordered_map.insert( pair );
    TESTLABS_CHECK( it.second );
    TESTLABS_CHECK_EQ( it.first->first, 4 );
    TESTLABS_CHECK_EQ( it.first->second, 8 );

    TESTLABS_CHECK_EQ( ordered_map.size(), 3 );

    TESTLABS_CHECK_EQ( ordered_map[2], 6 );
    TESTLABS_CHECK_EQ( ordered_map[3], 7 );
    TESTLABS_CHECK_EQ( ordered_map[4], 8 );

    it = ordered_map.insert( 2, 5 );
    TESTLABS_CHECK( !it.second );
    TESTLABS_CHECK_EQ( it.first->first, 2 );
    TESTLABS_CHECK_EQ( it.first->second, 5 );

    TESTLABS_CHECK_EQ( ordered_map.size(), 3 );

    TESTLABS_CHECK_EQ( ordered_map[2], 5 );
}



TESTLABS_CASE( u8"CoreLabs:  ordered_map:  emplace" )
{
    cl7::ordered_map<int, int> ordered_map;

    auto it = ordered_map.emplace( 2, 6 );
    TESTLABS_CHECK( it.second );
    TESTLABS_CHECK_EQ( it.first->first, 2 );
    TESTLABS_CHECK_EQ( it.first->second, 6 );

    it = ordered_map.emplace( std::pair<int, int>{ 3, 7 } );
    TESTLABS_CHECK( it.second );
    TESTLABS_CHECK_EQ( it.first->first, 3 );
    TESTLABS_CHECK_EQ( it.first->second, 7 );

    std::pair<int, int> pair{ 4, 8 };

    it = ordered_map.emplace( pair );
    TESTLABS_CHECK( it.second );
    TESTLABS_CHECK_EQ( it.first->first, 4 );
    TESTLABS_CHECK_EQ( it.first->second, 8 );

    TESTLABS_CHECK_EQ( ordered_map.size(), 3 );

    TESTLABS_CHECK_EQ( ordered_map[2], 6 );
    TESTLABS_CHECK_EQ( ordered_map[3], 7 );
    TESTLABS_CHECK_EQ( ordered_map[4], 8 );

    it = ordered_map.emplace( 2, 5 );
    TESTLABS_CHECK( !it.second );
    TESTLABS_CHECK_EQ( it.first->first, 2 );
    TESTLABS_CHECK_EQ( it.first->second, 5 );

    TESTLABS_CHECK_EQ( ordered_map.size(), 3 );

    TESTLABS_CHECK_EQ( ordered_map[2], 5 );
}



TESTLABS_CASE( u8"CoreLabs:  ordered_map:  erase" )
{
    cl7::ordered_map<int, int> ordered_map{
        { 1, 5 },
        { 2, 6 },
        { 3, 7 },
        { 4, 8 },
    };

    TESTLABS_CHECK_EQ( ordered_map.size(), 4 );

    auto it = ordered_map.erase( std::next(ordered_map.begin()) );
    TESTLABS_CHECK_EQ( ordered_map.size(), 3 );
    TESTLABS_CHECK_EQ( it->first, 3 );
    TESTLABS_CHECK_EQ( it->second, 7 );

    it = ordered_map.erase( ordered_map.cbegin() );
    TESTLABS_CHECK_EQ( ordered_map.size(), 2 );
    TESTLABS_CHECK_EQ( it->first, 3 );
    TESTLABS_CHECK_EQ( it->second, 7 );

    auto count = ordered_map.erase( 4 );
    TESTLABS_CHECK_EQ( ordered_map.size(), 1 );
    TESTLABS_CHECK_EQ( count, 1 );

    it = ordered_map.erase( ordered_map.end() );
    TESTLABS_CHECK_EQ( ordered_map.size(), 1 );
    TESTLABS_CHECK( it == ordered_map.end() );

    count = ordered_map.erase( -1 );
    TESTLABS_CHECK_EQ( ordered_map.size(), 1 );
    TESTLABS_CHECK_EQ( count, 0 );
}



TESTLABS_CASE( u8"CoreLabs:  ordered_map:  miscellaneous / string/struct" )
{
    struct item
    {
        int i;
        bool b;

        bool operator==(const item&) const noexcept = default;
    };

    cl7::ordered_map<cl7::u8string, item> ordered_map;
    cl7::ordered_map<cl7::u8string, item> other{
        { u8"0-false", { 0, false } },
        { u8"1-true", { 1, true } },
    };

    TESTLABS_CHECK( ordered_map != other );

    ordered_map[ u8"1-true" ] = { 1, true };
    ordered_map[ u8"0-false" ] = { 0, false };

    TESTLABS_CHECK( ordered_map == other );
    TESTLABS_CHECK_EQ( ordered_map.size(), 2 );
    TESTLABS_CHECK_EQ( other.size(), 2 );

    ordered_map.clear();
    ordered_map[ u8"0-false" ] = { 0, false };
    ordered_map[ u8"1-true" ] = { 1, true };

    TESTLABS_CHECK( ordered_map == other );
    TESTLABS_CHECK_EQ( ordered_map.size(), 2 );
    TESTLABS_CHECK_EQ( other.size(), 2 );

    ordered_map.insert( u8"foo", item{ 1, false } );
    other.emplace( u8"bar", item{ 2, false } );

    TESTLABS_CHECK( ordered_map != other );
    TESTLABS_CHECK_EQ( ordered_map.size(), 3 );
    TESTLABS_CHECK_EQ( other.size(), 3 );

    ordered_map.at( u8"foo" ) = { 1, false };
    ordered_map[ u8"bar" ] = { 2, true };
    other[ u8"foo" ] = { 1, false };
    other.at( u8"bar" ) = { 2, true };

    TESTLABS_CHECK( ordered_map == other );
    TESTLABS_CHECK_EQ( ordered_map.size(), 4 );
    TESTLABS_CHECK_EQ( other.size(), 4 );

    ordered_map.erase( u8"bar" );
    other.erase( u8"bar" );

    TESTLABS_CHECK( ordered_map == other );
    TESTLABS_CHECK_EQ( ordered_map.size(), 3 );
    TESTLABS_CHECK_EQ( other.size(), 3 );

    ordered_map.clear();

    TESTLABS_CHECK( ordered_map != other );
    TESTLABS_CHECK( ordered_map.empty() );

    ordered_map = other;

    TESTLABS_CHECK( ordered_map == other );
    TESTLABS_CHECK_EQ( ordered_map.size(), 3 );
    TESTLABS_CHECK_EQ( other.size(), 3 );

    TESTLABS_CHECK( ordered_map.contains( u8"0-false" ) );
    TESTLABS_CHECK( ordered_map.contains( u8"1-true" ) );
    TESTLABS_CHECK( ordered_map.contains( u8"foo" ) );

    TESTLABS_CHECK( other.contains( u8"0-false" ) );
    TESTLABS_CHECK( other.contains( u8"1-true" ) );
    TESTLABS_CHECK( other.contains( u8"foo" ) );

    ordered_map.clear();

    TESTLABS_CHECK( ordered_map != other );
    TESTLABS_CHECK( ordered_map.empty() );

    ordered_map = std::move(other);

    TESTLABS_CHECK_EQ( ordered_map.size(), 3 );

    TESTLABS_CHECK( ordered_map.contains( u8"0-false" ) );
    TESTLABS_CHECK( ordered_map.contains( u8"1-true" ) );
    TESTLABS_CHECK( ordered_map.contains( u8"foo" ) );
}



TESTLABS_CASE( u8"CoreLabs:  ordered_map:  miscellaneous / string/struct/string_view" )
{
    struct item
    {
        int i;
        bool b;

        bool operator==(const item&) const noexcept = default;
    };

    cl7::ordered_map<cl7::u8string, item, cl7::u8string_view> ordered_map;
    cl7::ordered_map<cl7::u8string, item, cl7::u8string_view> other{
        { u8"0-false", { 0, false } },
        { u8"1-true", { 1, true } },
    };

    TESTLABS_CHECK( ordered_map != other );

    ordered_map[ u8"1-true" ] = { 1, true };
    ordered_map[ u8"0-false" ] = { 0, false };

    TESTLABS_CHECK( ordered_map == other );
    TESTLABS_CHECK_EQ( ordered_map.size(), 2 );
    TESTLABS_CHECK_EQ( other.size(), 2 );

    ordered_map.clear();
    ordered_map[ u8"0-false" ] = { 0, false };
    ordered_map[ u8"1-true" ] = { 1, true };

    TESTLABS_CHECK( ordered_map == other );
    TESTLABS_CHECK_EQ( ordered_map.size(), 2 );
    TESTLABS_CHECK_EQ( other.size(), 2 );

    ordered_map.insert( u8"foo", item{ 1, false } );
    other.emplace( u8"bar", item{ 2, false } );

    TESTLABS_CHECK( ordered_map != other );
    TESTLABS_CHECK_EQ( ordered_map.size(), 3 );
    TESTLABS_CHECK_EQ( other.size(), 3 );

    ordered_map.at( u8"foo" ) = { 1, false };
    ordered_map[ u8"bar" ] = { 2, true };
    other[ u8"foo" ] = { 1, false };
    other.at( u8"bar" ) = { 2, true };

    TESTLABS_CHECK( ordered_map == other );
    TESTLABS_CHECK_EQ( ordered_map.size(), 4 );
    TESTLABS_CHECK_EQ( other.size(), 4 );

    ordered_map.erase( u8"bar" );
    other.erase( u8"bar" );

    TESTLABS_CHECK( ordered_map == other );
    TESTLABS_CHECK_EQ( ordered_map.size(), 3 );
    TESTLABS_CHECK_EQ( other.size(), 3 );

    ordered_map.clear();

    TESTLABS_CHECK( ordered_map != other );
    TESTLABS_CHECK( ordered_map.empty() );

    ordered_map = other;

    TESTLABS_CHECK( ordered_map == other );
    TESTLABS_CHECK_EQ( ordered_map.size(), 3 );
    TESTLABS_CHECK_EQ( other.size(), 3 );

    TESTLABS_CHECK( ordered_map.contains( u8"0-false" ) );
    TESTLABS_CHECK( ordered_map.contains( u8"1-true" ) );
    TESTLABS_CHECK( ordered_map.contains( u8"foo" ) );

    TESTLABS_CHECK( other.contains( u8"0-false" ) );
    TESTLABS_CHECK( other.contains( u8"1-true" ) );
    TESTLABS_CHECK( other.contains( u8"foo" ) );

    ordered_map.clear();

    TESTLABS_CHECK( ordered_map != other );
    TESTLABS_CHECK( ordered_map.empty() );

    ordered_map = std::move(other);

    TESTLABS_CHECK_EQ( ordered_map.size(), 3 );

    TESTLABS_CHECK( ordered_map.contains( u8"0-false" ) );
    TESTLABS_CHECK( ordered_map.contains( u8"1-true" ) );
    TESTLABS_CHECK( ordered_map.contains( u8"foo" ) );
}



#endif // F77_TESTS_CL7_ORDEREDMAP_H

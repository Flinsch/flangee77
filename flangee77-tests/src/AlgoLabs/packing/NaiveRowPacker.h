#ifndef F77_TESTS_AL7_PACKING_NAIVEROWPACKER_H
#define F77_TESTS_AL7_PACKING_NAIVEROWPACKER_H

#include <AlgoLabs/packing/NaiveRowPacker.h>

#include <TestLabs/TestSuite.h>

#include "../../shared.h"



TESTLABS_CASE( u8"AlgoLabs:  packing:  NaiveRowPacker:  insert a single item at (0,0)" )
{
    al7::packing::NaiveRowPacker packer( { 100, 100 } );

    TESTLABS_ASSERT( packer.can_insert( { 20, 10 } ) );

    auto r = packer.insert( { 20, 10 } );

    TESTLABS_ASSERT( r.has_value() );

    TESTLABS_CHECK_EQ( r->position.x, 0 );
    TESTLABS_CHECK_EQ( r->position.y, 0 );
    TESTLABS_CHECK_EQ( r->size.width, 20 );
    TESTLABS_CHECK_EQ( r->size.height, 10 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  NaiveRowPacker:  fail if item too big" )
{
    al7::packing::NaiveRowPacker packer( { 32, 32 } );

    TESTLABS_ASSERT( !packer.can_insert( { 64, 64 } ) );

    auto r = packer.insert( { 64, 64 } );

    TESTLABS_CHECK( !r.has_value() );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  NaiveRowPacker:  insert multiple items without overlap" )
{
    al7::packing::NaiveRowPacker packer( { 64, 64 } );

    auto a = packer.insert( { 32, 32 } );
    auto b = packer.insert( { 32, 32 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_CHECK( !a->overlaps( *b ) );
    TESTLABS_CHECK( !b->overlaps( *a ) );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  NaiveRowPacker:  use all available space" )
{
    al7::packing::NaiveRowPacker packer( { 1, 2 } );

    auto a = packer.insert( { 1, 1 } );
    auto b = packer.insert( { 1, 1 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_CHECK_EQ( packer.used_area(), packer.bin_size().area() );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  NaiveRowPacker:  allow flipping" )
{
    al7::packing::NaiveRowPacker packer( { 30, 15 } );

    TESTLABS_ASSERT( packer.can_insert( { 10, 20 }, { .allow_flip = true } ) );

    auto r = packer.insert( { 10, 20 }, { .allow_flip = true } );

    TESTLABS_ASSERT( r.has_value() );
    TESTLABS_CHECK_EQ( r->size.width, 20 );
    TESTLABS_CHECK_EQ( r->size.height, 10 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  NaiveRowPacker:  disallow flipping should fail" )
{
    al7::packing::NaiveRowPacker packer( { 30, 15 } );

    TESTLABS_ASSERT( !packer.can_insert( { 10, 20 }, { .allow_flip = false } ) );

    auto r = packer.insert( { 10, 20 }, { .allow_flip = false } );

    TESTLABS_CHECK( !r.has_value() );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  NaiveRowPacker:  advance horizontally within the same row" )
{
    al7::packing::NaiveRowPacker packer( { 100, 100 } );

    auto a = packer.insert( { 30, 10 } );
    auto b = packer.insert( { 20, 10 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );

    TESTLABS_CHECK_EQ( a->position.x, 0 );
    TESTLABS_CHECK_EQ( a->position.y, 0 );
    TESTLABS_CHECK_EQ( a->size.width, 30 );
    TESTLABS_CHECK_EQ( a->size.height, 10 );

    TESTLABS_CHECK_EQ( b->position.x, 30 );
    TESTLABS_CHECK_EQ( b->position.y, 0 );
    TESTLABS_CHECK_EQ( b->size.width, 20 );
    TESTLABS_CHECK_EQ( b->size.height, 10 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  NaiveRowPacker:  start new row when item no longer fits horizontally" )
{
    al7::packing::NaiveRowPacker packer( { 50, 100 } );

    auto a = packer.insert( { 30, 10 } );
    auto b = packer.insert( { 25, 12 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );

    TESTLABS_CHECK_EQ( a->position.x, 0 );
    TESTLABS_CHECK_EQ( a->position.y, 0 );
    TESTLABS_CHECK_EQ( a->size.width, 30 );
    TESTLABS_CHECK_EQ( a->size.height, 10 );

    TESTLABS_CHECK_EQ( b->position.x, 0 );
    TESTLABS_CHECK_EQ( b->position.y, 10 );
    TESTLABS_CHECK_EQ( b->size.width, 25 );
    TESTLABS_CHECK_EQ( b->size.height, 12 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  NaiveRowPacker:  base row height on tallest item in the row" )
{
    al7::packing::NaiveRowPacker packer( { 50, 200 } );

    auto a = packer.insert( { 20, 10 } );
    auto b = packer.insert( { 20, 30 } );
    auto c = packer.insert( { 20, 5 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_ASSERT( c.has_value() );

    TESTLABS_CHECK_EQ( c->position.y, 30 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  NaiveRowPacker:  flip if allowed and beneficial" )
{
    al7::packing::NaiveRowPacker packer( { 40, 40 } );

    auto a = packer.insert( { 20, 25 }, { .allow_flip = false } );
    auto b = packer.insert( { 30, 10 }, { .allow_flip = true } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );

    TESTLABS_CHECK_EQ( b->position.x, 20 );
    TESTLABS_CHECK_EQ( b->position.y, 0 );
    TESTLABS_CHECK_EQ( b->size.width, 10 );
    TESTLABS_CHECK_EQ( b->size.height, 30 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  NaiveRowPacker:  don't flip if unnecessary" )
{
    al7::packing::NaiveRowPacker packer( { 50, 40 } );

    auto a = packer.insert( { 20, 25 }, { .allow_flip = false } );
    auto b = packer.insert( { 30, 10 }, { .allow_flip = true } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );

    TESTLABS_CHECK_EQ( b->position.x, 20 );
    TESTLABS_CHECK_EQ( b->position.y, 0 );
    TESTLABS_CHECK_EQ( b->size.width, 30 );
    TESTLABS_CHECK_EQ( b->size.height, 10 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  NaiveRowPacker:  fail when item (even flipped) cannot fit vertically" )
{
    al7::packing::NaiveRowPacker packer( { 40, 40 } );

    auto a = packer.insert( { 40, 20 } );
    auto b = packer.insert( { 40, 20 } );
    auto c = packer.insert( { 10, 10 }, { .allow_flip = true } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_ASSERT( !c.has_value() );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  NaiveRowPacker:  accumulate vertical position correctly over multiple rows" )
{
    al7::packing::NaiveRowPacker packer( { 30, 100 } );

    auto a = packer.insert( { 30, 10 } );
    auto b = packer.insert( { 30, 15 } );
    auto c = packer.insert( { 10, 5 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_ASSERT( c.has_value() );

    TESTLABS_CHECK_EQ( c->position.x, 0 );
    TESTLABS_CHECK_EQ( c->position.y, 25 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  NaiveRowPacker:  don't return to previous rows" )
{
    al7::packing::NaiveRowPacker packer( { 50, 100 } );

    auto a = packer.insert( { 25, 10 } );
    auto b = packer.insert( { 30, 15 } );
    auto c = packer.insert( { 10, 5 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_ASSERT( c.has_value() );

    TESTLABS_CHECK_EQ( c->position.x, 30 );
    TESTLABS_CHECK_EQ( c->position.y, 10 );
}



#endif // F77_TESTS_AL7_PACKING_NAIVEROWPACKER_H

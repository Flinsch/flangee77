
#include <TestLabs/TestSuite.h>

#include <AlgoLabs/packing/MaxRectsPacker.h>

#include "../../shared.h"



TESTLABS_CASE( u8"AlgoLabs:  packing:  MaxRectsPacker:  insert a single item at (0,0)" )
{
    al7::packing::MaxRectsPacker packer( { 100, 100 } );

    TESTLABS_ASSERT( packer.can_insert( { 20, 10 } ) );

    auto r = packer.insert( { 20, 10 } );

    TESTLABS_ASSERT( r.has_value() );

    TESTLABS_CHECK_EQ( r->position.x, 0 );
    TESTLABS_CHECK_EQ( r->position.y, 0 );
    TESTLABS_CHECK_EQ( r->size.width, 20 );
    TESTLABS_CHECK_EQ( r->size.height, 10 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  MaxRectsPacker:  fail if item too big" )
{
    al7::packing::MaxRectsPacker packer( { 32, 32 } );

    TESTLABS_ASSERT( !packer.can_insert( { 64, 64 } ) );

    auto r = packer.insert( { 64, 64 } );

    TESTLABS_CHECK( !r.has_value() );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  MaxRectsPacker:  insert multiple items without overlap" )
{
    al7::packing::MaxRectsPacker packer( { 64, 64 } );

    auto a = packer.insert( { 32, 32 } );
    auto b = packer.insert( { 32, 32 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_CHECK( !a->overlaps( *b ) );
    TESTLABS_CHECK( !b->overlaps( *a ) );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  MaxRectsPacker:  use all available space" )
{
    al7::packing::MaxRectsPacker packer( { 1, 2 } );

    auto a = packer.insert( { 1, 1 } );
    auto b = packer.insert( { 1, 1 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_CHECK_EQ( packer.used_area(), packer.bin_size().area() );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  MaxRectsPacker:  allow flipping" )
{
    al7::packing::MaxRectsPacker packer( { 30, 15 } );

    TESTLABS_ASSERT( packer.can_insert( { 10, 20 }, { .allow_flip = true } ) );

    auto r = packer.insert( { 10, 20 }, { .allow_flip = true } );

    TESTLABS_ASSERT( r.has_value() );
    TESTLABS_CHECK_EQ( r->size.width, 20 );
    TESTLABS_CHECK_EQ( r->size.height, 10 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  MaxRectsPacker:  disallow flipping should fail" )
{
    al7::packing::MaxRectsPacker packer( { 30, 15 } );

    TESTLABS_ASSERT( !packer.can_insert( { 10, 20 }, { .allow_flip = false } ) );

    auto r = packer.insert( { 10, 20 }, { .allow_flip = false } );

    TESTLABS_CHECK( !r.has_value() );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  MaxRectsPacker:  don't flip if not necessary" )
{
    al7::packing::MaxRectsPacker packer( { 30, 30 } );

    auto r = packer.insert( { 10, 20 }, { .allow_flip = true } );

    TESTLABS_ASSERT( r.has_value() );
    TESTLABS_CHECK_EQ( r->size.width, 10 );
    TESTLABS_CHECK_EQ( r->size.height, 20 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  MaxRectsPacker:  fill space efficiently" )
{
    al7::packing::MaxRectsPacker packer( { 64, 64 } );

    auto r = packer.insert( std::vector<al7::packing::Size>( { { 30, 10 }, { 20, 20 }, { 15, 25 }, { 10, 30 } } ) );

    TESTLABS_ASSERT_EQ( r.size(), 4 );
    TESTLABS_ASSERT( r[0].has_value() );
    TESTLABS_ASSERT( r[1].has_value() );
    TESTLABS_ASSERT( r[2].has_value() );
    TESTLABS_ASSERT( r[3].has_value() );
    for (const auto& r1 : r)
        for (const auto& r2 : r)
            TESTLABS_CHECK_EQ( r1->overlaps( *r2 ), &*r1 == &*r2 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  MaxRectsPacker:  choose smaller short-side leftover with BSSF" )
{
    al7::packing::MaxRectsPacker packer( { 16, 10 }, al7::packing::MaxRectsPacker::Heuristic::BestShortSideFit );

    auto a = packer.insert( { 10, 5 } );
    auto b = packer.insert( { 4, 4 } );
    auto c = packer.insert( { 5, 5 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_ASSERT( c.has_value() );
    TESTLABS_CHECK_EQ( c->position.x, 4 );
    TESTLABS_CHECK_EQ( c->position.y, 5 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  MaxRectsPacker:  choose smaller leftover area with BAF" )
{
    al7::packing::MaxRectsPacker packer( { 16, 10 }, al7::packing::MaxRectsPacker::Heuristic::BestAreaFit );

    auto a = packer.insert( { 10, 5 } );
    auto b = packer.insert( { 4, 4 } );
    auto c = packer.insert( { 5, 5 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_ASSERT( c.has_value() );
    TESTLABS_CHECK_EQ( c->position.x, 10 );
    TESTLABS_CHECK_EQ( c->position.y, 4 );
}

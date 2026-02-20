
#include <TestLabs/TestSuite.h>

#include <AlgoLabs/packing/SkylinePacker.h>

#include "../../shared.h"



TESTLABS_CASE( u8"AlgoLabs:  packing:  SkylinePacker:  insert a single item at (0,0)" )
{
    al7::packing::SkylinePacker packer( { 100, 100 } );

    TESTLABS_ASSERT( packer.can_insert( { 20, 10 } ) );

    auto r = packer.insert( { 20, 10 } );

    TESTLABS_ASSERT( r.has_value() );

    TESTLABS_CHECK_EQ( r->position.x, 0 );
    TESTLABS_CHECK_EQ( r->position.y, 0 );
    TESTLABS_CHECK_EQ( r->size.width, 20 );
    TESTLABS_CHECK_EQ( r->size.height, 10 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  SkylinePacker:  fail if item too big" )
{
    al7::packing::SkylinePacker packer( { 32, 32 } );

    TESTLABS_ASSERT( !packer.can_insert( { 64, 64 } ) );

    auto r = packer.insert( { 64, 64 } );

    TESTLABS_CHECK( !r.has_value() );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  SkylinePacker:  insert multiple items without overlap" )
{
    al7::packing::SkylinePacker packer( { 64, 64 } );

    auto a = packer.insert( { 32, 32 } );
    auto b = packer.insert( { 32, 32 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_CHECK( !a->overlaps( *b ) );
    TESTLABS_CHECK( !b->overlaps( *a ) );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  SkylinePacker:  use all available space" )
{
    al7::packing::SkylinePacker packer( { 1, 2 } );

    auto a = packer.insert( { 1, 1 } );
    auto b = packer.insert( { 1, 1 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_CHECK_EQ( packer.used_area(), packer.bin_size().area() );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  SkylinePacker:  allow flipping" )
{
    al7::packing::SkylinePacker packer( { 30, 15 } );

    TESTLABS_ASSERT( packer.can_insert( { 10, 20 }, { .allow_flip = true } ) );

    auto r = packer.insert( { 10, 20 }, { .allow_flip = true } );

    TESTLABS_ASSERT( r.has_value() );
    TESTLABS_CHECK_EQ( r->size.width, 20 );
    TESTLABS_CHECK_EQ( r->size.height, 10 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  SkylinePacker:  disallow flipping should fail" )
{
    al7::packing::SkylinePacker packer( { 30, 15 } );

    TESTLABS_ASSERT( !packer.can_insert( { 10, 20 }, { .allow_flip = false } ) );

    auto r = packer.insert( { 10, 20 }, { .allow_flip = false } );

    TESTLABS_CHECK( !r.has_value() );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  SkylinePacker:  don't flip if not necessary" )
{
    al7::packing::SkylinePacker packer( { 30, 30 } );

    auto r = packer.insert( { 20, 10 }, { .allow_flip = true } );

    TESTLABS_ASSERT( r.has_value() );
    TESTLABS_CHECK_EQ( r->size.width, 20 );
    TESTLABS_CHECK_EQ( r->size.height, 10 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  SkylinePacker:  do flip if not necessary, but preferred (flatter item)" )
{
    al7::packing::SkylinePacker packer( { 30, 30 } );

    auto r = packer.insert( { 10, 20 }, { .allow_flip = true } );

    TESTLABS_ASSERT( r.has_value() );
    TESTLABS_CHECK_EQ( r->size.width, 20 );
    TESTLABS_CHECK_EQ( r->size.height, 10 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  SkylinePacker:  do flip if not necessary, but preferred (less waste)" )
{
    al7::packing::SkylinePacker packer( { 30, 30 } );

    auto a = packer.insert( { 15, 5 }, { .allow_flip = true } );
    auto b = packer.insert( { 20, 10 }, { .allow_flip = true } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_CHECK_EQ( b->size.width, 10 );
    TESTLABS_CHECK_EQ( b->size.height, 20 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  SkylinePacker:  fill space efficiently" )
{
    al7::packing::SkylinePacker packer( { 64, 64 } );

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

TESTLABS_CASE( u8"AlgoLabs:  packing:  SkylinePacker:  choose lower position with BL" )
{
    al7::packing::SkylinePacker packer( { 10, 10 }, al7::packing::SkylinePacker::Heuristic::BottomLeft );

    auto a = packer.insert( { 5, 3 } );
    auto b = packer.insert( { 3, 2 } );
    auto c = packer.insert( { 5, 2 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_ASSERT( c.has_value() );
    TESTLABS_CHECK_EQ( c->position.x, 5 );
    TESTLABS_CHECK_EQ( c->position.y, 2 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  SkylinePacker:  choose smaller waste area with BF" )
{
    al7::packing::SkylinePacker packer( { 10, 10 }, al7::packing::SkylinePacker::Heuristic::BestFit );

    auto a = packer.insert( { 5, 3 } );
    auto b = packer.insert( { 3, 2 } );
    auto c = packer.insert( { 5, 2 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_ASSERT( c.has_value() );
    TESTLABS_CHECK_EQ( c->position.x, 0 );
    TESTLABS_CHECK_EQ( c->position.y, 3 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  SkylinePacker:  choose smaller waste area with BF (left-aligned)" )
{
    al7::packing::SkylinePacker packer( { 10, 10 }, al7::packing::SkylinePacker::Heuristic::BottomLeft );

    auto a = packer.insert( { 1, 3 } );
    auto b = packer.insert( { 3, 2 } );
    auto c = packer.insert( { 2, 3 } );
    auto d = packer.insert( { 4, 1 } );
    packer.set_heuristic( al7::packing::SkylinePacker::Heuristic::BestFit );
    auto e = packer.insert( { 7, 2 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_ASSERT( c.has_value() );
    TESTLABS_ASSERT( d.has_value() );
    TESTLABS_ASSERT( e.has_value() );
    TESTLABS_CHECK_EQ( e->position.x, 0 );
    TESTLABS_CHECK_EQ( e->position.y, 3 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  SkylinePacker:  choose smaller waste area with BF (right-aligned)" )
{
    al7::packing::SkylinePacker packer( { 10, 10 }, al7::packing::SkylinePacker::Heuristic::BottomLeft );

    auto a = packer.insert( { 1, 3 } );
    auto b = packer.insert( { 3, 1 } );
    auto c = packer.insert( { 2, 3 } );
    auto d = packer.insert( { 4, 2 } );
    packer.set_heuristic( al7::packing::SkylinePacker::Heuristic::BestFit );
    auto e = packer.insert( { 7, 2 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_ASSERT( c.has_value() );
    TESTLABS_ASSERT( d.has_value() );
    TESTLABS_ASSERT( e.has_value() );
    TESTLABS_CHECK_EQ( e->position.x, 3 );
    TESTLABS_CHECK_EQ( e->position.y, 3 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  SkylinePacker:  refuse item due to wasted space" )
{
    al7::packing::SkylinePacker packer( { 10, 10 }, al7::packing::SkylinePacker::Heuristic::BestFit );

    auto a = packer.insert( { 5, 5 } );
    auto b = packer.insert( { 10, 5 } );
    TESTLABS_ASSERT( !packer.can_insert( { 5, 5 } ) );
    auto c = packer.insert( { 5, 5 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_ASSERT( !c.has_value() );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  SkylinePacker:  accept item that fits into waste map" )
{
    al7::packing::SkylinePacker packer( { 10, 10 }, al7::packing::SkylinePacker::Heuristic::BestFit, true );

    auto a = packer.insert( { 5, 5 } );
    auto b = packer.insert( { 10, 5 } );
    TESTLABS_ASSERT( packer.can_insert( { 5, 5 } ) );
    auto c = packer.insert( { 5, 5 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_ASSERT( c.has_value() );
}

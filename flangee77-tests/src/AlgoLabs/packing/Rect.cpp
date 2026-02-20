
#include <TestLabs/TestSuite.h>

#include <AlgoLabs/packing/Rect.h>

#include "../../shared.h"



TESTLABS_CASE( u8"AlgoLabs:  packing:  Size::covers" )
{
    al7::packing::Size a, b;

    // Identical sizes
    a = { 10, 10 };
    TESTLABS_CHECK_EQ( a.covers( a ), true );

    // Big enough
    a = { 10, 10 };
    b = { 5, 5 };
    TESTLABS_CHECK_EQ( a.covers( b ), true );
    TESTLABS_CHECK_EQ( b.covers( a ), false );

    // Too small horizontally
    a = { 4, 10 };
    b = { 5, 5 };
    TESTLABS_CHECK_EQ( a.covers( b ), false );
    TESTLABS_CHECK_EQ( b.covers( a ), false );

    // Too small vertically
    a = { 10, 4 };
    b = { 5, 5 };
    TESTLABS_CHECK_EQ( a.covers( b ), false );
    TESTLABS_CHECK_EQ( b.covers( a ), false );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  Size::flipped" )
{
    al7::packing::Size a, b;

    a = { 5, 7 };
    b = { 7, 5 };
    TESTLABS_CHECK_EQ( a.flipped().width, b.width );
    TESTLABS_CHECK_EQ( a.flipped().height, b.height );
    TESTLABS_CHECK_EQ( b.flipped().width, a.width );
    TESTLABS_CHECK_EQ( b.flipped().height, a.height );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  Size::area" )
{
    al7::packing::Size a, b;

    a = { 0, 0 };
    b = { 5, 7 };
    TESTLABS_CHECK_EQ( a.area(), 0 );
    TESTLABS_CHECK_EQ( b.area(), 35 );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  Rect::overlaps" )
{
    al7::packing::Rect a, b;

    // Identical rectangles
    a = { { 0, 0 }, { 10, 10 } };
    TESTLABS_CHECK_EQ( a.overlaps( a ), true );

    // Basic overlapping
    a = { { 0, 0 }, { 10, 10 } };
    b = { { 5, 5 }, { 10, 10 } } ;
    TESTLABS_CHECK_EQ( a.overlaps( b ), true );
    TESTLABS_CHECK_EQ( b.overlaps( a ), true );

    // Touching edges is NOT overlap
    a = { { 0, 0 }, { 10, 10 } };
    b = { { 10, 0 }, { 10, 10 } };
    TESTLABS_CHECK_EQ( a.overlaps( b ), false );
    TESTLABS_CHECK_EQ( b.overlaps( a ), false );

    // One completely inside another
    a = { { 0, 0 }, { 20, 20 } };
    b = { { 5, 5 }, { 5, 5 } };
    TESTLABS_CHECK_EQ( a.overlaps( b ), true );
    TESTLABS_CHECK_EQ( b.overlaps( a ), true );

    // No overlap at all
    a = { { 0, 0 }, { 5, 5 } } ;
    b = { { 100, 100 }, { 10, 10 } };
    TESTLABS_CHECK_EQ( a.overlaps( b ), false );
    TESTLABS_CHECK_EQ( b.overlaps( a ), false );

    // Overlapping only in one dimension (horizontal)
    a = { { 0, 0 }, { 10, 10 } };
    b = { { 5, 20 }, { 10, 10 } };
    TESTLABS_CHECK_EQ( a.overlaps( b ), false );
    TESTLABS_CHECK_EQ( b.overlaps( a ), false );

    // Overlapping only in one dimension (vertical)
    a = { { 0, 0 }, { 10, 10 } };
    b = { { 20, 5 }, { 10, 10 } };
    TESTLABS_CHECK_EQ( a.overlaps( b ), false );
    TESTLABS_CHECK_EQ( b.overlaps( a ), false );
}

TESTLABS_CASE( u8"AlgoLabs:  packing:  Rect::contains" )
{
    al7::packing::Rect a, b;

    // Identical rectangles
    a = { { 0, 0 }, { 10, 10 } };
    TESTLABS_CHECK_EQ( a.contains( a ), true );

    // Fully contains
    a = { { 0, 0 }, { 20, 20 } };
    b = { { 5, 5 }, { 10, 10 } };
    TESTLABS_CHECK_EQ( a.contains( b ), true );
    TESTLABS_CHECK_EQ( b.contains( a ), false );

    // Touching edges is NOT contain
    a = { { 0, 0 }, { 10, 10 } };
    b = { { 10, 0 }, { 5, 5 } };
    TESTLABS_CHECK_EQ( a.contains( b ), false );

    // Outside entirely
    a = { { 0, 0 }, { 10, 10 } };
    b = { { 20, 20 }, { 5, 5 } };
    TESTLABS_CHECK_EQ( a.contains( b ), false );

    // Partially outside
    a = { { 0, 0 }, { 10, 10 } };
    b = { { 5, 5 }, { 10, 10 } };
    TESTLABS_CHECK_EQ( a.contains( b ), false );
}

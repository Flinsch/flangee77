#ifndef F77_TESTS_AL7_PACKING_ABSTRACTRECTPACKER_H
#define F77_TESTS_AL7_PACKING_ABSTRACTRECTPACKER_H

#include <AlgoLabs/packing/AbstractRectPacker.h>

#include <TestLabs/TestSuite.h>

#include "../../shared.h"



TESTLABS_CASE( u8"AlgoLabs:  packing:  AbstractRectPacker" )
{
    class MyRectPacker
        : public al7::packing::AbstractRectPacker
    {
    public:
        using AbstractRectPacker::AbstractRectPacker;
    protected:
        void _on_reset(al7::packing::Size bin_size) override
        {
            _pos = 0;
        }
        bool _can_insert(al7::packing::Size padded_size, al7::packing::RectOptions options) const override
        {
            return _pos + padded_size.width <= bin_size().width && padded_size.height <= bin_size().height;
        }
        std::optional<al7::packing::Rect> _insert(al7::packing::Size padded_size, al7::packing::RectOptions options) override
        {
            if (!_can_insert(padded_size, options))
                return {};
            al7::packing::Rect rect{_pos, 0, padded_size};
            _pos += padded_size.width;
            return rect;
        }
    private:
        unsigned _pos = 0;
    };

    MyRectPacker packer( { 20, 10 } );

    TESTLABS_CHECK_EQ( packer.bin_size().width, 20 );
    TESTLABS_CHECK_EQ( packer.bin_size().height, 10 );
    TESTLABS_CHECK_EQ( packer.item_count(), 0 );
    TESTLABS_CHECK_EQ( packer.used_area(), 0 );
    TESTLABS_CHECK_EQ( packer.occupancy(), 0.0f );

    TESTLABS_CHECK_EQ( packer.can_insert( { 20, 10 } ), true );
    TESTLABS_CHECK_EQ( packer.can_insert( { 21, 10 } ), false );
    TESTLABS_CHECK_EQ( packer.can_insert( { 20, 11 } ), false );

    auto a = packer.insert( { 3, 3 } );
    auto b = packer.insert( { 5, 5 } );
    auto c = packer.insert( { 7, 7 } );

    TESTLABS_ASSERT( a.has_value() );
    TESTLABS_ASSERT( b.has_value() );
    TESTLABS_ASSERT( c.has_value() );

    TESTLABS_CHECK_EQ( c->position.x, 8 );
    TESTLABS_CHECK_EQ( c->position.y, 0 );
    TESTLABS_CHECK_EQ( c->size.width, 7 );
    TESTLABS_CHECK_EQ( c->size.height, 7 );

    TESTLABS_CHECK_EQ( packer.bin_size().width, 20 );
    TESTLABS_CHECK_EQ( packer.bin_size().height, 10 );
    TESTLABS_CHECK_EQ( packer.item_count(), 3 );
    TESTLABS_CHECK_EQ( packer.used_area(), 83 );
    TESTLABS_CHECK_EQ_ROUND( 3, packer.occupancy(), 0.415f );

    packer.reset();

    TESTLABS_CHECK_EQ( packer.bin_size().width, 20 );
    TESTLABS_CHECK_EQ( packer.bin_size().height, 10 );
    TESTLABS_CHECK_EQ( packer.item_count(), 0 );
    TESTLABS_CHECK_EQ( packer.used_area(), 0 );
    TESTLABS_CHECK_EQ( packer.occupancy(), 0.0f );

    auto abc = packer.insert( std::vector<al7::packing::Size>{ { 3, 3 }, { 5, 5 }, { 7, 7 } } );

    TESTLABS_ASSERT_EQ( abc.size(), 3 );
    TESTLABS_ASSERT( abc[0].has_value() );
    TESTLABS_ASSERT( abc[1].has_value() );
    TESTLABS_ASSERT( abc[2].has_value() );

    TESTLABS_CHECK_EQ( abc[2]->position.x, 8 );
    TESTLABS_CHECK_EQ( abc[2]->position.y, 0 );
    TESTLABS_CHECK_EQ( abc[2]->size.width, 7 );
    TESTLABS_CHECK_EQ( abc[2]->size.height, 7 );

    TESTLABS_CHECK_EQ( packer.bin_size().width, 20 );
    TESTLABS_CHECK_EQ( packer.bin_size().height, 10 );
    TESTLABS_CHECK_EQ( packer.item_count(), 3 );
    TESTLABS_CHECK_EQ( packer.used_area(), 83 );
    TESTLABS_CHECK_EQ_ROUND( 3, packer.occupancy(), 0.415f );

    packer.reset( { 31, 10 } );

    TESTLABS_CHECK_EQ( packer.bin_size().width, 31 );
    TESTLABS_CHECK_EQ( packer.bin_size().height, 10 );
    TESTLABS_CHECK_EQ( packer.item_count(), 0 );
    TESTLABS_CHECK_EQ( packer.used_area(), 0 );
    TESTLABS_CHECK_EQ( packer.occupancy(), 0.0f );

    abc = packer.insert( std::vector<al7::packing::Size>{ { 3, 3 }, { 5, 5 }, { 7, 7 } }, {.padding = 1} );

    TESTLABS_ASSERT_EQ( abc.size(), 3 );
    TESTLABS_ASSERT( abc[0].has_value() );
    TESTLABS_ASSERT( abc[1].has_value() );
    TESTLABS_ASSERT( abc[2].has_value() );

    TESTLABS_CHECK_EQ( abc[2]->position.x, 13 );
    TESTLABS_CHECK_EQ( abc[2]->position.y, 1 );
    TESTLABS_CHECK_EQ( abc[2]->size.width, 7 );
    TESTLABS_CHECK_EQ( abc[2]->size.height, 7 );

    TESTLABS_CHECK_EQ( packer.bin_size().width, 31 );
    TESTLABS_CHECK_EQ( packer.bin_size().height, 10 );
    TESTLABS_CHECK_EQ( packer.item_count(), 3 );
    TESTLABS_CHECK_EQ( packer.used_area(), 155 );
    TESTLABS_CHECK_EQ_ROUND( 3, packer.occupancy(), 0.5f );
}



#endif // F77_TESTS_AL7_PACKING_ABSTRACTRECTPACKER_H

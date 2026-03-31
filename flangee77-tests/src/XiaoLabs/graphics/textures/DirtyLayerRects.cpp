
#include <TestLabs/TestSuite.h>

#include <XiaoLabs/graphics/textures/DirtyLayerRects.h>

#include "../../shared.h"



TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerRects:  new object, not dirty" )
{
    xl7::graphics::textures::DirtyLayerRects dirty_layer_rects;
    TESTLABS_CHECK( !dirty_layer_rects.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_rects.is_all_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerRects:  init empty, not dirty" )
{
    xl7::graphics::textures::DirtyLayerRects dirty_layer_rects;
    dirty_layer_rects.update( 0, 0, 0, 0, 0 );
    TESTLABS_CHECK( !dirty_layer_rects.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_rects.is_all_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerRects:  init empty width, not dirty" )
{
    xl7::graphics::textures::DirtyLayerRects dirty_layer_rects;
    dirty_layer_rects.update( 0, 0, 0, 0, 1 );
    TESTLABS_CHECK( !dirty_layer_rects.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_rects.is_all_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerRects:  init empty height, not dirty" )
{
    xl7::graphics::textures::DirtyLayerRects dirty_layer_rects;
    dirty_layer_rects.update( 0, 0, 0, 1, 0 );
    TESTLABS_CHECK( !dirty_layer_rects.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_rects.is_all_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerRects:  init non-empty, is dirty" )
{
    xl7::graphics::textures::DirtyLayerRects dirty_layer_rects;
    dirty_layer_rects.update( 0, 0, 0, 1, 1 );
    TESTLABS_CHECK( dirty_layer_rects.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_rects.is_all_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerRects:  init non-empty, update empty, still dirty" )
{
    xl7::graphics::textures::DirtyLayerRects dirty_layer_rects;
    dirty_layer_rects.update( 0, 0, 0, 1, 1 );
    dirty_layer_rects.update( 0, 0, 0, 0, 0 );
    TESTLABS_CHECK( dirty_layer_rects.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_rects.is_all_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerRects:  init non-empty, clear, not dirty" )
{
    xl7::graphics::textures::DirtyLayerRects dirty_layer_rects;
    dirty_layer_rects.update( 0, 0, 0, 1, 1 );
    dirty_layer_rects.clear();
    TESTLABS_CHECK( !dirty_layer_rects.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_rects.is_all_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerRects:  update without effect" )
{
    xl7::graphics::textures::DirtyLayerRects dirty_layer_rects;
    dirty_layer_rects.update( 0, 20, 20, 20, 20 );
    dirty_layer_rects.update( 0, 25, 25, 10, 10 );
    TESTLABS_CHECK( dirty_layer_rects.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_rects.is_all_dirty() );
    TESTLABS_ASSERT_EQ( dirty_layer_rects.layer_rects().size(), 1 );
    TESTLABS_CHECK( dirty_layer_rects.layer_rects()[0].is_dirty() );
    TESTLABS_CHECK_EQ( dirty_layer_rects.layer_rects()[0].x(), 20 );
    TESTLABS_CHECK_EQ( dirty_layer_rects.layer_rects()[0].y(), 20 );
    TESTLABS_CHECK_EQ( dirty_layer_rects.layer_rects()[0].width(), 20 );
    TESTLABS_CHECK_EQ( dirty_layer_rects.layer_rects()[0].height(), 20 );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerRects:  update with expansion" )
{
    xl7::graphics::textures::DirtyLayerRects dirty_layer_rects;
    dirty_layer_rects.update( 0, 20, 20, 20, 20 );
    dirty_layer_rects.update( 0, 5, 10, 10, 20 );
    TESTLABS_CHECK( dirty_layer_rects.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_rects.is_all_dirty() );
    TESTLABS_ASSERT_EQ( dirty_layer_rects.layer_rects().size(), 1 );
    TESTLABS_CHECK( dirty_layer_rects.layer_rects()[0].is_dirty() );
    TESTLABS_CHECK_EQ( dirty_layer_rects.layer_rects()[0].x(), 5 );
    TESTLABS_CHECK_EQ( dirty_layer_rects.layer_rects()[0].y(), 10 );
    TESTLABS_CHECK_EQ( dirty_layer_rects.layer_rects()[0].width(), 35 );
    TESTLABS_CHECK_EQ( dirty_layer_rects.layer_rects()[0].height(), 30 );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerRects:  update another layer" )
{
    xl7::graphics::textures::DirtyLayerRects dirty_layer_rects;
    dirty_layer_rects.update( 0, 20, 20, 20, 20 );
    dirty_layer_rects.update( 2, 25, 25, 10, 10 );
    dirty_layer_rects.update( 2, 5, 10, 10, 20 );
    TESTLABS_CHECK( dirty_layer_rects.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_rects.is_all_dirty() );
    TESTLABS_ASSERT_EQ( dirty_layer_rects.layer_rects().size(), 3 );
    TESTLABS_CHECK( dirty_layer_rects.layer_rects()[0].is_dirty() );
    TESTLABS_CHECK_EQ( dirty_layer_rects.layer_rects()[0].x(), 20 );
    TESTLABS_CHECK_EQ( dirty_layer_rects.layer_rects()[0].y(), 20 );
    TESTLABS_CHECK_EQ( dirty_layer_rects.layer_rects()[0].width(), 20 );
    TESTLABS_CHECK_EQ( dirty_layer_rects.layer_rects()[0].height(), 20 );
    TESTLABS_CHECK( !dirty_layer_rects.layer_rects()[1].is_dirty() );
    TESTLABS_CHECK( dirty_layer_rects.layer_rects()[2].is_dirty() );
    TESTLABS_CHECK_EQ( dirty_layer_rects.layer_rects()[2].x(), 5 );
    TESTLABS_CHECK_EQ( dirty_layer_rects.layer_rects()[2].y(), 10 );
    TESTLABS_CHECK_EQ( dirty_layer_rects.layer_rects()[2].width(), 30 );
    TESTLABS_CHECK_EQ( dirty_layer_rects.layer_rects()[2].height(), 25 );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerRects:  init, update, clear, not dirty" )
{
    xl7::graphics::textures::DirtyLayerRects dirty_layer_rects;
    dirty_layer_rects.update( 0, 20, 20, 20, 20 );
    dirty_layer_rects.update( 0, 25, 25, 10, 10 );
    dirty_layer_rects.update( 2, 25, 25, 10, 10 );
    dirty_layer_rects.update( 2, 5, 10, 10, 20 );
    dirty_layer_rects.clear();
    TESTLABS_CHECK( !dirty_layer_rects.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_rects.is_all_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerRects:  set dirty, all dirty" )
{
    xl7::graphics::textures::DirtyLayerRects dirty_layer_rects;
    dirty_layer_rects.set_dirty();
    TESTLABS_CHECK( dirty_layer_rects.is_dirty() );
    TESTLABS_CHECK( dirty_layer_rects.is_all_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerRects:  init, set dirty, all dirty, clear, not dirty" )
{
    xl7::graphics::textures::DirtyLayerRects dirty_layer_rects;
    dirty_layer_rects.update( 0, 20, 20, 20, 20 );
    dirty_layer_rects.set_dirty();
    TESTLABS_CHECK( dirty_layer_rects.is_dirty() );
    TESTLABS_CHECK( dirty_layer_rects.is_all_dirty() );
    dirty_layer_rects.clear();
    TESTLABS_CHECK( !dirty_layer_rects.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_rects.is_all_dirty() );
}

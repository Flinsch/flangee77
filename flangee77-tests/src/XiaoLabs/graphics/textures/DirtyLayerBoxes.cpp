
#include <TestLabs/TestSuite.h>

#include <XiaoLabs/graphics/textures/DirtyLayerBoxes.h>

#include "../../shared.h"



TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerBoxes:  new object, not dirty" )
{
    xl7::graphics::textures::DirtyLayerBoxes dirty_layer_boxes;
    TESTLABS_CHECK( !dirty_layer_boxes.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_boxes.is_all_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerBoxes:  init empty, not dirty" )
{
    xl7::graphics::textures::DirtyLayerBoxes dirty_layer_boxes;
    dirty_layer_boxes.update( 0, 0, 0, 0, 0, 0, 0 );
    TESTLABS_CHECK( !dirty_layer_boxes.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_boxes.is_all_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerBoxes:  init empty width, not dirty" )
{
    xl7::graphics::textures::DirtyLayerBoxes dirty_layer_boxes;
    dirty_layer_boxes.update( 0, 0, 0, 0, 0, 1, 1 );
    TESTLABS_CHECK( !dirty_layer_boxes.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_boxes.is_all_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerBoxes:  init empty height, not dirty" )
{
    xl7::graphics::textures::DirtyLayerBoxes dirty_layer_boxes;
    dirty_layer_boxes.update( 0, 0, 0, 0, 1, 0, 1 );
    TESTLABS_CHECK( !dirty_layer_boxes.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_boxes.is_all_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerBoxes:  init empty depth, not dirty" )
{
    xl7::graphics::textures::DirtyLayerBoxes dirty_layer_boxes;
    dirty_layer_boxes.update( 0, 0, 0, 0, 1, 1, 0 );
    TESTLABS_CHECK( !dirty_layer_boxes.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_boxes.is_all_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerBoxes:  init non-empty, is dirty" )
{
    xl7::graphics::textures::DirtyLayerBoxes dirty_layer_boxes;
    dirty_layer_boxes.update( 0, 0, 0, 0, 1, 1, 1 );
    TESTLABS_CHECK( dirty_layer_boxes.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_boxes.is_all_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerBoxes:  init non-empty, update empty, still dirty" )
{
    xl7::graphics::textures::DirtyLayerBoxes dirty_layer_boxes;
    dirty_layer_boxes.update( 0, 0, 0, 0, 1, 1, 1 );
    dirty_layer_boxes.update( 0, 0, 0, 0, 0, 0, 0 );
    TESTLABS_CHECK( dirty_layer_boxes.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_boxes.is_all_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerBoxes:  init non-empty, clear, not dirty" )
{
    xl7::graphics::textures::DirtyLayerBoxes dirty_layer_boxes;
    dirty_layer_boxes.update( 0, 0, 0, 0, 1, 1, 1 );
    dirty_layer_boxes.clear();
    TESTLABS_CHECK( !dirty_layer_boxes.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_boxes.is_all_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerBoxes:  update without effect" )
{
    xl7::graphics::textures::DirtyLayerBoxes dirty_layer_boxes;
    dirty_layer_boxes.update( 0, 20, 20, 20, 20, 20, 20 );
    dirty_layer_boxes.update( 0, 25, 25, 25, 10, 10, 10 );
    TESTLABS_CHECK( dirty_layer_boxes.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_boxes.is_all_dirty() );
    TESTLABS_ASSERT_EQ( dirty_layer_boxes.layer_boxes().size(), 1 );
    TESTLABS_CHECK( dirty_layer_boxes.layer_boxes()[0].is_dirty() );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[0].x(), 20 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[0].y(), 20 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[0].z(), 20 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[0].width(), 20 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[0].height(), 20 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[0].depth(), 20 );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerBoxes:  update with expansion" )
{
    xl7::graphics::textures::DirtyLayerBoxes dirty_layer_boxes;
    dirty_layer_boxes.update( 0, 20, 20, 20, 20, 20, 20 );
    dirty_layer_boxes.update( 0, 5, 10, 0, 10, 20, 50 );
    TESTLABS_CHECK( dirty_layer_boxes.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_boxes.is_all_dirty() );
    TESTLABS_ASSERT_EQ( dirty_layer_boxes.layer_boxes().size(), 1 );
    TESTLABS_CHECK( dirty_layer_boxes.layer_boxes()[0].is_dirty() );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[0].x(), 5 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[0].y(), 10 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[0].z(), 0 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[0].width(), 35 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[0].height(), 30 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[0].depth(), 50 );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerBoxes:  update another layer" )
{
    xl7::graphics::textures::DirtyLayerBoxes dirty_layer_boxes;
    dirty_layer_boxes.update( 0, 20, 20, 20, 20, 20, 20 );
    dirty_layer_boxes.update( 2, 25, 25, 25, 10, 10, 10 );
    dirty_layer_boxes.update( 2, 5, 10, 0, 10, 20, 50 );
    TESTLABS_CHECK( dirty_layer_boxes.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_boxes.is_all_dirty() );
    TESTLABS_ASSERT_EQ( dirty_layer_boxes.layer_boxes().size(), 3 );
    TESTLABS_CHECK( dirty_layer_boxes.layer_boxes()[0].is_dirty() );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[0].x(), 20 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[0].y(), 20 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[0].z(), 20 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[0].width(), 20 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[0].height(), 20 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[0].depth(), 20 );
    TESTLABS_CHECK( !dirty_layer_boxes.layer_boxes()[1].is_dirty() );
    TESTLABS_CHECK( dirty_layer_boxes.layer_boxes()[2].is_dirty() );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[2].x(), 5 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[2].y(), 10 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[2].z(), 0 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[2].width(), 30 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[2].height(), 25 );
    TESTLABS_CHECK_EQ( dirty_layer_boxes.layer_boxes()[2].depth(), 50 );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerBoxes:  init, update, clear, not dirty" )
{
    xl7::graphics::textures::DirtyLayerBoxes dirty_layer_boxes;
    dirty_layer_boxes.update( 0, 20, 20, 20, 20, 20, 20 );
    dirty_layer_boxes.update( 0, 25, 25, 25, 10, 10, 10 );
    dirty_layer_boxes.update( 2, 25, 25, 25, 10, 10, 10 );
    dirty_layer_boxes.update( 2, 5, 10, 0, 10, 20, 50 );
    dirty_layer_boxes.clear();
    TESTLABS_CHECK( !dirty_layer_boxes.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_boxes.is_all_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerBoxes:  set dirty, all dirty" )
{
    xl7::graphics::textures::DirtyLayerBoxes dirty_layer_boxes;
    dirty_layer_boxes.set_dirty();
    TESTLABS_CHECK( dirty_layer_boxes.is_dirty() );
    TESTLABS_CHECK( dirty_layer_boxes.is_all_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyLayerBoxes:  init, set dirty, all dirty, clear, not dirty" )
{
    xl7::graphics::textures::DirtyLayerBoxes dirty_layer_boxes;
    dirty_layer_boxes.update( 0, 20, 20, 20, 20, 20, 20 );
    dirty_layer_boxes.set_dirty();
    TESTLABS_CHECK( dirty_layer_boxes.is_dirty() );
    TESTLABS_CHECK( dirty_layer_boxes.is_all_dirty() );
    dirty_layer_boxes.clear();
    TESTLABS_CHECK( !dirty_layer_boxes.is_dirty() );
    TESTLABS_CHECK( !dirty_layer_boxes.is_all_dirty() );
}


#include <TestLabs/TestSuite.h>

#include <XiaoLabs/graphics/textures/DirtyBox.h>

#include "../../shared.h"



TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyBox:  new object, not dirty" )
{
    xl7::graphics::textures::DirtyBox dirty_box;
    TESTLABS_CHECK( !dirty_box.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyBox:  init empty, not dirty" )
{
    xl7::graphics::textures::DirtyBox dirty_box;
    dirty_box.update( 0, 0, 0, 0, 0, 0 );
    TESTLABS_CHECK( !dirty_box.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyBox:  init empty width, not dirty" )
{
    xl7::graphics::textures::DirtyBox dirty_box;
    dirty_box.update( 0, 0, 0, 0, 1, 1 );
    TESTLABS_CHECK( !dirty_box.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyBox:  init empty height, not dirty" )
{
    xl7::graphics::textures::DirtyBox dirty_box;
    dirty_box.update( 0, 0, 0, 1, 0, 1 );
    TESTLABS_CHECK( !dirty_box.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyBox:  init empty depth, not dirty" )
{
    xl7::graphics::textures::DirtyBox dirty_box;
    dirty_box.update( 0, 0, 0, 1, 1, 0 );
    TESTLABS_CHECK( !dirty_box.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyBox:  init non-empty, is dirty" )
{
    xl7::graphics::textures::DirtyBox dirty_box;
    dirty_box.update( 0, 0, 0, 1, 1, 1 );
    TESTLABS_CHECK( dirty_box.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyBox:  init non-empty, update empty, still dirty" )
{
    xl7::graphics::textures::DirtyBox dirty_box;
    dirty_box.update( 0, 0, 0, 1, 1, 1 );
    dirty_box.update( 0, 0, 0, 0, 0, 0 );
    TESTLABS_CHECK( dirty_box.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyBox:  init non-empty, clear, not dirty" )
{
    xl7::graphics::textures::DirtyBox dirty_box;
    dirty_box.update( 0, 0, 0, 1, 1, 1 );
    dirty_box.clear();
    TESTLABS_CHECK( !dirty_box.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyBox:  update without effect" )
{
    xl7::graphics::textures::DirtyBox dirty_box;
    dirty_box.update( 20, 20, 20, 20, 20, 20 );
    dirty_box.update( 25, 25, 25, 10, 10, 10 );
    TESTLABS_CHECK( dirty_box.is_dirty() );
    TESTLABS_CHECK_EQ( dirty_box.x(), 20 );
    TESTLABS_CHECK_EQ( dirty_box.y(), 20 );
    TESTLABS_CHECK_EQ( dirty_box.z(), 20 );
    TESTLABS_CHECK_EQ( dirty_box.width(), 20 );
    TESTLABS_CHECK_EQ( dirty_box.height(), 20 );
    TESTLABS_CHECK_EQ( dirty_box.depth(), 20);
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyBox:  update with expansion" )
{
    xl7::graphics::textures::DirtyBox dirty_box;
    dirty_box.update( 20, 20, 20, 20, 20, 20 );
    dirty_box.update( 5, 10, 0, 10, 20, 50 );
    TESTLABS_CHECK( dirty_box.is_dirty() );
    TESTLABS_CHECK_EQ( dirty_box.x(), 5 );
    TESTLABS_CHECK_EQ( dirty_box.y(), 10 );
    TESTLABS_CHECK_EQ( dirty_box.z(), 0 );
    TESTLABS_CHECK_EQ( dirty_box.width(), 35 );
    TESTLABS_CHECK_EQ( dirty_box.height(), 30 );
    TESTLABS_CHECK_EQ( dirty_box.depth(), 50 );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyBox:  init, update, clear, not dirty" )
{
    xl7::graphics::textures::DirtyBox dirty_box;
    dirty_box.update( 20, 20, 20, 20, 20, 20 );
    dirty_box.update( 25, 25, 25, 10, 10, 10 );
    dirty_box.update( 5, 10, 0, 10, 20, 50 );
    dirty_box.clear();
    TESTLABS_CHECK( !dirty_box.is_dirty() );
}

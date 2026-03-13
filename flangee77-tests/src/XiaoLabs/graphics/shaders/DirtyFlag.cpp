
#include <TestLabs/TestSuite.h>

#include <XiaoLabs/graphics/shaders/DirtyFlag.h>

#include "../../shared.h"



TESTLABS_CASE( u8"XiaoLabs:  graphics:  shaders:  DirtyFlag:  new object, not dirty" )
{
    xl7::graphics::shaders::DirtyFlag dirty_flag;
    TESTLABS_CHECK( !dirty_flag.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  shaders:  DirtyFlag:  init false, not dirty" )
{
    xl7::graphics::shaders::DirtyFlag dirty_flag;
    dirty_flag.update( false );
    TESTLABS_CHECK( !dirty_flag.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  shaders:  DirtyFlag:  init true, is dirty" )
{
    xl7::graphics::shaders::DirtyFlag dirty_flag;
    dirty_flag.update( true );
    TESTLABS_CHECK( dirty_flag.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  shaders:  DirtyFlag:  init true, update false, still dirty" )
{
    xl7::graphics::shaders::DirtyFlag dirty_flag;
    dirty_flag.update( true );
    dirty_flag.update( false );
    TESTLABS_CHECK( dirty_flag.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  shaders:  DirtyFlag:  init true, clear, not dirty" )
{
    xl7::graphics::shaders::DirtyFlag dirty_flag;
    dirty_flag.update( true );
    dirty_flag.clear();
    TESTLABS_CHECK( !dirty_flag.is_dirty() );
}

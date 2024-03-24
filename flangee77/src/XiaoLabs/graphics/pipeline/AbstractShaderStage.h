#pragma once
#ifndef XL7_GRAPHICS_PIPELINE_ABSTRACTSHADERSTAGE_H
#define XL7_GRAPHICS_PIPELINE_ABSTRACTSHADERSTAGE_H
#include "./AbstractStage.h"

#include "../shaders/Shader.h"



namespace xl7 {
namespace graphics {
namespace pipeline {



class AbstractShaderStage
    : public AbstractStage
{

private:
    static constexpr unsigned DIRTY_FLAG_SHADER_ID  = 0x1;



    // #############################################################################
    // States
    // #############################################################################
private:
    _XL7_GRAPHICS_PIPELINE_SINGLE_STATE( shader_id, resources::ResourceID, resources::ResourceID::INVALID_ID, DIRTY_FLAG_SHADER_ID );

}; // class AbstractShaderStage



} // namespace pipeline
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_PIPELINE_ABSTRACTSHADERSTAGE_H

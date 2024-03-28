#pragma once
#ifndef XL7_GRAPHICS_PIPELINE_ABSTRACTSHADERSTAGE_H
#define XL7_GRAPHICS_PIPELINE_ABSTRACTSHADERSTAGE_H
#include "./AbstractStage.h"

#include "../shaders/Shader.h"
#include "../textures/Texture.h"



namespace xl7 {
namespace graphics {
namespace pipeline {



class AbstractShaderStage
    : public AbstractStage
{

public:
    /** The maximum number of texture/sampler slots (ignoring hardware/driver capabilities). */
    static constexpr unsigned MAX_TEXTURE_SAMPLER_SLOTS = 16;

private:
    static constexpr unsigned DIRTY_FLAG_SHADER     = 0x1;
    static constexpr unsigned DIRTY_FLAG_TEXTURE    = 0x2;



    // #############################################################################
    // States
    // #############################################################################
private:
    _XL7_GRAPHICS_PIPELINE_SINGLE_STATE( shader_id, resources::ResourceID, resources::ResourceID::INVALID_ID, DIRTY_FLAG_SHADER );
    _XL7_GRAPHICS_PIPELINE_STATE_ARRAY( texture_id, MAX_TEXTURE_SAMPLER_SLOTS, resources::ResourceID, resources::ResourceID::INVALID_ID, DIRTY_FLAG_TEXTURE );

}; // class AbstractShaderStage



} // namespace pipeline
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_PIPELINE_ABSTRACTSHADERSTAGE_H

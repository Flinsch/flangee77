#pragma once
#ifndef XL7_GRAPHICS_PIPELINE_ABSTRACTSHADERSTAGE_H
#define XL7_GRAPHICS_PIPELINE_ABSTRACTSHADERSTAGE_H
#include "./AbstractStage.h"

#include "../shaders/Shader.h"
#include "../shaders/ConstantBuffer.h"
#include "../textures/Texture.h"
#include "../states/SamplerState.h"



namespace xl7 {
namespace graphics {
namespace pipeline {



class AbstractShaderStage
    : public AbstractStage
{

public:
    /** The maximum number of constant buffer slots (ignoring hardware/driver capabilities). */
    static constexpr unsigned MAX_CONSTANT_BUFFER_SLOTS = 8;
    /** The maximum number of texture/sampler slots (ignoring hardware/driver capabilities). */
    static constexpr unsigned MAX_TEXTURE_SAMPLER_SLOTS = 16;

private:
    static constexpr unsigned DIRTY_SHADER_FLAG                 = 0x1;
    static constexpr unsigned DIRTY_CONSTANT_BUFFER_FLAG_BASE   = 0x2;
    static constexpr unsigned DIRTY_CONSTANT_BUFFER_FLAG_MASK   = ((DIRTY_CONSTANT_BUFFER_FLAG_BASE << MAX_CONSTANT_BUFFER_SLOTS) - 1) & ~(DIRTY_CONSTANT_BUFFER_FLAG_BASE - 1);
    static constexpr unsigned DIRTY_TEXTURE_SAMPLER_FLAG_BASE   = 0x4 << (MAX_CONSTANT_BUFFER_SLOTS - 1);
    static constexpr unsigned DIRTY_TEXTURE_SAMPLER_FLAG_MASK   = ((DIRTY_TEXTURE_SAMPLER_FLAG_BASE << MAX_TEXTURE_SAMPLER_SLOTS) - 1) & ~(DIRTY_TEXTURE_SAMPLER_FLAG_BASE - 1);



    // #############################################################################
    // States
    // #############################################################################
private:
    _XL7_GRAPHICS_PIPELINE_SINGLE_STATE( shader_id, resources::ResourceID, resources::ResourceID::INVALID_ID, DIRTY_SHADER_FLAG );
    _XL7_GRAPHICS_PIPELINE_STATE_ARRAY( constant_buffer_id, MAX_CONSTANT_BUFFER_SLOTS, resources::ResourceID, resources::ResourceID::INVALID_ID, DIRTY_CONSTANT_BUFFER_FLAG_BASE );
    _XL7_GRAPHICS_PIPELINE_STATE_ARRAY( texture_id, MAX_TEXTURE_SAMPLER_SLOTS, resources::ResourceID, resources::ResourceID::INVALID_ID, DIRTY_TEXTURE_SAMPLER_FLAG_BASE );
    _XL7_GRAPHICS_PIPELINE_STATE_ARRAY( sampler_state_id, MAX_TEXTURE_SAMPLER_SLOTS, resources::ResourceID, resources::ResourceID::INVALID_ID, DIRTY_TEXTURE_SAMPLER_FLAG_BASE );

}; // class AbstractShaderStage



} // namespace pipeline
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_PIPELINE_ABSTRACTSHADERSTAGE_H

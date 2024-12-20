#pragma once
#ifndef XL7_GRAPHICS_PIPELINE_PIPELINE_H
#define XL7_GRAPHICS_PIPELINE_PIPELINE_H
#include "./AbstractPipelineObject.h"

#include "./InputAssemblerStage.h"
#include "./VertexShaderStage.h"
#include "./RasterizerStage.h"
#include "./PixelShaderStage.h"
#include "./OutputMergerStage.h"



namespace xl7::graphics::pipeline {



class Pipeline
    : public AbstractPipelineObject
{

public:
    /** The input assembler stage. */
    InputAssemblerStage ia;
    /** The vertex shader stage. */
    VertexShaderStage vs;
    /** The rasterizer stage. */
    RasterizerStage rs;
    /** The pixel shader stage. */
    PixelShaderStage ps;
    /** The output merger stage. */
    OutputMergerStage om;

}; // class Pipeline



} // namespace xl7::graphics::pipeline

#endif // XL7_GRAPHICS_PIPELINE_PIPELINE_H

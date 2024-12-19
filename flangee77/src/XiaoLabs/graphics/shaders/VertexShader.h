#pragma once
#ifndef XL7_GRAPHICS_SHADERS_VERTEXSHADER_H
#define XL7_GRAPHICS_SHADERS_VERTEXSHADER_H
#include "./Shader.h"



namespace xl7::graphics::shaders {



class VertexShader // NOLINT(*-virtual-class-destructor)
    : public Shader
{

public:
    VertexShader() = delete;

    VertexShader(const VertexShader&) = delete;
    VertexShader& operator = (const VertexShader&) = delete;
    VertexShader(VertexShader&&) = delete;
    VertexShader& operator = (VertexShader&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::string_view get_type_string() const override { return TEXT("vertex shader"); }



protected:
    VertexShader(const CreateParams<Desc>& params);
    ~VertexShader() override = default;

}; // class VertexShader



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_VERTEXSHADER_H

#pragma once
#ifndef XL7_GRAPHICS_SHADERS_PIXELSHADER_H
#define XL7_GRAPHICS_SHADERS_PIXELSHADER_H
#include "./Shader.h"



namespace xl7::graphics::shaders {



class PixelShader // NOLINT(*-virtual-class-destructor)
    : public Shader
{

public:
    PixelShader() = delete;

    PixelShader(const PixelShader&) = delete;
    PixelShader& operator=(const PixelShader&) = delete;
    PixelShader(PixelShader&&) = delete;
    PixelShader& operator=(PixelShader&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::string_view get_type_string() const override { return TEXT("pixel shader"); }



protected:
    PixelShader(const CreateParams<Desc>& params);
    ~PixelShader() override = default;

}; // class PixelShader



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_PIXELSHADER_H

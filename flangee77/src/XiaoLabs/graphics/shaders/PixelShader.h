#ifndef XL7_GRAPHICS_SHADERS_PIXELSHADER_H
#define XL7_GRAPHICS_SHADERS_PIXELSHADER_H
#include "./Shader.h"



namespace xl7::graphics::shaders {



class PixelShader
    : public Shader
{

public:
    XL7_DERIVE_RESOURCE_ID(Shader);

    PixelShader() = delete;

    PixelShader(const PixelShader&) = delete;
    PixelShader& operator=(const PixelShader&) = delete;
    PixelShader(PixelShader&&) = delete;
    PixelShader& operator=(PixelShader&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"pixel shader"; }



protected:
    PixelShader(const CreateParams<Desc>& params);
    ~PixelShader() override = default;

}; // class PixelShader



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_PIXELSHADER_H

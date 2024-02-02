#pragma once
#ifndef XL7_GRAPHICS_SHADERS_PIXELSHADER_H
#define XL7_GRAPHICS_SHADERS_PIXELSHADER_H
#include "./Shader.h"



namespace xl7 {
namespace graphics {
namespace shaders {



class PixelShader;
typedef std::shared_ptr<PixelShader> PixelShaderPtr;



class PixelShader
    : public Shader
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    PixelShader(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~PixelShader() = default;

private:
    /** Default constructor. */
    PixelShader() = delete;
    /** Copy constructor. */
    PixelShader(const PixelShader&) = delete;
    /** Copy assignment operator. */
    PixelShader& operator = (const PixelShader&) = delete;



    // #############################################################################
    // Resource Implementations
    // #############################################################################
public:
    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    virtual cl7::string_view get_type_string() const override { return TEXT("pixel shader"); }

}; // class PixelShader



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_PIXELSHADER_H

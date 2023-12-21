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
    PixelShader(ShaderManager* manager, const cl7::string& identifier);

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

}; // class PixelShader



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_PIXELSHADER_H

#pragma once
#ifndef XL7_GRAPHICS_SHADERS_VERTEXSHADER_H
#define XL7_GRAPHICS_SHADERS_VERTEXSHADER_H
#include "./Shader.h"



namespace xl7 {
namespace graphics {
namespace shaders {



class VertexShader;
typedef std::shared_ptr<VertexShader> VertexShaderPtr;



class VertexShader
    : public Shader
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    explicit VertexShader(ShaderManager* manager);

    /**
     * Explicit constructor.
     */
    VertexShader(ShaderManager* manager, const cl7::string& identifier);

    /**
     * Destructor.
     */
    virtual ~VertexShader() = default;

private:
    /** Default constructor. */
    VertexShader() = delete;
    /** Copy constructor. */
    VertexShader(const VertexShader&) = delete;
    /** Copy assignment operator. */
    VertexShader& operator = (const VertexShader&) = delete;

}; // class VertexShader



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_VERTEXSHADER_H

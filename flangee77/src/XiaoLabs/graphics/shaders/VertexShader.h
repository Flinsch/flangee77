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
    VertexShader(const CreateParams<Desc>& params);

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



    // #############################################################################
    // Resource Implementations
    // #############################################################################
public:
    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    virtual cl7::string_view get_type_string() const override { return TEXT("vertex shader"); }

}; // class VertexShader



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_VERTEXSHADER_H

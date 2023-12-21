#pragma once
#ifndef XL7_GRAPHICS_SHADERS_SHADER_H
#define XL7_GRAPHICS_SHADERS_SHADER_H
#include "../../Resource.h"



namespace xl7 {
namespace graphics {
namespace shaders {



class Shader;
typedef std::weak_ptr<Shader> ShaderPtr;



class ShaderManager;



class Shader
    : public Resource
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    explicit Shader(ShaderManager* manager);

    /**
     * Explicit constructor.
     */
    Shader(ShaderManager* manager, const cl7::string& identifier);

    /**
     * Destructor.
     */
    virtual ~Shader() = default;

private:
    /** Default constructor. */
    Shader() = delete;
    /** Copy constructor. */
    Shader(const Shader&) = delete;
    /** Copy assignment operator. */
    Shader& operator = (const Shader&) = delete;

}; // class Shader



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_SHADER_H

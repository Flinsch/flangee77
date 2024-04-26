#pragma once
#ifndef XL7_GRAPHICS_SHADERS_SHADERMANAGER_H
#define XL7_GRAPHICS_SHADERS_SHADERMANAGER_H
#include "../../resources/ResourceManager.h"

#include "./IShaderFactory.h"



namespace xl7 {
namespace graphics {
    class RenderingDevice;
namespace shaders {



class ShaderManager
    : public resources::ResourceManager
{

public:
    class Attorney
    {
        static ShaderManager* create(IShaderFactory* factory) { return new ShaderManager( factory ); }
        static void destroy(ShaderManager* manager) { delete manager; }
        friend class RenderingDevice;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    ShaderManager(IShaderFactory* factory);

    /**
     * Destructor.
     */
    virtual ~ShaderManager();

private:
    /** Default constructor. */
    ShaderManager() = delete;
    /** Copy constructor. */
    ShaderManager(const ShaderManager&) = delete;
    /** Copy assignment operator. */
    ShaderManager& operator = (const ShaderManager&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The shader factory.
     */
    IShaderFactory* const _factory;



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Creates and acquires the specified constant buffer.
     */
    resources::ResourceID create_constant_buffer(cl7::astring_view identifier, const ConstantBuffer::Desc& desc, const ConstantDataProvider& constant_data_provider = {});

    /**
     * Creates and acquires the specified vertex shader.
     * The name of the shader entry point can be empty, especially for precompiled
     * shaders; a standard name is then used for (re)compilable shaders.
     */
    resources::ResourceID create_vertex_shader(cl7::astring_view identifier, const CodeDataProvider& code_data_provider, cl7::astring_view entry_point = "");

    /**
     * Creates and acquires the specified pixel shader.
     * The name of the shader entry point can be empty, especially for precompiled
     * shaders; a standard name is then used for (re)compilable shaders.
     */
    resources::ResourceID create_pixel_shader(cl7::astring_view identifier, const CodeDataProvider& code_data_provider, cl7::astring_view entry_point = "");

}; // class ShaderManager



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_SHADERMANAGER_H

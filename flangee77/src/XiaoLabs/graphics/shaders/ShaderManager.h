#pragma once
#ifndef XL7_GRAPHICS_SHADERS_SHADERMANAGER_H
#define XL7_GRAPHICS_SHADERS_SHADERMANAGER_H
#include "../../resources/ResourceManager.h"

#include "./IShaderFactory.h"



namespace xl7::graphics {
    class RenderingDevice;
namespace shaders {



class ShaderManager final // NOLINT(*-virtual-class-destructor)
    : public resources::ResourceManager
{

public:
    class Attorney
    {
        static ShaderManager* create(IShaderFactory* factory) { return new ShaderManager(factory); }
        static void destroy(ShaderManager* manager) { delete manager; }
        friend class xl7::graphics::RenderingDevice;
    };



    ShaderManager() = delete;

    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;
    ShaderManager(ShaderManager&&) = delete;
    ShaderManager& operator=(ShaderManager&&) = delete;



    /**
     * Creates and acquires the specified constant buffer.
     */
    resources::ResourceID create_constant_buffer(cl7::u8string_view identifier, const ConstantBuffer::Desc& desc, const ConstantDataProvider& constant_data_provider = {});

    /**
     * Creates and acquires the specified vertex shader.
     * The name of the shader entry point can be empty, especially for precompiled
     * shaders; a standard name is then used for (re)compilable shaders.
     */
    resources::ResourceID create_vertex_shader(cl7::u8string_view identifier, const CodeDataProvider& code_data_provider, cl7::u8string_view entry_point = u8"");

    /**
     * Creates and acquires the specified pixel shader.
     * The name of the shader entry point can be empty, especially for precompiled
     * shaders; a standard name is then used for (re)compilable shaders.
     */
    resources::ResourceID create_pixel_shader(cl7::u8string_view identifier, const CodeDataProvider& code_data_provider, cl7::u8string_view entry_point = u8"");



protected:
    ShaderManager(IShaderFactory* factory) : _factory(factory) {}
    ~ShaderManager() override = default;



private:
    /**
     * The shader factory.
     */
    IShaderFactory* const _factory;

}; // class ShaderManager



} // namespace shaders
} // namespace xl7::graphics

#endif // XL7_GRAPHICS_SHADERS_SHADERMANAGER_H

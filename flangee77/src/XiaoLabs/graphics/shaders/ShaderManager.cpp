#include "ShaderManager.h"



namespace xl7::graphics::shaders {



    /**
     * Creates and acquires the specified constant buffer.
     */
    ConstantBuffer::Id ShaderManager::create_constant_buffer(cl7::u8string_view identifier, const ConstantBufferDesc& desc, const ConstantDataProvider& constant_data_provider)
    {
        resources::Resource::CreateContext ctx{.manager = this, .id = _next_id(), .identifier = identifier};

        ResourcePtr constant_buffer(_factory->create_constant_buffer(ctx, desc), _destroy_resource);

        return _try_acquire_and_add_resource<ConstantBuffer::Id>(std::move(constant_buffer), constant_data_provider);
    }

    /**
     * Creates and acquires the specified vertex shader.
     * The name of the shader entry point can be empty, especially for precompiled
     * shaders; a standard name is then used for (re)compilable shaders.
     */
    VertexShader::Id ShaderManager::create_vertex_shader(cl7::u8string_view identifier, const CodeDataProvider& code_data_provider, cl7::astring_view entry_point)
    {
        ShaderDesc desc{.language = code_data_provider.get_language(), .entry_point = cl7::astring(entry_point)};
        resources::Resource::CreateContext ctx{.manager = this, .id = _next_id(), .identifier = identifier};

        ResourcePtr vertex_shader(_factory->create_vertex_shader(ctx, desc), _destroy_resource);

        return _try_acquire_and_add_resource<VertexShader::Id>(std::move(vertex_shader), code_data_provider);
    }

    /**
     * Creates and acquires the specified pixel shader.
     * The name of the shader entry point can be empty, especially for precompiled
     * shaders; a standard name is then used for (re)compilable shaders.
     */
    PixelShader::Id ShaderManager::create_pixel_shader(cl7::u8string_view identifier, const CodeDataProvider& code_data_provider, cl7::astring_view entry_point)
    {
        ShaderDesc desc{.language = code_data_provider.get_language(), .entry_point = cl7::astring(entry_point)};
        resources::Resource::CreateContext ctx{.manager = this, .id = _next_id(), .identifier = identifier};

        ResourcePtr pixel_shader(_factory->create_pixel_shader(ctx, desc), _destroy_resource);

        return _try_acquire_and_add_resource<PixelShader::Id>(std::move(pixel_shader), code_data_provider);
    }



} // namespace xl7::graphics::shaders

#include "ShaderManager.h"



namespace xl7::graphics::shaders {



    /**
     * Creates and acquires the specified constant buffer.
     */
    ConstantBuffer::Id ShaderManager::create_constant_buffer(cl7::u8string_view identifier, const ConstantBufferDesc& desc, const ConstantBufferWrite* initial_write)
    {
        resources::Resource::CreateContext ctx{.manager = this, .id = _next_id(), .identifier = identifier};

        auto* constant_buffer = _factory->create_constant_buffer(ctx, desc);
        ResourcePtr resource_ptr(constant_buffer, _destroy_resource);

        if (initial_write)
            constant_buffer->edit().write(*initial_write);

        return _try_acquire_and_add_resource<ConstantBuffer::Id>(std::move(resource_ptr));
    }

    /**
     * Creates and acquires the specified vertex shader.
     * The name of the shader entry point can be empty, especially for precompiled
     * shaders; a standard name is then used for (re)compilable shaders.
     */
    VertexShader::Id ShaderManager::create_vertex_shader(cl7::u8string_view identifier, const ShaderDesc& desc, const ShaderWrite& write)
    {
        assert(write.shader_code->get_language() == desc.language);

        resources::Resource::CreateContext ctx{.manager = this, .id = _next_id(), .identifier = identifier};

        auto* vertex_shader = _factory->create_vertex_shader(ctx, desc);
        ResourcePtr resource_ptr(vertex_shader, _destroy_resource);

        assert(write.shader_code);
        assert(write.compile_options);
        Shader::Attorney::build(vertex_shader, *write.shader_code, *write.compile_options);

        return _try_acquire_and_add_resource<VertexShader::Id>(std::move(resource_ptr));
    }

    /**
     * Creates and acquires the specified pixel shader.
     * The name of the shader entry point can be empty, especially for precompiled
     * shaders; a standard name is then used for (re)compilable shaders.
     */
    PixelShader::Id ShaderManager::create_pixel_shader(cl7::u8string_view identifier, const ShaderDesc& desc, const ShaderWrite& write)
    {
        assert(write.shader_code->get_language() == desc.language);

        resources::Resource::CreateContext ctx{.manager = this, .id = _next_id(), .identifier = identifier};

        auto* pixel_shader = _factory->create_pixel_shader(ctx, desc);
        ResourcePtr resource_ptr(pixel_shader, _destroy_resource);

        assert(write.shader_code);
        assert(write.compile_options);
        Shader::Attorney::build(pixel_shader, *write.shader_code, *write.compile_options);

        return _try_acquire_and_add_resource<PixelShader::Id>(std::move(resource_ptr));
    }



} // namespace xl7::graphics::shaders

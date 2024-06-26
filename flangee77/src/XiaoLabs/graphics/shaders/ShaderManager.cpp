#include "ShaderManager.h"



namespace xl7 {
namespace graphics {
namespace shaders {


    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    ShaderManager::ShaderManager(IShaderFactory* factory)
        : ResourceManager()
        , _factory( factory )
    {
    }

    /**
     * Destructor.
     */
    ShaderManager::~ShaderManager()
    {
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Creates and acquires the specified constant buffer.
     */
    resources::ResourceID ShaderManager::create_constant_buffer(cl7::astring_view identifier, const ConstantBuffer::Desc& desc, const ConstantDataProvider& constant_data_provider)
    {
        resources::Resource::CreateParams<ConstantBuffer::Desc> params{ this, _next_id(), identifier, desc };

        ResourcePtr constant_buffer( _factory->create_constant_buffer( params ), _destroy_resource );

        return _try_acquire_and_add_resource( std::move(constant_buffer), constant_data_provider );
    }

    /**
     * Creates and acquires the specified vertex shader.
     * The name of the shader entry point can be empty, especially for precompiled
     * shaders; a standard name is then used for (re)compilable shaders.
     */
    resources::ResourceID ShaderManager::create_vertex_shader(cl7::astring_view identifier, const CodeDataProvider& code_data_provider, cl7::astring_view entry_point)
    {
        Shader::Desc desc{ code_data_provider.get_language(), cl7::astring(entry_point) };
        resources::Resource::CreateParams<Shader::Desc> params{ this, _next_id(), identifier, desc };

        ResourcePtr vertex_shader( _factory->create_vertex_shader( params ), _destroy_resource );

        return _try_acquire_and_add_resource( std::move(vertex_shader), code_data_provider );
    }

    /**
     * Creates and acquires the specified pixel shader.
     * The name of the shader entry point can be empty, especially for precompiled
     * shaders; a standard name is then used for (re)compilable shaders.
     */
    resources::ResourceID ShaderManager::create_pixel_shader(cl7::astring_view identifier, const CodeDataProvider& code_data_provider, cl7::astring_view entry_point)
    {
        Shader::Desc desc{ code_data_provider.get_language(), cl7::astring(entry_point) };
        resources::Resource::CreateParams<Shader::Desc> params{ this, _next_id(), identifier, desc };

        ResourcePtr pixel_shader( _factory->create_pixel_shader( params ), _destroy_resource );

        return _try_acquire_and_add_resource( std::move(pixel_shader), code_data_provider );
    }



} // namespace shaders
} // namespace graphics
} // namespace xl7

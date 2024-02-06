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
     * Creates and acquires the specified vertex shader.
     * The name of the shader entry point can be empty, especially for precompiled
     * shaders; a standard name is then used for (re)compilable shaders.
     */
    VertexShaderPtr ShaderManager::create_vertex_shader(const cl7::string_view& identifier, const CodeProvider& code_provider, const cl7::astring_view& entry_point)
    {
        Shader::Desc desc{ code_provider.get_language(), cl7::astring(entry_point) };
        resources::Resource::CreateParams<Shader::Desc> params{ this, identifier, desc };

        VertexShaderPtr vertex_shader( _factory->create_vertex_shader( params ), _destroy_resource );

        return _try_acquire_and_add_resource( vertex_shader, code_provider );
    }

    /**
     * Creates and acquires the specified pixel shader.
     * The name of the shader entry point can be empty, especially for precompiled
     * shaders; a standard name is then used for (re)compilable shaders.
     */
    PixelShaderPtr ShaderManager::create_pixel_shader(const cl7::string_view& identifier, const CodeProvider& code_provider, const cl7::astring_view& entry_point)
    {
        Shader::Desc desc{ code_provider.get_language(), cl7::astring(entry_point) };
        resources::Resource::CreateParams<Shader::Desc> params{ this, identifier, desc };

        PixelShaderPtr pixel_shader( _factory->create_pixel_shader( params ), _destroy_resource );

        return _try_acquire_and_add_resource( pixel_shader, code_provider );
    }



} // namespace shaders
} // namespace graphics
} // namespace xl7

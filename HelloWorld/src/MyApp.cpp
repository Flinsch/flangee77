#include "MyApp.h"

#include <XiaoLabs/graphics.h>
    #include <XiaoLabs/graphics/images/TargaImageHandler.h>
    #include <XiaoLabs/graphics/images/PngImageHandler.h>

#include <CoreLabs/filesystem.h>

#include <CoreLabs/profiling.h>
#include <CoreLabs/logging.h>
#include <CoreLabs/strings.h>

#include <CoreLabs/fstream.h>
#include <CoreLabs/sstream.h>

#include <algorithm>



namespace helloworld {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    MyApp::MyApp(int argc, cl7::char_type* argv[])
        : pl7::Application( argc, argv )
    {
    }

    /**
     * Destructor.
     */
    MyApp::~MyApp()
    {
    }



    // #############################################################################
    // Application Implementations
    // #############################################################################

    /**
     * Initializes the actual application instance before initializing the framework
     * components. The passed configuration object might be manipulated to influence
     * the following initialization process accordingly. In the event of an error,
     * false is returned and the application exits without continuing initialization
     * let alone entering the main loop.
     */
    bool MyApp::_pre_init_impl(xl7::Config& config)
    {
        return true;
    }

    /**
     * Initializes the actual application instance after the framework components
     * have (successfully) been initialized. In the event of an error, false is
     * returned and the application exits without even entering the main loop.
     */
    bool MyApp::_post_init_impl()
    {
        struct Vertex
        {
            float x, y, z;
            unsigned color;
            float u, v;
        };

        xl7::graphics::meshes::VertexLayout vertex_layout;
        vertex_layout.elements.push_back( { xl7::graphics::meshes::VertexLayout::Semantic::POSITION,    0,  xl7::graphics::meshes::VertexLayout::DataType::FLOAT3 } );
        vertex_layout.elements.push_back( { xl7::graphics::meshes::VertexLayout::Semantic::COLOR,       0,  xl7::graphics::meshes::VertexLayout::DataType::COLOR } );
        vertex_layout.elements.push_back( { xl7::graphics::meshes::VertexLayout::Semantic::TEXCOORD,    0,  xl7::graphics::meshes::VertexLayout::DataType::FLOAT2 } );
        assert( vertex_layout.calculate_size() == sizeof(Vertex) );

        xl7::graphics::meshes::VertexBuffer::Desc vertex_buffer_desc {
            xl7::resources::ResourceUsage::Immutable,
            xl7::graphics::meshes::Topology::PointList,
            4,
            sizeof(Vertex),
            vertex_layout,
        };

        xl7::graphics::meshes::IndexBuffer::Desc index_buffer_desc {
            xl7::resources::ResourceUsage::Immutable,
            xl7::graphics::meshes::Topology::TriangleStrip,
            4,
            xl7::graphics::meshes::IndexType::UINT16,
        };

        constexpr Vertex vertices[] {
            { -0.5f, -0.5f,  0.5f, 0xffff0000, -0.5, 1.5 },
            { -0.5f, +0.5f,  0.5f, 0xff00ff00, -0.5, -0.5 },
            { +0.5f, -0.5f,  0.5f, 0xff0000ff, 1.5, 1.5 },
            { +0.5f, +0.5f,  0.5f, 0xffffff00, 1.5, -0.5 },
        };
        constexpr unsigned short indices[] { 0, 1, 2, 3 };

        xl7::graphics::meshes::VertexDataProvider<Vertex> vertex_data_provider{ vertices };
        xl7::graphics::meshes::IndexDataProvider<unsigned short> index_data_provider{ indices };

        _vertex_buffer_id = xl7::graphics::mesh_manager()->create_vertex_buffer( "My Vertex Buffer", vertex_buffer_desc, vertex_data_provider );
        _index_buffer_id = xl7::graphics::mesh_manager()->create_index_buffer( "My Index Buffer", index_buffer_desc, index_data_provider );


        cl7::aifstream ifs( cl7::filesystem::get_working_directory() + TEXT("assets/shaders/shader.hlsl") );
        assert( ifs );
        assert( ifs.is_open() );
        cl7::asstream oss;
        oss << ifs.rdbuf();
        cl7::astring high_level_code = oss.str();
        xl7::graphics::shaders::ShaderCode shader_code{ high_level_code };
        xl7::graphics::shaders::MacroDefinitions macro_definitions;
        xl7::graphics::shaders::CodeDataProvider code_data_provider{ &shader_code, &macro_definitions };

        _vertex_shader_id = xl7::graphics::shader_manager()->create_vertex_shader( "My Vertex Shader", code_data_provider );
        _pixel_shader_id = xl7::graphics::shader_manager()->create_pixel_shader( "My Pixel Shader", code_data_provider );


        xl7::graphics::shaders::ConstantBuffer::Desc constant_buffer_desc;
        constant_buffer_desc.layout.constant_declarations = {
            { "VertexOffset", xl7::graphics::shaders::ConstantType::Float, xl7::graphics::shaders::ConstantClass::Vector, 1, 3, 1, 0, 12, 12, },
            { "PixelBaseColor", xl7::graphics::shaders::ConstantType::Float, xl7::graphics::shaders::ConstantClass::Vector, 1, 4, 1, 12, 16, 16, },
        };
        
        _constant_buffer_id = xl7::graphics::shader_manager()->create_constant_buffer( "My Constant Buffer", constant_buffer_desc );


        xl7::graphics::images::Image image;
        xl7::graphics::images::TargaImageHandler targa_image_handler;
        xl7::graphics::images::PngImageHandler png_image_handler;
        targa_image_handler.load_from_file( cl7::filesystem::get_working_directory() + TEXT("assets/gfx/dummy.tga"), image );
        //targa_image_handler.load_from_file( cl7::filesystem::get_working_directory() + TEXT("assets/gfx/dummy-compressed.tga"), image );
        //png_image_handler.load_from_file( cl7::filesystem::get_working_directory() + TEXT("assets/gfx/dummy.png"), image );
        //png_image_handler.load_from_file( cl7::filesystem::get_working_directory() + TEXT("assets/gfx/dummy-indexed.png"), image );

        xl7::graphics::textures::Texture2D::Desc texture_desc {
            xl7::resources::ResourceUsage::Immutable,
            xl7::graphics::PixelFormat::R8G8B8A8_UNORM,
            xl7::graphics::ChannelOrder::RGBA,
            0, // mip_levels
            image.get_width(),
            image.get_height(),
        };

        xl7::graphics::states::SamplerState::Desc sampler_desc;

        xl7::graphics::textures::ImageDataProvider image_data_provider{ &image };

        _texture_id = xl7::graphics::texture_manager()->create_texture_2d( "My Texture", texture_desc, image_data_provider );
        _sampler_state_id = xl7::graphics::state_manager()->ensure_sampler_state( sampler_desc );


        xl7::graphics::states::RasterizerState::Desc rasterizer_desc;
        rasterizer_desc.cull_mode = xl7::graphics::states::RasterizerState::CullMode::None;

        _rasterizer_state_id = xl7::graphics::state_manager()->ensure_rasterizer_state( rasterizer_desc );


        xl7::graphics::states::DepthStencilState::Desc depth_stencil_desc;
        depth_stencil_desc.is_depth_testing_enabled = true;
        depth_stencil_desc.is_depth_writing_enabled = true;

        _depth_stencil_state_id = xl7::graphics::state_manager()->ensure_depth_stencil_state( depth_stencil_desc );


        xl7::graphics::states::BlendState::Desc blend_desc;
        blend_desc.is_blending_enabled = true;
        blend_desc.src_color_factor = blend_desc.src_alpha_factor = xl7::graphics::states::BlendState::BlendFactor::SrcAlpha;
        blend_desc.dest_color_factor = blend_desc.dest_alpha_factor = xl7::graphics::states::BlendState::BlendFactor::InvSrcAlpha;

        _blend_state_id = xl7::graphics::state_manager()->ensure_blend_state( blend_desc );



        for ( size_t i = 0; i < xl7::graphics::shader_manager()->get_resource_count(); ++i )
        {
            const auto* resource = xl7::graphics::shader_manager()->get_resource( i );
            const auto* shader = dynamic_cast<const xl7::graphics::shaders::Shader*>( resource );
            if ( !shader )
                continue;

            LOG_TYPE( TEXT("Parameters of ") + shader->get_typed_identifier_string() + TEXT(':'), cl7::logging::LogType::Caption );

            std::vector<const xl7::graphics::shaders::ConstantBufferDeclaration*> constant_buffer_declarations;
            for ( const auto& constant_buffer_declaration : shader->get_reflection_result().constant_buffer_declarations )
                constant_buffer_declarations.push_back( &constant_buffer_declaration );
            std::sort( constant_buffer_declarations.begin(), constant_buffer_declarations.end(), [](const auto& a, const auto& b) {
                return a->index < b->index;
            } );

            for ( const auto* cb : constant_buffer_declarations )
            {
                if ( !cb->name.empty() )
                    LOG_TYPE( cl7::strings::from_ascii(cb->name) + TEXT("\tcb") + cl7::to_string(cb->index) + TEXT(" (") + cl7::to_string(cb->layout.calculate_size()) + TEXT(")"), cl7::logging::LogType::Item );

                std::vector<const xl7::graphics::shaders::ConstantDeclaration*> constant_declarations;
                for ( const auto& constant_declaration : cb->layout.constant_declarations )
                    constant_declarations.push_back( &constant_declaration );
                std::sort( constant_declarations.begin(), constant_declarations.end(), [](const auto& a, const auto& b) {
                    return a->offset < b->offset;
                } );

                for ( const auto* c : constant_declarations )
                {
                    assert( c->offset % 16 == 0 );
                    if ( cb->name.empty() )
                        LOG_TYPE( cl7::strings::from_ascii(c->name) + TEXT("\tc") + cl7::to_string(c->offset / 16) + TEXT(" (") + cl7::to_string((c->size + 15) / 16) + TEXT(")"), cl7::logging::LogType::Item );
                    else
                        LOG_TYPE( TEXT(".") + cl7::strings::from_ascii(c->name) + TEXT("\tc") + cl7::to_string(c->offset / 16) + TEXT(" (") + cl7::to_string((c->size + 15) / 16) + TEXT(")"), cl7::logging::LogType::Item );
                } // for each constant "variable"
            } // for each cbuffer

            std::vector<const xl7::graphics::shaders::TextureSamplerDeclaration*> texture_sampler_declarations;
            for ( const auto& texture_sampler_declaration : shader->get_reflection_result().texture_sampler_declarations )
                texture_sampler_declarations.push_back( &texture_sampler_declaration );
            std::sort( texture_sampler_declarations.begin(), texture_sampler_declarations.end(), [](const auto& a, const auto& b) {
                return a->index < b->index;
            } );

            for ( const auto* s : texture_sampler_declarations )
            {
                LOG_TYPE( cl7::strings::from_ascii(s->name) + TEXT("\ts") + cl7::to_string(s->index), cl7::logging::LogType::Item );
            } // for each texture/sampler
        }



        LOG_INFO( TEXT("Please note the following: The quick brown fox jumps over the lazy dog.") );
        LOG_SUCCESS( TEXT("Great things have been done!") );
        LOG_WARNING( TEXT("Brace yourselves! Winter is coming.") );
        LOG_ERROR( TEXT("Something went terribly wrong!") );

        LOG_TYPE( TEXT("Caption"), cl7::logging::LogType::Caption );
        LOG_TYPE( TEXT("Section"), cl7::logging::LogType::Section );
        LOG_TYPE( TEXT("Item A\tItem"), cl7::logging::LogType::Item );
        LOG_TYPE( TEXT("Item B\tPass"), cl7::logging::LogType::ItemPass );
        LOG_TYPE( TEXT("Item C\tFail"), cl7::logging::LogType::ItemFail );

        LOG_TYPE( TEXT("print( \"Hello, World!\" );"), cl7::logging::LogType::Code );
        LOG_TYPE( TEXT("This is so meta"), cl7::logging::LogType::Meta );
        LOG_TYPE( TEXT("The best comment is the one you don't have to write."), cl7::logging::LogType::Comment );
        LOG_TYPE( TEXT("This is something completely different."), cl7::logging::LogType::Other );

        return true;
    }

    /**
     * Shuts down the application instance before the framework components are also
     * shut down. If false is returned, the application will then be terminated in
     * any case, even if it should actually be restarted.
     */
    bool MyApp::_shutdown_impl()
    {
        xl7::graphics::state_manager()->release_resource_and_invalidate( _blend_state_id );
        xl7::graphics::state_manager()->release_resource_and_invalidate( _depth_stencil_state_id );
        xl7::graphics::state_manager()->release_resource_and_invalidate( _rasterizer_state_id );

        xl7::graphics::state_manager()->release_resource_and_invalidate( _sampler_state_id );
        xl7::graphics::texture_manager()->release_resource_and_invalidate( _texture_id );

        xl7::graphics::shader_manager()->release_resource_and_invalidate( _constant_buffer_id );

        xl7::graphics::shader_manager()->release_resource_and_invalidate( _vertex_shader_id );
        xl7::graphics::shader_manager()->release_resource_and_invalidate( _pixel_shader_id );

        xl7::graphics::mesh_manager()->release_resource_and_invalidate( _vertex_buffer_id );
        xl7::graphics::mesh_manager()->release_resource_and_invalidate( _index_buffer_id );

        return true;
    }

    /**
     * Performs the actual rendering.
     */
    void MyApp::_render_impl()
    {
        cl7::profiling::Profiler profiler( "MyApp::_render_impl" );

        unsigned fps = xl7::graphics::graphics_system().get_config().video.display_mode.refresh_rate;
        if ( !fps )
            fps = 60;
        static float a = 0.0f;
        a += 2.0f * 3.1414927f / static_cast<float>( fps );
        float sn = ::sinf( a ) * 0.01f;
        float cs = ::cosf( a ) * 0.01f;

        struct ConstantBufferData
        {
            float x, y, z;
            xl7::graphics::Color color;
        } constant_buffer_data = { cs, sn, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f } };

        xl7::graphics::shaders::ConstantDataProvider constant_data_provider( &constant_buffer_data );

        auto* constant_buffer = xl7::graphics::shader_manager()->find_resource<xl7::graphics::shaders::ConstantBuffer>( _constant_buffer_id );
        assert( constant_buffer );
        constant_buffer->update( constant_data_provider );


        auto* rendering_context = xl7::graphics::primary_context();

        rendering_context->clear( xl7::graphics::ClearFlags::All, { 1.0f, 0.333f, 0.75f }, 1.0f, 0x0 );

        rendering_context->pipeline.ia.set_vertex_buffer_id( _vertex_buffer_id );
        rendering_context->pipeline.ia.set_index_buffer_id( _index_buffer_id );

        rendering_context->pipeline.vs.set_shader_id( _vertex_shader_id );
        rendering_context->pipeline.vs.set_constant_buffer_id( 0, _constant_buffer_id );

        rendering_context->pipeline.ps.set_shader_id( _pixel_shader_id );
        rendering_context->pipeline.ps.set_constant_buffer_id( 0, _constant_buffer_id );
        rendering_context->pipeline.ps.set_texture_id( 0, _texture_id );
        rendering_context->pipeline.ps.set_sampler_state_id( 0, _sampler_state_id );

        rendering_context->pipeline.rs.set_rasterizer_state_id( _rasterizer_state_id );

        rendering_context->pipeline.om.set_depth_stencil_state_id( _depth_stencil_state_id );
        rendering_context->pipeline.om.set_blend_state_id( _blend_state_id );

        rendering_context->draw_indexed();
    }

    /**
     * Performs the application logic.
     */
    void MyApp::_move_impl()
    {
        cl7::profiling::Profiler profiler( "MyApp::_move_impl" );
    }



} // namespace helloworld

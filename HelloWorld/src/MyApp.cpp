#include "MyApp.h"

#include <XiaoLabs/graphics.h>

#include <CoreLabs/filesystem.h>

#include <CoreLabs/fstream.h>
#include <CoreLabs/sstream.h>



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
        };

        xl7::graphics::meshes::VertexLayout vertex_layout;
        vertex_layout.elements.push_back( { xl7::graphics::meshes::VertexLayout::Semantic::POSITION,    0,  xl7::graphics::meshes::VertexLayout::DataType::FLOAT3 } );
        vertex_layout.elements.push_back( { xl7::graphics::meshes::VertexLayout::Semantic::COLOR,       0,  xl7::graphics::meshes::VertexLayout::DataType::COLOR } );
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
            { -1.0f, -1.0f,  0.0f, 0xffff0000 },
            { -1.0f, +1.0f,  0.0f, 0xff00ff00 },
            { +1.0f, -1.0f,  0.0f, 0xff0000ff },
            { +1.0f, +1.0f,  0.0f, 0xffffff00 },
        };
        constexpr unsigned short indices[] { 0, 1, 2, 3 };

        xl7::graphics::meshes::VertexDataProvider<Vertex> vertex_data_provider{ vertices };
        xl7::graphics::meshes::IndexDataProvider<unsigned short> index_data_provider{ indices };

        _vertex_buffer_id = xl7::graphics::mesh_manager()->create_vertex_buffer( TEXT("My Vertex Buffer"), vertex_buffer_desc, vertex_data_provider );
        _index_buffer_id = xl7::graphics::mesh_manager()->create_index_buffer( TEXT("My Index Buffer"), index_buffer_desc, index_data_provider );



        cl7::aifstream ifs( cl7::filesystem::get_working_directory() + TEXT("assets/shaders/shader.hlsl") );
        assert( ifs );
        assert( ifs.is_open() );
        cl7::asstream oss;
        oss << ifs.rdbuf();
        cl7::astring high_level_code = oss.str();
        xl7::graphics::shaders::ShaderCode shader_code{ high_level_code };
        xl7::graphics::shaders::MacroDefinitions macro_definitions;
        xl7::graphics::shaders::CodeProvider code_provider{ &shader_code, &macro_definitions };

        _vertex_shader_id = xl7::graphics::shader_manager()->create_vertex_shader( TEXT("My Vertex Shader"), code_provider );
        _pixel_shader_id = xl7::graphics::shader_manager()->create_pixel_shader( TEXT("My Pixel Shader"), code_provider );

        return true;
    }

    /**
     * Shuts down the application instance before the framework components are also
     * shut down. If false is returned, the application will then be terminated in
     * any case, even if it should actually be restarted.
     */
    bool MyApp::_shutdown_impl()
    {
        xl7::graphics::mesh_manager()->release_resource_and_invalidate( _vertex_buffer_id );
        xl7::graphics::mesh_manager()->release_resource_and_invalidate( _index_buffer_id );

        xl7::graphics::shader_manager()->release_resource_and_invalidate( _vertex_shader_id );
        xl7::graphics::shader_manager()->release_resource_and_invalidate( _pixel_shader_id );

        return true;
    }

    /**
     * Performs the actual rendering.
     */
    void MyApp::_render_impl()
    {
    }

    /**
     * Performs the application logic.
     */
    void MyApp::_move_impl()
    {
    }



} // namespace helloworld
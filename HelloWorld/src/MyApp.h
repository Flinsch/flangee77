#pragma once
#ifndef HELLOWORLD_MYAPP_H
#define HELLOWORLD_MYAPP_H
#include <ProgLabs/Application.h>

#include <XiaoLabs/resources/ResourceID.h>



namespace helloworld {



class MyApp
    : public pl7::Application
{

public:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    MyApp() = delete;

    MyApp(int argc, wchar_t* argv[]);

    MyApp(const MyApp&) = delete;
    MyApp& operator=(const MyApp&) = delete;
    MyApp(MyApp&&) = delete;
    MyApp& operator=(MyApp&&) = delete;

    ~MyApp() override = default;



private:

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
    bool _pre_init_impl(xl7::Config& config) override;

    /**
     * Initializes the actual application instance after the framework components
     * have (successfully) been initialized. In the event of an error, false is
     * returned and the application exits without even entering the main loop.
     */
    bool _post_init_impl() override;

    /**
     * Shuts down the application instance before the framework components are also
     * shut down. If false is returned, the application will then be terminated in
     * any case, even if it should actually be restarted.
     */
    bool _shutdown_impl() override;

    /**
     * Performs the actual rendering.
     */
    void _render_impl() override;

    /**
     * Performs the application logic.
     */
    void _move_impl() override;



    // #############################################################################
    // Attributes
    // #############################################################################

    xl7::resources::ResourceID _vertex_buffer_id;
    xl7::resources::ResourceID _index_buffer_id;

    xl7::resources::ResourceID _vertex_shader_id;
    xl7::resources::ResourceID _pixel_shader_id;

    xl7::resources::ResourceID _constant_buffer_id;

    xl7::resources::ResourceID _texture_id;
    xl7::resources::ResourceID _sampler_state_id;

    xl7::resources::ResourceID _rasterizer_state_id;
    xl7::resources::ResourceID _depth_stencil_state_id;
    xl7::resources::ResourceID _blend_state_id;

}; // class MyApp



} // namespace helloworld

#endif // HELLOWORLD_MYAPP_H

#pragma once
#ifndef HELLOWORLD_MYAPP_H
#define HELLOWORLD_MYAPP_H
#include <ProgLabs/Application.h>

#include <XiaoLabs/resources/ResourceID.h>



namespace helloworld {



class MyApp
    : public pl7::Application
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Explicit constructor.
     */
    MyApp(int argc, cl7::char_type* argv[]);

    /**
     * Destructor.
     */
    virtual ~MyApp();

private:
    /** Default constructor. */
    MyApp() = delete;
    /** Copy constructor. */
    MyApp(const MyApp&) = delete;
    /** Copy assignment operator. */
    MyApp& operator = (const MyApp&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    xl7::resources::ResourceID _vertex_buffer_id;
    xl7::resources::ResourceID _index_buffer_id;

    xl7::resources::ResourceID _vertex_shader_id;
    xl7::resources::ResourceID _pixel_shader_id;

    xl7::resources::ResourceID _texture_id;
    xl7::resources::ResourceID _sampler_state_id;

    xl7::resources::ResourceID _rasterizer_state_id;



    // #############################################################################
    // Application Implementations
    // #############################################################################
private:
    /**
     * Initializes the actual application instance before initializing the framework
     * components. The passed configuration object might be manipulated to influence
     * the following initialization process accordingly. In the event of an error,
     * false is returned and the application exits without continuing initialization
     * let alone entering the main loop.
     */
    virtual bool _pre_init_impl(xl7::Config& config) override;

    /**
     * Initializes the actual application instance after the framework components
     * have (successfully) been initialized. In the event of an error, false is
     * returned and the application exits without even entering the main loop.
     */
    virtual bool _post_init_impl() override;

    /**
     * Shuts down the application instance before the framework components are also
     * shut down. If false is returned, the application will then be terminated in
     * any case, even if it should actually be restarted.
     */
    virtual bool _shutdown_impl() override;

    /**
     * Performs the actual rendering.
     */
    virtual void _render_impl() override;

    /**
     * Performs the application logic.
     */
    virtual void _move_impl() override;

}; // class MyApp



} // namespace helloworld

#endif // HELLOWORLD_MYAPP_H

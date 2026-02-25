#ifndef PL7_APPLICATION_H
#define PL7_APPLICATION_H

#include <XiaoLabs/Config.h>

#include "./ArgumentBag.h"



namespace pl7 {



class Application
{

public:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Application() = delete;

    explicit Application(ArgumentBag argument_bag) noexcept;

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

    virtual ~Application() noexcept = default;



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Runs the application instance.
     * Initializes the framework components, runs through the main message loop, and
     * shuts down the framework components.
     */
    bool run();

    /**
     * Returns the argument bag holding a list of command-line arguments.
     */
    const ArgumentBag& get_argument_bag() const { return _argument_bag; }

    /**
     * Returns the exit code of the application instance.
     */
    int get_exit_code() const { return _exit_code; }



private:

    // #############################################################################
    // Prototypes
    // #############################################################################

    /**
     * Initializes the actual application instance before initializing the framework
     * components. The passed configuration object might be manipulated to influence
     * the following initialization process accordingly. In the event of an error,
     * false is returned and the application exits without continuing initialization
     * let alone entering the main loop.
     */
    virtual bool _pre_init_impl(xl7::Config& config) { return true; }

    /**
     * Initializes the actual application instance after the framework components
     * have (successfully) been initialized. In the event of an error, false is
     * returned and the application exits without even entering the main loop.
     */
    virtual bool _post_init_impl() { return true; }

    /**
     * Shuts down the application instance before the framework components are also
     * shut down. If false is returned, the application will then be terminated in
     * any case, even if it should actually be restarted.
     */
    virtual bool _shutdown_impl() { return true; }

    /**
     * Prepares rendering after the scene has started.
     */
    virtual void _before_render_impl() {}

    /**
     * Performs the actual rendering.
     */
    virtual void _render_impl() {}

    /**
     * Performs follow-up stuff before ending the scene.
     */
    virtual void _after_render_impl() {}

    /**
     * Performs the application logic.
     */
    virtual void _move_impl() {}



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Initializes the application instance including the framework components.
     */
    bool _init();

    /**
     * Shuts down the application instance including the framework components.
     */
    bool _shutdown();

    /**
     * Enters/runs the main message loop of the application instance.
     */
    bool _run_loop();

    /**
     * Does a full application loop iteration.
     * Therefore, it clears the render targets; prepares rendering; begins the
     * scene; performs the actual rendering; ends the scene; presents the rendered
     * scene by flipping the swap chain; and performs the application logic.
     * (See _before_render, _render, _after_render, _present, and _move.)
     */
    void _loop();

    /**
     * Begins the scene and prepares rendering.
     */
    void _before_render();

    /**
     * Performs the actual rendering.
     */
    void _render();

    /**
     * Performs follow-up stuff and ends the scene.
     */
    void _after_render();

    /**
     * Presents the rendered scene by "flipping" the swap chain.
     */
    void _present();

    /**
     * Performs the application logic.
     */
    void _move();



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The argument bag holding a list of command-line arguments.
     */
    ArgumentBag _argument_bag;

    /**
     * The exit code of the application instance.
     */
    int _exit_code = 0;

    /**
     * The flag indicating whether to quit the application.
     */
    bool _quit_flag = false;

    /**
     * The flag indicating whether to restart the application.
     */
    bool _restart_flag = false;

}; // class Application



} // namespace pl7

#endif // PL7_APPLICATION_H

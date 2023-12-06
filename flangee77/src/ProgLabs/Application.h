#pragma once
#ifndef PL7_APPLICATION_H
#define PL7_APPLICATION_H

#include "./ArgumentBag.h"



namespace pl7 {



class Application
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Explicit constructor.
     */
    Application(int argc, cl7::char_type* argv[]);

    /**
     * Destructor.
     */
    virtual ~Application();

private:
    /** Default constructor. */
    Application() = delete;
    /** Copy constructor. */
    Application(const Application&) = delete;
    /** Copy assignment operator. */
    Application& operator = (const Application&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The argument bag holding a list of command-line arguments.
     */
    ArgumentBag _argument_bag;

    /**
     * The exit code of the application instance.
     */
    int _exit_code;

    /**
     * The flag indicating whether to quit the application.
     */
    bool _quit_flag;

    /**
     * The flag indicating whether to restart the application.
     */
    bool _restart_flag;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the argument bag holding a list of command-line arguments.
     */
    const ArgumentBag& get_argument_bag() const { return _argument_bag; }

    /**
     * Returns the exit code of the application instance.
     */
    int get_exit_code() const { return _exit_code; }



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Runs the application instance.
     * Initializes the framework components, runs through the main message loop, and
     * shuts down the framework components.
     */
    bool run();



    // #############################################################################
    // Helpers
    // #############################################################################
private:
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

private:
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

}; // class Application



} // namespace pl7

#endif // PL7_APPLICATION_H

#include "Application.h"

#include <XiaoLabs/MainWindow.h>
#include <XiaoLabs/graphics.h>

#include <CoreLabs/creational/Singleton.h>
#include <CoreLabs/system/MemoryStatus.h>
#include <CoreLabs/logging/FileLogHandler.h>
#include <CoreLabs/logging.h>
#include <CoreLabs/memory.h>



namespace pl7 {


    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Application::Application(int argc, cl7::char_type* argv[])
        : _argument_bag( argc, argv )
        , _exit_code( 0 )
        , _quit_flag( false )
        , _restart_flag( false )
    {
    }

    /**
     * Destructor.
     */
    Application::~Application()
    {
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Runs the application instance.
     * Initializes the framework components, starts the main message loop, and shuts
     * down the framework components.
     */
    bool Application::run()
    {
        bool ok = true;

        while ( ok )
        {
            _quit_flag = false;
            _restart_flag = false;

            ok = _init();
            ok = ok && _run_loop();
            ok = _shutdown() && ok;

            if ( ok && _restart_flag )
                continue;
            break;
        } // while ( ok )

        return ok;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Initializes the application instance including the framework components.
     */
    bool Application::_init()
    {
        // Create/replace log handler.
        cl7::logging::StandardLogger::instance().clear_log_handlers().add_log_handler( std::make_shared<cl7::logging::FileLogHandler>() );

        xl7::Config config;

        // Perform "custom" pre-initialization.
        if ( !_pre_init_impl( config ) )
            return false;

        // Print out the system memory status.
        LOG_TYPE( TEXT("System memory status:"), cl7::logging::LogType::Caption );
        cl7::system::MemoryStatus memory_status;
        if ( !memory_status.capture() )
            LOG_WARNING( TEXT("Unable to retrieve system memory status.") );
        LOG_TYPE( TEXT("Total physical memory\t") + cl7::memory::stringify_byte_amount( memory_status.total_physical_memory ), cl7::logging::LogType::Item );
        LOG_TYPE( TEXT("Available physical memory\t") + cl7::memory::stringify_byte_amount( memory_status.available_physical_memory ), cl7::logging::LogType::Item );

        // Create the main window.
        if ( !xl7::main_window().init( config ) )
            return false;

        // Initialize the graphics system.
        if ( !xl7::graphics::graphics_system().init( config ) )
            return false;

        // Show the main window.
        xl7::main_window().show_window();

        // Perform "custom" post-initialization.
        if ( !_post_init_impl() )
            return false;

        return true;
    }

    /**
     * Shuts down the application instance including the framework components.
     */
    bool Application::_shutdown()
    {
        // Perform "custom" shutdown.
        bool ok = _shutdown_impl();

        // Close the main window.
        xl7::main_window().close();

        // Destroy all singleton objects
        // (also shutting down all "X" components).
        cl7::creational::SingletonManager::destroy_all();

        return ok;
    }

    /**
     * Enters/runs the main message loop of the application instance.
     */
    bool Application::_run_loop()
    {
        while ( true )
        {
            const std::pair<bool, int> quit_flag_and_exit_code = xl7::MainWindow::instance().process_window_messages();

            if ( quit_flag_and_exit_code.first )
            {
                _quit_flag = true;
                _exit_code = quit_flag_and_exit_code.second;
            }

            if ( _quit_flag || _restart_flag )
                break;

            

            // Pause the application logic while the
            // rendering device is lost and not reset.
            if ( xl7::graphics::rendering_device()->check_device_lost() )
            {
                if ( !xl7::graphics::rendering_device()->handle_device_lost() )
                    continue;
            }

            

            // Perform application loop iteration.
            _loop();

            
        } // while ( true )

        return true;
    }

    /**
     * Does a full application loop iteration.
     * Therefore, it clears the render targets; prepares rendering; begins the
     * scene; performs the actual rendering; ends the scene; presents the rendered
     * scene by flipping the swap chain; and performs the application logic.
     * (See _before_render, _render, _after_render, _present, and _move.)
     */
    void Application::_loop()
    {
        // We (first) render the complete scene (without "flipping" the swap chain),
        // (second) perform CPU calculations, and (third) present the scene (now by
        // "flipping" the swap chain). Having CPU calculations between rendering and
        // presentation, we try to make the most of the time the graphics card still
        // has to finish processing.
        _before_render();
        _render();
        _after_render();
        _move();
        _present();
    }

    /**
     * Begins the scene and prepares rendering.
     */
    void Application::_before_render()
    {
        // Update resources etc.?

        //xl7::graphics::rendering_device()->clear_buffers();
        xl7::graphics::primary_context()->begin_scene();

        _before_render_impl();
    }

    /**
     * Performs the actual rendering.
     */
    void Application::_render()
    {
        _render_impl();

        // Draw FPS etc.?
    }

    /**
     * Performs follow-up stuff and ends the scene.
     */
    void Application::_after_render()
    {
        _after_render_impl();

        xl7::graphics::primary_context()->end_scene();
    }

    /**
     * Presents the rendered scene by "flipping" the swap chain.
     */
    void Application::_present()
    {
        xl7::graphics::rendering_device()->present();
    }

    /**
     * Performs the application logic.
     */
    void Application::_move()
    {
        // Update input devices etc.?

        _move_impl();
    }



} // namespace pl7

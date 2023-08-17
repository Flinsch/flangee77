#include "Application.h"

#include <XiaoLabs/Window.h>

#include <CoreLabs/creational/Singleton.h>
#include <CoreLabs/logging/FileLogHandler.h>
#include <CoreLabs/logging.h>



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
    Application::~Application(void)
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

            if ( _restart_flag )
                continue;
            assert( _quit_flag );
            break;
        } // while ( ok )

        if ( !ok )
        {
            return false;
        }

        return true;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Initializes the application instance including the framework components.
     */
    bool Application::_init()
    {
        xl7::Config config;

        // Create/replace log handler.
        cl7::logging::StandardLogger::obj()->clear_log_handlers().add_log_handler( std::make_shared<cl7::logging::FileLogHandler>() );

        // Create the DirextX main window.
        if ( !xl7::Window::obj().init( config ) )
            return false;

        // Show the DirectX main window.
        xl7::Window::obj().show_window();

        return true;
    }

    /**
     * Shuts down the application instance including the framework components.
     */
    bool Application::_shutdown()
    {
        // Close the DirectX main window.
        xl7::Window::obj().close();

        // Destroy all singleton objects
        // (including all DirectX components).
        cl7::creational::SingletonManager::destroy_all();

        return true;
    }

    /**
     * Enters/runs the main message loop of the application instance.
     */
    bool Application::_run_loop()
    {
        while ( true )
        {
            const std::pair<bool, int> quit_flag_and_exit_code = xl7::Window::obj().process_window_messages();

            if ( quit_flag_and_exit_code.first )
            {
                _quit_flag = true;
                _exit_code = quit_flag_and_exit_code.second;
            }

            if ( _quit_flag || _restart_flag )
                break;

            
        } // while ( true )

        return true;
    }



} // namespace pl7

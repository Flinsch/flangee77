#include "Application.h"

#include <XiaoLabs/MainWindow.h>
#include <XiaoLabs/graphics.h>

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

            if ( _restart_flag )
                continue;
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
        cl7::logging::StandardLogger::instance().clear_log_handlers().add_log_handler( std::make_shared<cl7::logging::FileLogHandler>() );

        // Create the main window.
        if ( !xl7::MainWindow::instance().init( config ) )
            return false;

        // Initialize the graphics system.
        if ( !xl7::graphics_system().init( config ) )
            return false;

        // Show the main window.
        xl7::MainWindow::instance().show_window();

        return true;
    }

    /**
     * Shuts down the application instance including the framework components.
     */
    bool Application::_shutdown()
    {
        // Close the main window.
        xl7::MainWindow::instance().close();

        // Destroy all singleton objects
        // (also shutting down all "X" components).
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
            const std::pair<bool, int> quit_flag_and_exit_code = xl7::MainWindow::instance().process_window_messages();

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

#include "Application.h"



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

        

        return ok;
    }


} // namespace pl7

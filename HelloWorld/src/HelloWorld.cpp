
#include "framework.h"

#include <ProgLabs/Application.h>



#ifdef _DEBUG
#pragma warning( disable: 4074 ) // Disable "initializers put in compiler reserved initialization area".
#pragma init_seg( compiler ) // Global objects in this file get constructed very early on.
struct CrtBreakAllocSetter {
    CrtBreakAllocSetter() {
        //_crtBreakAlloc = <request_number>;
    }
} g_crtBreakAllocSetter;
#endif // _DEBUG



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
#ifdef _DEBUG
    _CrtSetDbgFlag( _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG ) | _CRTDBG_LEAK_CHECK_DF );
#endif // _DEBUG

#ifdef _DEBUG
    //_CrtSetBreakAlloc( <request_number> );
#endif // _DEBUG

    std::unique_ptr<pl7::Application> app = std::make_unique<pl7::Application>( __argc, __targv );
    app->run();

    const int exit_code = app->get_exit_code();
    app.reset();

#ifdef _DEBUG
    _CrtDumpMemoryLeaks();
#endif // _DEBUG

    return exit_code;
}

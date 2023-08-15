// HelloWorld.cpp : Definiert den Einstiegspunkt für die Anwendung.
//

#include "framework.h"

#include <ProgLabs/Application.h>



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    std::unique_ptr<pl7::Application> app = std::make_unique<pl7::Application>( __argc, __targv );
    app->run();

    const int exit_code = app->get_exit_code();
    app.reset();

    return exit_code;
}

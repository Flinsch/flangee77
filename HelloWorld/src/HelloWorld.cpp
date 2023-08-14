// HelloWorld.cpp : Definiert den Einstiegspunkt für die Anwendung.
//

#include "framework.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    ::MessageBox( NULL, TEXT("Hello, World!"), TEXT(""), MB_OK);

    return 0;
}

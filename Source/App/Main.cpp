#include "../UI/MainWindow.h"
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int showCommand)
{
    Nyxoryth::MainWindow app;

    if(!app.Initialize(hInstance, showCommand))
        return 1;

    return app.Run();
}

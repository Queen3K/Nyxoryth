#include <windows.h>
#include "ui.h"

int WINAPI WinMain(HINSTANCE h,HINSTANCE,LPSTR,int show)
{
    NyxorythUI app;
    return app.Run(h,show);
}
#pragma once
#include <windows.h>

class NyxorythUI
{
public:
    int Run(HINSTANCE instance,int show);

private:
    static LRESULT CALLBACK WindowProc(HWND,UINT,WPARAM,LPARAM);
};
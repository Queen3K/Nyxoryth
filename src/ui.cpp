#include "ui.h"

LRESULT CALLBACK NyxorythUI::WindowProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp)
{
    switch(msg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps{};
            HDC dc=BeginPaint(hwnd,&ps);

            TextOutW(dc,40,30,L"Nyxoryth Calculator",20);
            TextOutW(dc,40,80,L"Offline Advanced Math System",27);
            TextOutW(dc,40,130,L"Anime UI Engine Active",22);

            EndPaint(hwnd,&ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd,msg,wp,lp);
}


int NyxorythUI::Run(HINSTANCE instance,int show)
{
    WNDCLASS wc{};
    wc.lpfnWndProc=WindowProc;
    wc.hInstance=instance;
    wc.lpszClassName=L"NyxorythCalculator";

    RegisterClass(&wc);

    HWND window=CreateWindowEx(
        0,
        L"NyxorythCalculator",
        L"Nyxoryth Calculator Beta 1.0",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,CW_USEDEFAULT,
        550,720,
        nullptr,nullptr,
        instance,nullptr);

    ShowWindow(window,show);

    MSG msg{};
    while(GetMessage(&msg,nullptr,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
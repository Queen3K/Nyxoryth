#pragma once
#include <windows.h>

namespace Nyxoryth {
class MainWindow {
public:
    bool Create(HINSTANCE h, int show);
    int Run();

private:
    HWND window = nullptr;
    static LRESULT CALLBACK Proc(HWND, UINT, WPARAM, LPARAM);
};
}

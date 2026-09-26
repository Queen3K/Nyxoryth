#include "MainWindow.h"
#include "../Resources/Resource.h"

#include <gdiplus.h>
#include <commdlg.h>
#include <commctrl.h>

#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <cwctype>
#include <iomanip>
#include <limits>
#include <cstring>
#include <sstream>

namespace Nyxoryth
{

namespace
{
    constexpr double kPi = 3.141592653589793238462643383279502884;
    constexpr double kE  = 2.718281828459045235360287471352662498;
    constexpr const wchar_t* kNyxorythVersion = L"1.0.0 RC1";
    constexpr const wchar_t* kButtonHoverProperty = L"NyxorythButtonHover";

    enum ControlId
    {
        BTN_0 = 100,
        BTN_1,
        BTN_2,
        BTN_3,
        BTN_4,
        BTN_5,
        BTN_6,
        BTN_7,
        BTN_8,
        BTN_9,

        BTN_PLUS = 110,
        BTN_MINUS,
        BTN_MULTIPLY,
        BTN_DIVIDE,
        BTN_EQUALS,
        BTN_CLEAR,

        BTN_DECIMAL = 116,
        BTN_BACKSPACE,
        BTN_PERCENT,
        BTN_SIGN,
        BTN_CLEAR_ENTRY,
        BTN_RECIPROCAL,
        BTN_SQUARE,
        BTN_SQRT,
        BTN_HISTORY,
        BTN_CLEAR_HISTORY,

        BTN_MODE = 126,
        BTN_ANGLE,
        BTN_SIN,
        BTN_COS,
        BTN_TAN,
        BTN_ASIN,
        BTN_ACOS,
        BTN_ATAN,
        BTN_LOG10,
        BTN_LN,
        BTN_FACTORIAL,
        BTN_PI,
        BTN_E,
        BTN_POWER,
        BTN_TEN_POWER,
        BTN_E_POWER,
        BTN_ABS,

        BTN_TIP_MODE,
        BTN_PROGRAMMER_MODE,

        BTN_TIP_CALCULATE,
        BTN_TIP_15,
        BTN_TIP_18,
        BTN_TIP_20,
        BTN_TIP_25,

        BTN_PROGRAMMER_CONVERT,
        BTN_BIT_AND,
        BTN_BIT_OR,
        BTN_BIT_XOR,
        BTN_BIT_NOT,
        BTN_SHIFT_LEFT,
        BTN_SHIFT_RIGHT,
        BTN_STORAGE_CONVERT,
        BTN_CIDR_CALCULATE,
        BTN_CHAR_CODE,

        BTN_SETTINGS_MODE,
        BTN_THEME_DARK,
        BTN_THEME_LIGHT,
        BTN_BACKGROUND_CHOOSE,
        BTN_BACKGROUND_CLEAR,
        BTN_BACKGROUND_SCALE,
        BTN_BACKGROUND_OVERLAY,

        BTN_COPY_RESULT,
        BTN_COPY_HISTORY,
        BTN_ALWAYS_ON_TOP,
        BTN_RESET_SETTINGS,
        BTN_ABOUT
    };

    struct ButtonDefinition
    {
        const wchar_t* text;
        int id;
        int x;
        int y;
        int width;
        int height;
    };

    std::wstring GetWindowTextString(HWND control)
    {
        if(!control)
            return L"";

        const int length = GetWindowTextLengthW(control);
        std::wstring text(static_cast<size_t>(length), L'\0');

        if(length > 0)
            GetWindowTextW(control, text.data(), length + 1);

        return text;
    }

    void SetWindowTextString(HWND control, const std::wstring& text)
    {
        if(control)
            SetWindowTextW(control, text.c_str());
    }

    std::wstring Trim(const std::wstring& input)
    {
        size_t first = 0;
        while(first < input.size() && std::iswspace(input[first]))
            ++first;

        size_t last = input.size();
        while(last > first && std::iswspace(input[last - 1]))
            --last;

        return input.substr(first, last - first);
    }

    bool TryParseDouble(const std::wstring& input, double& value)
    {
        try
        {
            const std::wstring text = Trim(input);
            if(text.empty())
                return false;

            size_t used = 0;
            value = std::stod(text, &used);
            return used == text.size() && std::isfinite(value);
        }
        catch(...)
        {
            return false;
        }
    }

    bool TryParseUnsigned(const std::wstring& input, unsigned long long& value)
    {
        try
        {
            std::wstring text = Trim(input);
            if(text.empty())
                return false;

            int base = 10;

            if(text.size() > 2 && text[0] == L'0' && (text[1] == L'x' || text[1] == L'X'))
            {
                base = 16;
                text = text.substr(2);
            }
            else if(text.size() > 2 && text[0] == L'0' && (text[1] == L'b' || text[1] == L'B'))
            {
                base = 2;
                text = text.substr(2);
            }
            else if(text.size() > 2 && text[0] == L'0' && (text[1] == L'o' || text[1] == L'O'))
            {
                base = 8;
                text = text.substr(2);
            }

            if(text.empty())
                return false;

            size_t used = 0;
            value = std::stoull(text, &used, base);
            return used == text.size();
        }
        catch(...)
        {
            return false;
        }
    }

    std::wstring ToBinaryString(unsigned long long value)
    {
        std::string bits = std::bitset<64>(value).to_string();
        const size_t first = bits.find('1');

        if(first == std::string::npos)
            return L"0";

        bits.erase(0, first);
        return std::wstring(bits.begin(), bits.end());
    }

    std::wstring ToUpperHex(unsigned long long value)
    {
        std::wostringstream out;
        out << std::uppercase << std::hex << value;
        return out.str();
    }

    std::wstring ToOctal(unsigned long long value)
    {
        std::wostringstream out;
        out << std::oct << value;
        return out.str();
    }

    bool ParseIpv4(const std::wstring& input, std::uint32_t& address)
    {
        std::wistringstream in(Trim(input));
        unsigned int a = 0, b = 0, c = 0, d = 0;
        wchar_t dot1 = 0, dot2 = 0, dot3 = 0;

        if(!(in >> a >> dot1 >> b >> dot2 >> c >> dot3 >> d))
            return false;

        in >> std::ws;
        if(!in.eof() || dot1 != L'.' || dot2 != L'.' || dot3 != L'.' ||
           a > 255 || b > 255 || c > 255 || d > 255)
        {
            return false;
        }

        address =
            (static_cast<std::uint32_t>(a) << 24) |
            (static_cast<std::uint32_t>(b) << 16) |
            (static_cast<std::uint32_t>(c) << 8) |
             static_cast<std::uint32_t>(d);

        return true;
    }

    std::wstring FormatIpv4(std::uint32_t address)
    {
        std::wostringstream out;
        out << ((address >> 24) & 0xFF) << L'.'
            << ((address >> 16) & 0xFF) << L'.'
            << ((address >> 8) & 0xFF) << L'.'
            << (address & 0xFF);
        return out.str();
    }

    std::wstring FileNameOnly(const std::wstring& path)
    {
        const size_t slash = path.find_last_of(L"\\/");
        return slash == std::wstring::npos ? path : path.substr(slash + 1);
    }

    constexpr UINT_PTR kBackgroundTimerId = 0x4E59;
}

bool MainWindow::Initialize(HINSTANCE instance, int showCommand)
{
    Gdiplus::GdiplusStartupInput startupInput;
    if(Gdiplus::GdiplusStartup(&gdiplusToken, &startupInput, nullptr) != Gdiplus::Ok)
        return false;

    InitializeSettingsPath();
    LoadSettings();

    // Recover automatically if a saved window location belonged to a monitor
    // that is no longer connected.
    if(savedWindowX != CW_USEDEFAULT && savedWindowY != CW_USEDEFAULT)
    {
        POINT savedPoint{savedWindowX + 40, savedWindowY + 40};
        if(MonitorFromPoint(savedPoint, MONITOR_DEFAULTTONULL) == nullptr)
        {
            savedWindowX = CW_USEDEFAULT;
            savedWindowY = CW_USEDEFAULT;
        }
    }

    ApplyTheme();

    WNDCLASSW wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = instance;
    wc.lpszClassName = L"NyxorythWindow";
    wc.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));
    wc.hIcon = LoadIconW(instance, MAKEINTRESOURCEW(IDI_NYXORYTH));
    wc.hbrBackground = nullptr;

    if(!RegisterClassW(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
        return false;

    hwnd = CreateWindowExW(
        0,
        L"NyxorythWindow",
        L"Nyxoryth Calculator",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN,
        savedWindowX,
        savedWindowY,
        500,
        670,
        nullptr,
        nullptr,
        instance,
        this
    );

    if(!hwnd)
        return false;

    if(wc.hIcon)
    {
        SendMessageW(
            hwnd,
            WM_SETICON,
            ICON_SMALL,
            reinterpret_cast<LPARAM>(wc.hIcon));
    }

    ShowWindow(hwnd, showCommand);
    UpdateWindow(hwnd);
    return true;
}

void MainWindow::CreateControls()
{
    modeLabel = CreateWindowExW(
        0, L"STATIC", L"STANDARD",
        WS_CHILD | WS_VISIBLE,
        20, 12, 330, 24,
        hwnd, nullptr, nullptr, nullptr);

    copyResultButton = CreateWindowExW(
        0, L"BUTTON", L"Copy Result",
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        360, 8, 100, 28,
        hwnd,
        reinterpret_cast<HMENU>(static_cast<INT_PTR>(BTN_COPY_RESULT)),
        nullptr, nullptr);
    RegisterButton(copyResultButton);

    expressionDisplay = CreateWindowExW(
        WS_EX_CLIENTEDGE,
        L"STATIC",
        L"",
        WS_CHILD | WS_VISIBLE | SS_RIGHT,
        20, 42, 440, 30,
        hwnd, nullptr, nullptr, nullptr);

    resultDisplay = CreateWindowExW(
        WS_EX_CLIENTEDGE,
        L"STATIC",
        L"0",
        WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
        20, 78, 440, 66,
        hwnd, nullptr, nullptr, nullptr);

    const ButtonDefinition buttons[] =
    {
        {L"%",   BTN_PERCENT,     20, 166, 100, 50},
        {L"CE",  BTN_CLEAR_ENTRY,130, 166, 100, 50},
        {L"C",   BTN_CLEAR,      240, 166, 100, 50},
        {L"⌫",   BTN_BACKSPACE,  350, 166, 100, 50},

        {L"1/x", BTN_RECIPROCAL, 20, 224, 100, 50},
        {L"x²",  BTN_SQUARE,     130, 224, 100, 50},
        {L"√",   BTN_SQRT,       240, 224, 100, 50},
        {L"÷",   BTN_DIVIDE,     350, 224, 100, 50},

        {L"7",   BTN_7,          20, 282, 100, 50},
        {L"8",   BTN_8,          130, 282, 100, 50},
        {L"9",   BTN_9,          240, 282, 100, 50},
        {L"×",   BTN_MULTIPLY,   350, 282, 100, 50},

        {L"4",   BTN_4,          20, 340, 100, 50},
        {L"5",   BTN_5,          130, 340, 100, 50},
        {L"6",   BTN_6,          240, 340, 100, 50},
        {L"−",   BTN_MINUS,      350, 340, 100, 50},

        {L"1",   BTN_1,          20, 398, 100, 50},
        {L"2",   BTN_2,          130, 398, 100, 50},
        {L"3",   BTN_3,          240, 398, 100, 50},
        {L"+",   BTN_PLUS,       350, 398, 100, 50},

        {L"±",   BTN_SIGN,       20, 456, 100, 50},
        {L"0",   BTN_0,          130, 456, 100, 50},
        {L".",   BTN_DECIMAL,    240, 456, 100, 50},
        {L"=",   BTN_EQUALS,     350, 456, 100, 50},

        {L"History",    BTN_HISTORY,          20, 524,  75, 42},
        {L"Scientific", BTN_MODE,            100, 524,  95, 42},
        {L"Tip",        BTN_TIP_MODE,        200, 524,  55, 42},
        {L"Programmer", BTN_PROGRAMMER_MODE, 260, 524, 100, 42},
        {L"Settings",   BTN_SETTINGS_MODE,   365, 524,  85, 42}
    };

    for(const auto& button : buttons)
    {
        HWND handle = CreateWindowExW(
            0,
            L"BUTTON",
            button.text,
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            button.x,
            button.y,
            button.width,
            button.height,
            hwnd,
            reinterpret_cast<HMENU>(static_cast<INT_PTR>(button.id)),
            nullptr,
            nullptr
        );

        RegisterButton(handle);

        if(button.id == BTN_MODE)
            modeButton = handle;
        else if(button.id == BTN_TIP_MODE)
            tipButton = handle;
        else if(button.id == BTN_PROGRAMMER_MODE)
            programmerButton = handle;
        else if(button.id == BTN_SETTINGS_MODE)
            settingsButton = handle;
    }

    CreateScientificControls();
    CreateTipControls();
    CreateProgrammerControls();
    CreateSettingsControls();

    historyList = CreateWindowExW(
        WS_EX_CLIENTEDGE,
        L"LISTBOX",
        L"",
        WS_CHILD | WS_VSCROLL | LBS_NOINTEGRALHEIGHT,
        500, 42, 220, 464,
        hwnd,
        nullptr,
        nullptr,
        nullptr);

    copyHistoryButton = CreateWindowExW(
        0,
        L"BUTTON",
        L"Copy Selected",
        WS_CHILD | BS_OWNERDRAW,
        500, 524, 105, 42,
        hwnd,
        reinterpret_cast<HMENU>(static_cast<INT_PTR>(BTN_COPY_HISTORY)),
        nullptr,
        nullptr);
    RegisterButton(copyHistoryButton);

    clearHistoryButton = CreateWindowExW(
        0,
        L"BUTTON",
        L"Clear History",
        WS_CHILD | BS_OWNERDRAW,
        615, 524, 105, 42,
        hwnd,
        reinterpret_cast<HMENU>(static_cast<INT_PTR>(BTN_CLEAR_HISTORY)),
        nullptr,
        nullptr);
    RegisterButton(clearHistoryButton);

    ApplyStartupMode();
    ApplyFonts();
    UpdateModeText();
    UpdateWindowLayout();
    UpdateDisplay();

    if(!backgroundPath.empty())
    {
        if(!LoadBackgroundImage(backgroundPath))
        {
            backgroundPath.clear();
            SaveSettings();
            UpdateSettingsText();
        }
    }

    if(alwaysOnTop)
    {
        SetWindowPos(
            hwnd, HWND_TOPMOST, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
}

void MainWindow::CreateScientificControls()
{
    const ButtonDefinition buttons[] =
    {
        {L"DEG",   BTN_ANGLE,      500,  42, 290, 42},

        {L"sin",   BTN_SIN,        500,  96, 90, 48},
        {L"cos",   BTN_COS,        600,  96, 90, 48},
        {L"tan",   BTN_TAN,        700,  96, 90, 48},

        {L"asin",  BTN_ASIN,       500, 154, 90, 48},
        {L"acos",  BTN_ACOS,       600, 154, 90, 48},
        {L"atan",  BTN_ATAN,       700, 154, 90, 48},

        {L"log",   BTN_LOG10,      500, 212, 90, 48},
        {L"ln",    BTN_LN,         600, 212, 90, 48},
        {L"n!",    BTN_FACTORIAL,  700, 212, 90, 48},

        {L"π",     BTN_PI,         500, 270, 90, 48},
        {L"e",     BTN_E,          600, 270, 90, 48},
        {L"xʸ",    BTN_POWER,      700, 270, 90, 48},

        {L"10ˣ",   BTN_TEN_POWER,  500, 328, 90, 48},
        {L"eˣ",    BTN_E_POWER,    600, 328, 90, 48},
        {L"|x|",   BTN_ABS,        700, 328, 90, 48}
    };

    for(const auto& button : buttons)
    {
        HWND handle = CreateWindowExW(
            0,
            L"BUTTON",
            button.text,
            WS_CHILD | BS_OWNERDRAW,
            button.x,
            button.y,
            button.width,
            button.height,
            hwnd,
            reinterpret_cast<HMENU>(static_cast<INT_PTR>(button.id)),
            nullptr,
            nullptr
        );

        RegisterButton(handle);
        scientificControls.push_back(handle);

        if(button.id == BTN_ANGLE)
            angleButton = handle;
    }
}


void MainWindow::CreateTipControls()
{
    auto addStatic = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            0, L"STATIC", text,
            WS_CHILD | SS_LEFT,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        tipControls.push_back(control);
        return control;
    };

    auto addEdit = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            WS_EX_CLIENTEDGE, L"EDIT", text,
            WS_CHILD | ES_AUTOHSCROLL | ES_RIGHT,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        tipControls.push_back(control);
        return control;
    };

    auto addButton = [&](const wchar_t* text, int id, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            0, L"BUTTON", text,
            WS_CHILD | BS_OWNERDRAW,
            x, y, width, height,
            hwnd,
            reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
            nullptr, nullptr);
        RegisterButton(control);
        tipControls.push_back(control);
        return control;
    };

    addStatic(L"TIP / BILL SPLIT", 500, 42, 290, 26);

    addStatic(L"Bill amount", 500, 82, 110, 24);
    tipBillEdit = addEdit(L"0.00", 620, 78, 170, 30);

    addStatic(L"Tip %", 500, 122, 110, 24);
    tipPercentEdit = addEdit(L"20", 620, 118, 170, 30);

    addStatic(L"People", 500, 162, 110, 24);
    tipPeopleEdit = addEdit(L"1", 620, 158, 170, 30);

    addStatic(L"Quick tip", 500, 205, 110, 24);
    addButton(L"15%", BTN_TIP_15, 500, 235, 65, 38);
    addButton(L"18%", BTN_TIP_18, 575, 235, 65, 38);
    addButton(L"20%", BTN_TIP_20, 650, 235, 65, 38);
    addButton(L"25%", BTN_TIP_25, 725, 235, 65, 38);

    addButton(L"Calculate", BTN_TIP_CALCULATE, 500, 288, 290, 42);

    addStatic(L"Tip amount", 500, 354, 115, 24);
    tipAmountResult = addStatic(L"$0.00", 620, 354, 170, 24);

    addStatic(L"Total", 500, 392, 115, 24);
    tipTotalResult = addStatic(L"$0.00", 620, 392, 170, 24);

    addStatic(L"Per person", 500, 430, 115, 24);
    tipPerPersonResult = addStatic(L"$0.00", 620, 430, 170, 24);

    addStatic(L"Tip calculations stay local and are not sent anywhere.", 500, 488, 290, 44);
}

void MainWindow::CreateProgrammerControls()
{
    auto addStatic = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            0, L"STATIC", text,
            WS_CHILD | SS_LEFT,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        programmerControls.push_back(control);
        return control;
    };

    auto addEdit = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            WS_EX_CLIENTEDGE, L"EDIT", text,
            WS_CHILD | ES_AUTOHSCROLL,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        programmerControls.push_back(control);
        return control;
    };

    auto addButton = [&](const wchar_t* text, int id, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            0, L"BUTTON", text,
            WS_CHILD | BS_OWNERDRAW,
            x, y, width, height,
            hwnd,
            reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
            nullptr, nullptr);
        RegisterButton(control);
        programmerControls.push_back(control);
        return control;
    };

    addStatic(L"PROGRAMMER / COMPUTER TOOLS", 500, 38, 290, 24);

    addStatic(L"Integer (dec, 0x, 0b, 0o)", 500, 66, 180, 22);
    programmerValueEdit = addEdit(L"42", 500, 88, 205, 28);
    addButton(L"Convert", BTN_PROGRAMMER_CONVERT, 713, 88, 77, 28);

    programmerDecResult = addStatic(L"DEC: 42", 500, 120, 290, 20);
    programmerHexResult = addStatic(L"HEX: 2A", 500, 140, 290, 20);
    programmerOctResult = addStatic(L"OCT: 52", 500, 160, 290, 20);
    programmerBinResult = addStatic(L"BIN: 101010", 500, 180, 290, 38);

    addStatic(L"Bitwise A / B", 500, 222, 120, 20);
    bitwiseAEdit = addEdit(L"12", 500, 244, 137, 28);
    bitwiseBEdit = addEdit(L"10", 653, 244, 137, 28);

    addButton(L"AND", BTN_BIT_AND, 500, 278, 52, 28);
    addButton(L"OR",  BTN_BIT_OR, 558, 278, 52, 28);
    addButton(L"XOR", BTN_BIT_XOR, 616, 278, 52, 28);
    addButton(L"NOT", BTN_BIT_NOT, 674, 278, 52, 28);
    addButton(L"<<",  BTN_SHIFT_LEFT, 732, 278, 28, 28);
    addButton(L">>",  BTN_SHIFT_RIGHT, 762, 278, 28, 28);
    bitwiseResult = addStatic(L"Result: —", 500, 310, 290, 22);

    addStatic(L"Storage bytes", 500, 338, 105, 20);
    storageBytesEdit = addEdit(L"1073741824", 610, 334, 105, 28);
    addButton(L"Convert", BTN_STORAGE_CONVERT, 721, 334, 69, 28);
    storageResult = addStatic(L"1 GiB", 500, 366, 290, 42);

    addStatic(L"IPv4 / CIDR", 500, 412, 90, 20);
    cidrIpEdit = addEdit(L"192.168.1.10", 500, 434, 165, 28);
    cidrPrefixEdit = addEdit(L"24", 671, 434, 42, 28);
    addButton(L"Calc", BTN_CIDR_CALCULATE, 719, 434, 71, 28);
    cidrResult = addStatic(L"Network / mask / broadcast / addresses", 500, 466, 290, 66);

    addStatic(L"Character", 500, 542, 75, 20);
    charCodeEdit = addEdit(L"A", 578, 538, 70, 28);
    addButton(L"Code", BTN_CHAR_CODE, 654, 538, 60, 28);
    charCodeResult = addStatic(L"U+0041 / 65 / 0x41", 500, 572, 290, 24);
}


void MainWindow::CreateSettingsControls()
{
    auto addStatic = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            0, L"STATIC", text,
            WS_CHILD | SS_LEFT,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        settingsControls.push_back(control);
        return control;
    };

    auto addButton = [&](const wchar_t* text, int id, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            0, L"BUTTON", text,
            WS_CHILD | BS_OWNERDRAW,
            x, y, width, height,
            hwnd,
            reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
            nullptr, nullptr);
        RegisterButton(control);
        settingsControls.push_back(control);
        return control;
    };

    addStatic(L"SETTINGS / APPEARANCE", 500, 42, 290, 26);

    addStatic(L"Theme", 500, 86, 90, 24);
    addButton(L"Dark", BTN_THEME_DARK, 500, 116, 135, 40);
    addButton(L"Light", BTN_THEME_LIGHT, 655, 116, 135, 40);

    addStatic(L"Local background", 500, 182, 150, 24);
    addButton(L"Choose image / GIF", BTN_BACKGROUND_CHOOSE, 500, 212, 185, 40);
    addButton(L"Clear", BTN_BACKGROUND_CLEAR, 695, 212, 95, 40);

    backgroundStatus = addStatic(L"No background selected", 500, 264, 290, 44);

    scaleModeButton = addButton(L"Scale: Cover", BTN_BACKGROUND_SCALE, 500, 326, 135, 40);
    overlayButton = addButton(L"Overlay: 45%", BTN_BACKGROUND_OVERLAY, 655, 326, 135, 40);

    addStatic(
        L"Supported local files: PNG, JPG/JPEG, GIF, BMP. "
        L"Files stay on your computer and are never bundled with Nyxoryth.",
        500, 390, 290, 72);

    alwaysOnTopButton = addButton(
        L"Always on top: Off", BTN_ALWAYS_ON_TOP, 500, 458, 290, 38);

    resetSettingsButton = addButton(
        L"Reset Settings", BTN_RESET_SETTINGS, 500, 506, 290, 38);

    aboutButton = addButton(
        L"About Nyxoryth", BTN_ABOUT, 500, 554, 290, 38);

    addStatic(L"MIT License • Local / offline calculator", 500, 604, 290, 22);

    UpdateSettingsText();
}


void MainWindow::RegisterButton(HWND button)
{
    if(!button)
        return;

    allButtons.push_back(button);
    SetWindowSubclass(
        button,
        ButtonSubclassProc,
        1,
        reinterpret_cast<DWORD_PTR>(this));
}

void MainWindow::DrawOwnerButton(const DRAWITEMSTRUCT* drawItem)
{
    if(!drawItem || drawItem->CtlType != ODT_BUTTON)
        return;

    RECT rect = drawItem->rcItem;
    const bool pressed = (drawItem->itemState & ODS_SELECTED) != 0;
    const bool disabled = (drawItem->itemState & ODS_DISABLED) != 0;
    const bool focused = (drawItem->itemState & ODS_FOCUS) != 0;
    const bool hovered =
        GetPropW(drawItem->hwndItem, kButtonHoverProperty) != nullptr;

    const int id = GetDlgCtrlID(drawItem->hwndItem);

    const bool activeState =
        (id == BTN_HISTORY && historyVisible) ||
        (id == BTN_MODE && scientificMode) ||
        (id == BTN_TIP_MODE && tipMode) ||
        (id == BTN_PROGRAMMER_MODE && programmerMode) ||
        (id == BTN_SETTINGS_MODE && settingsMode) ||
        (id == BTN_THEME_DARK && darkTheme) ||
        (id == BTN_THEME_LIGHT && !darkTheme) ||
        (id == BTN_ALWAYS_ON_TOP && alwaysOnTop);

    const bool accent =
        activeState ||
        id == BTN_EQUALS ||
        id == BTN_TIP_CALCULATE ||
        id == BTN_PROGRAMMER_CONVERT ||
        id == BTN_CIDR_CALCULATE ||
        id == BTN_BACKGROUND_CHOOSE;

    const bool operation =
        id == BTN_PLUS || id == BTN_MINUS ||
        id == BTN_MULTIPLY || id == BTN_DIVIDE ||
        id == BTN_POWER;

    const bool navigation =
        id == BTN_HISTORY || id == BTN_MODE ||
        id == BTN_TIP_MODE || id == BTN_PROGRAMMER_MODE ||
        id == BTN_SETTINGS_MODE || id == BTN_COPY_RESULT ||
        id == BTN_COPY_HISTORY || id == BTN_ALWAYS_ON_TOP ||
        id == BTN_RESET_SETTINGS || id == BTN_ABOUT;

    COLORREF fill{};
    COLORREF border{};
    COLORREF textColor{};

    if(darkTheme)
    {
        fill = RGB(39, 40, 54);
        border = RGB(82, 84, 112);
        textColor = RGB(244, 244, 252);

        if(navigation)
        {
            fill = RGB(31, 33, 46);
            border = RGB(92, 94, 126);
        }

        if(operation)
        {
            fill = RGB(63, 49, 92);
            border = RGB(132, 102, 190);
        }

        if(accent)
        {
            fill = RGB(92, 62, 152);
            border = RGB(176, 134, 242);
        }

        if(hovered)
        {
            fill = accent
                ? RGB(112, 76, 181)
                : (operation ? RGB(78, 60, 112) : RGB(52, 54, 72));
        }
    }
    else
    {
        // Explicit light palette. This is intentionally owner-drawn so Windows'
        // system dark theme cannot leave calculator buttons as dark boxes.
        fill = RGB(255, 255, 255);
        border = RGB(202, 202, 216);
        textColor = RGB(28, 28, 36);

        if(navigation)
        {
            fill = RGB(248, 248, 252);
            border = RGB(190, 190, 208);
        }

        if(operation)
        {
            fill = RGB(239, 232, 252);
            border = RGB(174, 151, 214);
        }

        if(accent)
        {
            fill = RGB(224, 211, 248);
            border = RGB(151, 116, 208);
        }

        if(hovered)
        {
            fill = accent
                ? RGB(212, 194, 244)
                : (operation ? RGB(231, 220, 248) : RGB(241, 241, 247));
        }
    }

    if(pressed)
    {
        fill = darkTheme
            ? RGB(
                std::max(0, static_cast<int>(GetRValue(fill)) - 10),
                std::max(0, static_cast<int>(GetGValue(fill)) - 10),
                std::max(0, static_cast<int>(GetBValue(fill)) - 10))
            : RGB(
                std::max(0, static_cast<int>(GetRValue(fill)) - 8),
                std::max(0, static_cast<int>(GetGValue(fill)) - 8),
                std::max(0, static_cast<int>(GetBValue(fill)) - 8));
        OffsetRect(&rect, 0, 1);
    }

    if(disabled)
    {
        fill = darkTheme ? RGB(34, 34, 42) : RGB(239, 239, 243);
        textColor = darkTheme ? RGB(112, 112, 124) : RGB(150, 150, 158);
    }

    // The button is a child window and the parent uses WS_CLIPCHILDREN to
    // prevent animated GIF repaint flicker. That means Windows will not paint
    // the area behind this control for us. Clear the full button client area
    // first so the corners outside the rounded rectangle never retain the
    // system/default black background in Light mode.
    if(themeBackgroundBrush)
        FillRect(drawItem->hDC, &rect, themeBackgroundBrush);
    else
    {
        HBRUSH fallbackBackground = CreateSolidBrush(
            darkTheme ? RGB(22, 22, 30) : RGB(250, 250, 253));
        FillRect(drawItem->hDC, &rect, fallbackBackground);
        DeleteObject(fallbackBackground);
    }

    HBRUSH brush = CreateSolidBrush(fill);
    HPEN pen = CreatePen(PS_SOLID, 1, border);

    HGDIOBJ oldBrush = SelectObject(drawItem->hDC, brush);
    HGDIOBJ oldPen = SelectObject(drawItem->hDC, pen);

    RoundRect(
        drawItem->hDC,
        rect.left + 1,
        rect.top + 1,
        rect.right - 1,
        rect.bottom - 1,
        12,
        12);

    SelectObject(drawItem->hDC, oldBrush);
    SelectObject(drawItem->hDC, oldPen);
    DeleteObject(brush);
    DeleteObject(pen);

    wchar_t text[256]{};
    GetWindowTextW(drawItem->hwndItem, text, 256);

    SetBkMode(drawItem->hDC, TRANSPARENT);
    SetTextColor(drawItem->hDC, textColor);

    HFONT font = buttonFont
        ? buttonFont
        : reinterpret_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT));

    HGDIOBJ oldFont = SelectObject(drawItem->hDC, font);

    RECT textRect = rect;
    DrawTextW(
        drawItem->hDC,
        text,
        -1,
        &textRect,
        DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

    SelectObject(drawItem->hDC, oldFont);

    if(focused)
    {
        RECT focusRect = rect;
        InflateRect(&focusRect, -5, -5);
        DrawFocusRect(drawItem->hDC, &focusRect);
    }
}

LRESULT CALLBACK MainWindow::ButtonSubclassProc(
    HWND button,
    UINT message,
    WPARAM wParam,
    LPARAM lParam,
    UINT_PTR subclassId,
    DWORD_PTR referenceData)
{
    (void)subclassId;
    (void)referenceData;

    switch(message)
    {
        case WM_MOUSEMOVE:
        {
            if(GetPropW(button, kButtonHoverProperty) == nullptr)
            {
                SetPropW(button, kButtonHoverProperty, reinterpret_cast<HANDLE>(1));

                TRACKMOUSEEVENT track{};
                track.cbSize = sizeof(track);
                track.dwFlags = TME_LEAVE;
                track.hwndTrack = button;
                TrackMouseEvent(&track);

                InvalidateRect(button, nullptr, FALSE);
            }
            break;
        }

        case WM_MOUSELEAVE:
            RemovePropW(button, kButtonHoverProperty);
            InvalidateRect(button, nullptr, FALSE);
            break;

        case WM_SETFOCUS:
        case WM_KILLFOCUS:
            InvalidateRect(button, nullptr, FALSE);
            break;

        case WM_NCDESTROY:
            RemovePropW(button, kButtonHoverProperty);
            RemoveWindowSubclass(button, ButtonSubclassProc, 1);
            break;
    }

    return DefSubclassProc(button, message, wParam, lParam);
}

void MainWindow::ApplyFonts()
{
    expressionFont = CreateFontW(
        17, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Bahnschrift");

    resultFont = CreateFontW(
        36, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Bahnschrift SemiBold");

    buttonFont = CreateFontW(
        18, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Bahnschrift SemiBold");

    if(expressionDisplay)
        SendMessageW(expressionDisplay, WM_SETFONT, reinterpret_cast<WPARAM>(expressionFont), TRUE);

    if(resultDisplay)
        SendMessageW(resultDisplay, WM_SETFONT, reinterpret_cast<WPARAM>(resultFont), TRUE);

    if(modeLabel)
        SendMessageW(modeLabel, WM_SETFONT, reinterpret_cast<WPARAM>(expressionFont), TRUE);

    EnumChildWindows(
        hwnd,
        [](HWND child, LPARAM param) -> BOOL
        {
            MainWindow* app = reinterpret_cast<MainWindow*>(param);
            wchar_t className[32]{};
            GetClassNameW(child, className, 32);

            if(lstrcmpiW(className, L"BUTTON") == 0)
            {
                SendMessageW(
                    child,
                    WM_SETFONT,
                    reinterpret_cast<WPARAM>(app->buttonFont),
                    TRUE);
            }
            else if(
                child != app->resultDisplay &&
                child != app->expressionDisplay &&
                (lstrcmpiW(className, L"STATIC") == 0 ||
                 lstrcmpiW(className, L"EDIT") == 0 ||
                 lstrcmpiW(className, L"LISTBOX") == 0))
            {
                SendMessageW(
                    child,
                    WM_SETFONT,
                    reinterpret_cast<WPARAM>(app->expressionFont),
                    TRUE);
            }

            return TRUE;
        },
        reinterpret_cast<LPARAM>(this));
}

void MainWindow::InputDigit(wchar_t digit)
{
    if(errorState)
        ClearAll();

    if(justEvaluated && pendingOperator == 0)
    {
        currentInput = L"0";
        expression.clear();
        justEvaluated = false;
    }

    if(waitingForOperand || currentInput == L"0")
    {
        currentInput.assign(1, digit);
        waitingForOperand = false;
    }
    else if(currentInput.size() < 24)
    {
        currentInput.push_back(digit);
    }

    UpdateExpressionForCurrentInput();
    UpdateDisplay();
}

void MainWindow::InputDecimal()
{
    if(errorState)
        ClearAll();

    if(justEvaluated && pendingOperator == 0)
    {
        currentInput = L"0";
        expression.clear();
        justEvaluated = false;
    }

    if(waitingForOperand)
    {
        currentInput = L"0.";
        waitingForOperand = false;
    }
    else if(currentInput.find(L'.') == std::wstring::npos &&
            currentInput.find(L'e') == std::wstring::npos &&
            currentInput.find(L'E') == std::wstring::npos)
    {
        currentInput += L".";
    }

    UpdateExpressionForCurrentInput();
    UpdateDisplay();
}

void MainWindow::SetOperator(wchar_t op)
{
    if(errorState)
        return;

    if(pendingOperator != 0 && waitingForOperand)
    {
        pendingOperator = op;
        expression = FormatNumber(accumulator) + L" " + OperatorText(op);
        UpdateDisplay();
        return;
    }

    const double current = CurrentValue();

    if(pendingOperator != 0 && !waitingForOperand)
    {
        double chainedResult = 0.0;

        if(!ApplyBinary(accumulator, current, pendingOperator, chainedResult))
            return;

        accumulator = chainedResult;
        currentInput = FormatNumber(chainedResult);
    }
    else
    {
        accumulator = current;
    }

    pendingOperator = op;
    waitingForOperand = true;
    justEvaluated = false;
    expression = FormatNumber(accumulator) + L" " + OperatorText(op);
    UpdateDisplay();
}

void MainWindow::Evaluate()
{
    if(errorState || pendingOperator == 0 || waitingForOperand)
        return;

    const double right = CurrentValue();
    const double left = accumulator;
    double result = 0.0;

    if(!ApplyBinary(left, right, pendingOperator, result))
        return;

    const std::wstring completedExpression =
        FormatNumber(left) + L" " + OperatorText(pendingOperator) + L" " +
        FormatNumber(right) + L" =";

    currentInput = FormatNumber(result);
    expression = completedExpression;
    AddHistory(completedExpression + L"  " + currentInput);

    accumulator = result;
    pendingOperator = 0;
    waitingForOperand = true;
    justEvaluated = true;
    UpdateDisplay();
}

void MainWindow::ClearAll()
{
    accumulator = 0.0;
    pendingOperator = 0;
    currentInput = L"0";
    expression.clear();
    waitingForOperand = false;
    justEvaluated = false;
    errorState = false;
    UpdateDisplay();
}

void MainWindow::ClearEntry()
{
    if(errorState)
    {
        ClearAll();
        return;
    }

    currentInput = L"0";
    waitingForOperand = false;
    justEvaluated = false;
    UpdateExpressionForCurrentInput();
    UpdateDisplay();
}

void MainWindow::Backspace()
{
    if(errorState || waitingForOperand || justEvaluated)
        return;

    if(currentInput.size() <= 1 ||
       (currentInput.size() == 2 && currentInput.front() == L'-'))
    {
        currentInput = L"0";
    }
    else
    {
        currentInput.pop_back();
    }

    UpdateExpressionForCurrentInput();
    UpdateDisplay();
}

void MainWindow::ToggleSign()
{
    if(errorState || currentInput == L"0")
        return;

    if(currentInput.front() == L'-')
        currentInput.erase(currentInput.begin());
    else
        currentInput.insert(currentInput.begin(), L'-');

    UpdateExpressionForCurrentInput();
    UpdateDisplay();
}

void MainWindow::Percent()
{
    if(errorState)
        return;

    double current = CurrentValue();

    if(pendingOperator != 0)
        current = accumulator * current / 100.0;
    else
        current /= 100.0;

    currentInput = FormatNumber(current);
    waitingForOperand = false;
    justEvaluated = false;
    UpdateExpressionForCurrentInput();
    UpdateDisplay();
}

void MainWindow::SquareRoot()
{
    if(errorState)
        return;

    const double current = CurrentValue();

    if(current < 0.0)
    {
        SetError(L"Invalid input");
        return;
    }

    ApplyScientificUnary(std::sqrt(current), L"√(" + FormatNumber(current) + L")");
}

void MainWindow::Square()
{
    if(errorState)
        return;

    const double current = CurrentValue();
    ApplyScientificUnary(current * current, L"sqr(" + FormatNumber(current) + L")");
}

void MainWindow::Reciprocal()
{
    if(errorState)
        return;

    const double current = CurrentValue();

    if(current == 0.0)
    {
        SetError(L"Cannot divide by zero");
        return;
    }

    ApplyScientificUnary(1.0 / current, L"1/(" + FormatNumber(current) + L")");
}

void MainWindow::ToggleScientificMode()
{
    const bool opening = !scientificMode;
    scientificMode = opening;
    if(opening)
    {
        tipMode = false;
        programmerMode = false;
        settingsMode = false;
    }

    lastMode = scientificMode ? L"Scientific" : L"Standard";
    SaveSettings();
    UpdateModeText();
    UpdateWindowLayout();
}

void MainWindow::ToggleTipMode()
{
    const bool opening = !tipMode;
    tipMode = opening;
    if(opening)
    {
        scientificMode = false;
        programmerMode = false;
        settingsMode = false;
    }

    lastMode = tipMode ? L"Tip" : L"Standard";
    SaveSettings();
    UpdateModeText();
    UpdateWindowLayout();
}

void MainWindow::ToggleProgrammerMode()
{
    const bool opening = !programmerMode;
    programmerMode = opening;
    if(opening)
    {
        scientificMode = false;
        tipMode = false;
        settingsMode = false;
    }

    lastMode = programmerMode ? L"Programmer" : L"Standard";
    SaveSettings();
    UpdateModeText();
    UpdateWindowLayout();
}

void MainWindow::ToggleAngleMode()
{
    degreeMode = !degreeMode;
    UpdateModeText();
}

void MainWindow::Sine()
{
    if(errorState) return;
    const double value = CurrentValue();
    const double radians = degreeMode ? value * kPi / 180.0 : value;
    ApplyScientificUnary(std::sin(radians), L"sin(" + FormatNumber(value) + L")");
}

void MainWindow::Cosine()
{
    if(errorState) return;
    const double value = CurrentValue();
    const double radians = degreeMode ? value * kPi / 180.0 : value;
    ApplyScientificUnary(std::cos(radians), L"cos(" + FormatNumber(value) + L")");
}

void MainWindow::Tangent()
{
    if(errorState) return;
    const double value = CurrentValue();
    const double radians = degreeMode ? value * kPi / 180.0 : value;
    const double cosine = std::cos(radians);

    if(std::fabs(cosine) < 1e-14)
    {
        SetError(L"Undefined");
        return;
    }

    ApplyScientificUnary(std::tan(radians), L"tan(" + FormatNumber(value) + L")");
}

void MainWindow::ArcSine()
{
    if(errorState) return;
    const double value = CurrentValue();

    if(value < -1.0 || value > 1.0)
    {
        SetError(L"Invalid input");
        return;
    }

    double result = std::asin(value);
    if(degreeMode) result = result * 180.0 / kPi;
    ApplyScientificUnary(result, L"asin(" + FormatNumber(value) + L")");
}

void MainWindow::ArcCosine()
{
    if(errorState) return;
    const double value = CurrentValue();

    if(value < -1.0 || value > 1.0)
    {
        SetError(L"Invalid input");
        return;
    }

    double result = std::acos(value);
    if(degreeMode) result = result * 180.0 / kPi;
    ApplyScientificUnary(result, L"acos(" + FormatNumber(value) + L")");
}

void MainWindow::ArcTangent()
{
    if(errorState) return;
    const double value = CurrentValue();
    double result = std::atan(value);
    if(degreeMode) result = result * 180.0 / kPi;
    ApplyScientificUnary(result, L"atan(" + FormatNumber(value) + L")");
}

void MainWindow::Log10()
{
    if(errorState) return;
    const double value = CurrentValue();

    if(value <= 0.0)
    {
        SetError(L"Invalid input");
        return;
    }

    ApplyScientificUnary(std::log10(value), L"log(" + FormatNumber(value) + L")");
}

void MainWindow::NaturalLog()
{
    if(errorState) return;
    const double value = CurrentValue();

    if(value <= 0.0)
    {
        SetError(L"Invalid input");
        return;
    }

    ApplyScientificUnary(std::log(value), L"ln(" + FormatNumber(value) + L")");
}

void MainWindow::Factorial()
{
    if(errorState) return;
    const double value = CurrentValue();
    const double rounded = std::round(value);

    if(value < 0.0 || std::fabs(value - rounded) > 1e-12 || rounded > 170.0)
    {
        SetError(L"Invalid factorial");
        return;
    }

    double result = 1.0;
    for(int i = 2; i <= static_cast<int>(rounded); ++i)
        result *= static_cast<double>(i);

    ApplyScientificUnary(result, FormatNumber(value) + L"!");
}

void MainWindow::TenPower()
{
    if(errorState) return;
    const double value = CurrentValue();
    ApplyScientificUnary(std::pow(10.0, value), L"10^(" + FormatNumber(value) + L")");
}

void MainWindow::EPower()
{
    if(errorState) return;
    const double value = CurrentValue();
    ApplyScientificUnary(std::exp(value), L"e^(" + FormatNumber(value) + L")");
}

void MainWindow::AbsoluteValue()
{
    if(errorState) return;
    const double value = CurrentValue();
    ApplyScientificUnary(std::fabs(value), L"abs(" + FormatNumber(value) + L")");
}

void MainWindow::InsertPi()
{
    if(errorState) ClearAll();
    currentInput = FormatNumber(kPi);
    waitingForOperand = false;
    justEvaluated = false;
    UpdateExpressionForCurrentInput();
    UpdateDisplay();
}

void MainWindow::InsertE()
{
    if(errorState) ClearAll();
    currentInput = FormatNumber(kE);
    waitingForOperand = false;
    justEvaluated = false;
    UpdateExpressionForCurrentInput();
    UpdateDisplay();
}

void MainWindow::ApplyScientificUnary(double result, const std::wstring& label)
{
    if(!std::isfinite(result))
    {
        SetError(L"Result overflow");
        return;
    }

    currentInput = FormatNumber(result);
    waitingForOperand = false;
    errorState = false;

    if(pendingOperator != 0)
    {
        justEvaluated = false;
        UpdateExpressionForCurrentInput();
    }
    else
    {
        expression = label;
        justEvaluated = true;
        AddHistory(label + L" =  " + currentInput);
    }

    UpdateDisplay();
}



void MainWindow::InitializeSettingsPath()
{
    wchar_t localAppData[32768]{};
    const DWORD length = GetEnvironmentVariableW(
        L"LOCALAPPDATA",
        localAppData,
        static_cast<DWORD>(sizeof(localAppData) / sizeof(localAppData[0])));

    if(length > 0 && length < (sizeof(localAppData) / sizeof(localAppData[0])))
    {
        std::wstring directory = std::wstring(localAppData) + L"\\Nyxoryth";
        CreateDirectoryW(directory.c_str(), nullptr);
        settingsPath = directory + L"\\settings.ini";
    }
    else
    {
        settingsPath = L"Nyxoryth_settings.ini";
    }
}

void MainWindow::LoadSettings()
{
    if(settingsPath.empty())
        return;

    darkTheme = GetPrivateProfileIntW(
        L"Appearance", L"DarkTheme", 1, settingsPath.c_str()) != 0;

    backgroundCover = GetPrivateProfileIntW(
        L"Appearance", L"BackgroundCover", 1, settingsPath.c_str()) != 0;

    backgroundOverlayPercent = GetPrivateProfileIntW(
        L"Appearance", L"OverlayPercent", 45, settingsPath.c_str());

    if(backgroundOverlayPercent < 0) backgroundOverlayPercent = 0;
    if(backgroundOverlayPercent > 75) backgroundOverlayPercent = 75;

    wchar_t pathBuffer[32768]{};
    GetPrivateProfileStringW(
        L"Appearance",
        L"BackgroundPath",
        L"",
        pathBuffer,
        static_cast<DWORD>(sizeof(pathBuffer) / sizeof(pathBuffer[0])),
        settingsPath.c_str());

    backgroundPath = pathBuffer;

    alwaysOnTop = GetPrivateProfileIntW(
        L"General", L"AlwaysOnTop", 0, settingsPath.c_str()) != 0;

    savedWindowX = GetPrivateProfileIntW(
        L"General", L"WindowX", CW_USEDEFAULT, settingsPath.c_str());

    savedWindowY = GetPrivateProfileIntW(
        L"General", L"WindowY", CW_USEDEFAULT, settingsPath.c_str());

    wchar_t modeBuffer[64]{};
    GetPrivateProfileStringW(
        L"General",
        L"LastMode",
        L"Standard",
        modeBuffer,
        static_cast<DWORD>(sizeof(modeBuffer) / sizeof(modeBuffer[0])),
        settingsPath.c_str());

    lastMode = modeBuffer;
}

void MainWindow::SaveSettings()
{
    if(settingsPath.empty())
        return;

    WritePrivateProfileStringW(
        L"Appearance",
        L"DarkTheme",
        darkTheme ? L"1" : L"0",
        settingsPath.c_str());

    WritePrivateProfileStringW(
        L"Appearance",
        L"BackgroundCover",
        backgroundCover ? L"1" : L"0",
        settingsPath.c_str());

    const std::wstring overlay = std::to_wstring(backgroundOverlayPercent);
    WritePrivateProfileStringW(
        L"Appearance",
        L"OverlayPercent",
        overlay.c_str(),
        settingsPath.c_str());

    WritePrivateProfileStringW(
        L"Appearance",
        L"BackgroundPath",
        backgroundPath.c_str(),
        settingsPath.c_str());

    WritePrivateProfileStringW(
        L"General",
        L"AlwaysOnTop",
        alwaysOnTop ? L"1" : L"0",
        settingsPath.c_str());

    WritePrivateProfileStringW(
        L"General",
        L"LastMode",
        lastMode.c_str(),
        settingsPath.c_str());

    if(savedWindowX != CW_USEDEFAULT)
    {
        const std::wstring x = std::to_wstring(savedWindowX);
        WritePrivateProfileStringW(
            L"General", L"WindowX", x.c_str(), settingsPath.c_str());
    }

    if(savedWindowY != CW_USEDEFAULT)
    {
        const std::wstring y = std::to_wstring(savedWindowY);
        WritePrivateProfileStringW(
            L"General", L"WindowY", y.c_str(), settingsPath.c_str());
    }
}

void MainWindow::ApplyTheme()
{
    if(themeBackgroundBrush)
    {
        DeleteObject(themeBackgroundBrush);
        themeBackgroundBrush = nullptr;
    }

    if(themeEditBrush)
    {
        DeleteObject(themeEditBrush);
        themeEditBrush = nullptr;
    }

    const COLORREF background =
        darkTheme ? RGB(22, 22, 30) : RGB(250, 250, 253);

    const COLORREF edit =
        darkTheme ? RGB(36, 36, 48) : RGB(255, 255, 255);

    themeBackgroundBrush = CreateSolidBrush(background);
    themeEditBrush = CreateSolidBrush(edit);

    if(hwnd)
    {
        RedrawWindow(
            hwnd,
            nullptr,
            nullptr,
            RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN | RDW_UPDATENOW);
    }

    UpdateSettingsText();
}

void MainWindow::ChooseBackground()
{
    wchar_t fileBuffer[32768]{};

    OPENFILENAMEW dialog{};
    dialog.lStructSize = sizeof(dialog);
    dialog.hwndOwner = hwnd;
    dialog.lpstrFile = fileBuffer;
    dialog.nMaxFile = static_cast<DWORD>(sizeof(fileBuffer) / sizeof(fileBuffer[0]));
    dialog.lpstrFilter =
        L"Image files (*.png;*.jpg;*.jpeg;*.gif;*.bmp)\0"
        L"*.png;*.jpg;*.jpeg;*.gif;*.bmp\0"
        L"All files (*.*)\0"
        L"*.*\0\0";
    dialog.nFilterIndex = 1;
    dialog.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER;

    if(GetOpenFileNameW(&dialog))
    {
        if(LoadBackgroundImage(fileBuffer))
        {
            backgroundPath = fileBuffer;
            SaveSettings();
            UpdateSettingsText();
        }
        else
        {
            MessageBoxW(
                hwnd,
                L"Nyxoryth could not load that image. Try a PNG, JPG/JPEG, GIF, or BMP file.",
                L"Background",
                MB_OK | MB_ICONWARNING);
        }
    }
}

void MainWindow::ClearBackground()
{
    backgroundPath.clear();
    ReleaseBackgroundImage();
    SaveSettings();
    UpdateSettingsText();

    if(hwnd)
        InvalidateRect(hwnd, nullptr, TRUE);
}

bool MainWindow::LoadBackgroundImage(const std::wstring& path)
{
    ReleaseBackgroundImage();

    if(path.empty() || gdiplusToken == 0)
        return false;

    Gdiplus::Image* image = Gdiplus::Image::FromFile(path.c_str(), FALSE);
    if(!image || image->GetLastStatus() != Gdiplus::Ok)
    {
        delete image;
        return false;
    }

    backgroundImage = image;
    backgroundFrameCount = 1;
    backgroundFrameIndex = 0;
    backgroundFrameDelays.clear();

    const UINT dimensionCount = backgroundImage->GetFrameDimensionsCount();

    if(dimensionCount > 0)
    {
        std::vector<GUID> dimensions(dimensionCount);
        if(backgroundImage->GetFrameDimensionsList(
               dimensions.data(),
               dimensionCount) == Gdiplus::Ok)
        {
            backgroundFrameDimension = dimensions[0];
            backgroundFrameCount =
                backgroundImage->GetFrameCount(&backgroundFrameDimension);

            if(backgroundFrameCount > 1)
            {
                const UINT propertySize =
                    backgroundImage->GetPropertyItemSize(0x5100);

                if(propertySize > 0)
                {
                    std::vector<BYTE> propertyStorage(propertySize);
                    Gdiplus::PropertyItem* item =
                        reinterpret_cast<Gdiplus::PropertyItem*>(propertyStorage.data());

                    if(backgroundImage->GetPropertyItem(
                           0x5100,
                           propertySize,
                           item) == Gdiplus::Ok &&
                       item->value &&
                       item->length >= sizeof(UINT))
                    {
                        const UINT count =
                            std::min<UINT>(
                                backgroundFrameCount,
                                item->length / sizeof(UINT));

                        const UINT* delays =
                            reinterpret_cast<const UINT*>(item->value);

                        backgroundFrameDelays.assign(delays, delays + count);
                    }
                }

                if(backgroundFrameDelays.size() < backgroundFrameCount)
                    backgroundFrameDelays.resize(backgroundFrameCount, 10);

                backgroundImage->SelectActiveFrame(
                    &backgroundFrameDimension,
                    backgroundFrameIndex);

                if(hwnd)
                {
                    SetTimer(
                        hwnd,
                        kBackgroundTimerId,
                        CurrentBackgroundFrameDelay(),
                        nullptr);
                }
            }
        }
    }

    if(hwnd)
        InvalidateRect(hwnd, nullptr, TRUE);

    return true;
}

void MainWindow::ReleaseBackgroundImage()
{
    if(hwnd)
        KillTimer(hwnd, kBackgroundTimerId);

    delete backgroundImage;
    backgroundImage = nullptr;
    backgroundFrameDelays.clear();
    backgroundFrameCount = 0;
    backgroundFrameIndex = 0;
}

unsigned int MainWindow::CurrentBackgroundFrameDelay() const
{
    if(backgroundFrameIndex < backgroundFrameDelays.size())
    {
        const unsigned int hundredths = backgroundFrameDelays[backgroundFrameIndex];
        const unsigned int milliseconds = hundredths * 10;
        return milliseconds < 30 ? 30 : milliseconds;
    }

    return 100;
}

void MainWindow::AdvanceBackgroundFrame()
{
    if(!backgroundImage || backgroundFrameCount <= 1)
        return;

    backgroundFrameIndex =
        (backgroundFrameIndex + 1) % backgroundFrameCount;

    backgroundImage->SelectActiveFrame(
        &backgroundFrameDimension,
        backgroundFrameIndex);

    SetTimer(
        hwnd,
        kBackgroundTimerId,
        CurrentBackgroundFrameDelay(),
        nullptr);

    RedrawWindow(
        hwnd,
        nullptr,
        nullptr,
        RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE | RDW_NOCHILDREN);
}

void MainWindow::ToggleBackgroundScaleMode()
{
    backgroundCover = !backgroundCover;
    SaveSettings();
    UpdateSettingsText();

    if(hwnd)
        InvalidateRect(hwnd, nullptr, TRUE);
}

void MainWindow::CycleBackgroundOverlay()
{
    const int levels[] = {0, 25, 45, 60, 75};
    size_t currentIndex = 0;

    for(size_t i = 0; i < sizeof(levels) / sizeof(levels[0]); ++i)
    {
        if(backgroundOverlayPercent == levels[i])
        {
            currentIndex = i;
            break;
        }
    }

    currentIndex = (currentIndex + 1) %
        (sizeof(levels) / sizeof(levels[0]));

    backgroundOverlayPercent = levels[currentIndex];
    SaveSettings();
    UpdateSettingsText();

    if(hwnd)
        InvalidateRect(hwnd, nullptr, TRUE);
}

void MainWindow::UpdateSettingsText()
{
    if(backgroundStatus)
    {
        if(backgroundPath.empty())
        {
            SetWindowTextW(backgroundStatus, L"No background selected");
        }
        else
        {
            const std::wstring label =
                L"Selected: " + FileNameOnly(backgroundPath);
            SetWindowTextW(backgroundStatus, label.c_str());
        }
    }

    if(scaleModeButton)
    {
        SetWindowTextW(
            scaleModeButton,
            backgroundCover ? L"Scale: Cover" : L"Scale: Fit");
    }

    if(overlayButton)
    {
        const std::wstring label =
            L"Overlay: " + std::to_wstring(backgroundOverlayPercent) + L"%";
        SetWindowTextW(overlayButton, label.c_str());
    }

    if(alwaysOnTopButton)
    {
        SetWindowTextW(
            alwaysOnTopButton,
            alwaysOnTop ? L"Always on top: On" : L"Always on top: Off");
    }
}


bool MainWindow::CopyTextToClipboard(const std::wstring& text)
{
    if(text.empty() || !OpenClipboard(hwnd))
        return false;

    EmptyClipboard();

    const SIZE_T bytes = (text.size() + 1) * sizeof(wchar_t);
    HGLOBAL memory = GlobalAlloc(GMEM_MOVEABLE, bytes);

    if(!memory)
    {
        CloseClipboard();
        return false;
    }

    void* destination = GlobalLock(memory);
    if(!destination)
    {
        GlobalFree(memory);
        CloseClipboard();
        return false;
    }

    std::memcpy(destination, text.c_str(), bytes);
    GlobalUnlock(memory);

    if(!SetClipboardData(CF_UNICODETEXT, memory))
    {
        GlobalFree(memory);
        CloseClipboard();
        return false;
    }

    CloseClipboard();
    return true;
}

void MainWindow::CopyResult()
{
    CopyTextToClipboard(currentInput);
}

void MainWindow::CopySelectedHistory()
{
    if(!historyList)
        return;

    const LRESULT selected =
        SendMessageW(historyList, LB_GETCURSEL, 0, 0);

    if(selected == LB_ERR)
    {
        MessageBoxW(
            hwnd,
            L"Select a history entry first.",
            L"Nyxoryth History",
            MB_OK | MB_ICONINFORMATION);
        return;
    }

    const LRESULT length =
        SendMessageW(historyList, LB_GETTEXTLEN, selected, 0);

    if(length == LB_ERR || length < 0)
        return;

    std::wstring item(static_cast<size_t>(length) + 1, L'\0');
    SendMessageW(
        historyList,
        LB_GETTEXT,
        selected,
        reinterpret_cast<LPARAM>(item.data()));

    item.resize(static_cast<size_t>(length));
    CopyTextToClipboard(item);
}

void MainWindow::ToggleAlwaysOnTop()
{
    alwaysOnTop = !alwaysOnTop;

    SetWindowPos(
        hwnd,
        alwaysOnTop ? HWND_TOPMOST : HWND_NOTOPMOST,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

    SaveSettings();
    UpdateSettingsText();
}

void MainWindow::ResetSettings()
{
    const int choice = MessageBoxW(
        hwnd,
        L"Reset Nyxoryth appearance and window preferences to their defaults?\r\n\r\n"
        L"This clears the saved background path, returns to Dark mode, turns "
        L"Always-on-Top off, and returns to Standard mode.\r\n\r\n"
        L"The user's background file itself will not be deleted.",
        L"Reset Nyxoryth Settings",
        MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2);

    if(choice != IDYES)
        return;

    ReleaseBackgroundImage();

    darkTheme = true;
    backgroundCover = true;
    backgroundOverlayPercent = 45;
    backgroundPath.clear();
    alwaysOnTop = false;
    degreeMode = true;

    scientificMode = false;
    tipMode = false;
    programmerMode = false;
    settingsMode = false;
    lastMode = L"Standard";

    savedWindowX = CW_USEDEFAULT;
    savedWindowY = CW_USEDEFAULT;

    if(!settingsPath.empty())
        DeleteFileW(settingsPath.c_str());

    SetWindowPos(
        hwnd,
        HWND_NOTOPMOST,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

    RECT workArea{};
    if(SystemParametersInfoW(SPI_GETWORKAREA, 0, &workArea, 0))
    {
        const int width = 500;
        const int height = 670;
        const int x = workArea.left + ((workArea.right - workArea.left) - width) / 2;
        const int y = workArea.top + ((workArea.bottom - workArea.top) - height) / 2;

        SetWindowPos(
            hwnd,
            HWND_NOTOPMOST,
            x, y,
            width, height,
            SWP_NOACTIVATE);
    }

    ClearAll();
    ApplyTheme();
    UpdateModeText();
    UpdateWindowLayout();
    UpdateSettingsText();
    SaveWindowPosition();
    SaveSettings();

    MessageBoxW(
        hwnd,
        L"Nyxoryth settings were reset to their defaults.",
        L"Nyxoryth",
        MB_OK | MB_ICONINFORMATION);
}

void MainWindow::ShowAbout()
{
    std::wstring text =
        L"Nyxoryth Calculator\r\n"
        L"Version: " + std::wstring(kNyxorythVersion) +
        L"\r\n\r\n"
        L"Native Windows calculator with Standard, Scientific, Tip, "
        L"Programmer/Computer Tools, History, themes, and user-selected "
        L"local backgrounds.\r\n\r\n"
        L"Release stage: v1.0 release candidate.\r\n"
        L"Privacy: local/offline; no telemetry or cloud calculation.\r\n"
        L"License: MIT.\r\n"
        L"Copyright/security contact: Queen3K@proton.me\r\n"
        L"Custom background images/GIFs are supplied by the user and are not bundled.";

    MessageBoxW(
        hwnd,
        text.c_str(),
        L"About Nyxoryth",
        MB_OK | MB_ICONINFORMATION);
}

void MainWindow::SaveWindowPosition()
{
    if(!hwnd || IsIconic(hwnd))
        return;

    RECT windowRect{};
    if(GetWindowRect(hwnd, &windowRect))
    {
        savedWindowX = windowRect.left;
        savedWindowY = windowRect.top;
    }
}

void MainWindow::ApplyStartupMode()
{
    scientificMode = false;
    tipMode = false;
    programmerMode = false;
    settingsMode = false;

    if(lastMode == L"Scientific")
        scientificMode = true;
    else if(lastMode == L"Tip")
        tipMode = true;
    else if(lastMode == L"Programmer")
        programmerMode = true;
    else
        lastMode = L"Standard";
}

void MainWindow::PaintBackground(HDC hdc)
{
    RECT client{};
    GetClientRect(hwnd, &client);

    const int width = client.right - client.left;
    const int height = client.bottom - client.top;

    if(width <= 0 || height <= 0)
        return;

    // Render the entire frame off-screen first. Drawing the base color directly
    // to the window before drawing the GIF frame caused a visible black flash
    // on every animation tick. The finished frame is now copied to the window
    // in one BitBlt operation.
    HDC memoryDc = CreateCompatibleDC(hdc);
    if(!memoryDc)
        return;

    HBITMAP frameBitmap = CreateCompatibleBitmap(hdc, width, height);
    if(!frameBitmap)
    {
        DeleteDC(memoryDc);
        return;
    }

    HGDIOBJ previousBitmap = SelectObject(memoryDc, frameBitmap);

    {
        Gdiplus::Graphics graphics(memoryDc);
        graphics.SetCompositingMode(Gdiplus::CompositingModeSourceOver);
        graphics.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);
        graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
        graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

        const Gdiplus::Color baseColor =
            darkTheme
                ? Gdiplus::Color(255, 22, 22, 30)
                : Gdiplus::Color(255, 246, 246, 250);

        Gdiplus::SolidBrush baseBrush(baseColor);
        graphics.FillRectangle(&baseBrush, 0, 0, width, height);

        if(backgroundImage)
        {
            const UINT imageWidth = backgroundImage->GetWidth();
            const UINT imageHeight = backgroundImage->GetHeight();

            if(imageWidth > 0 && imageHeight > 0)
            {
                const double scaleX =
                    static_cast<double>(width) / static_cast<double>(imageWidth);
                const double scaleY =
                    static_cast<double>(height) / static_cast<double>(imageHeight);

                const double scale =
                    backgroundCover
                        ? std::max(scaleX, scaleY)
                        : std::min(scaleX, scaleY);

                const int drawWidth =
                    static_cast<int>(static_cast<double>(imageWidth) * scale);
                const int drawHeight =
                    static_cast<int>(static_cast<double>(imageHeight) * scale);

                const int drawX = (width - drawWidth) / 2;
                const int drawY = (height - drawHeight) / 2;

                graphics.DrawImage(
                    backgroundImage,
                    Gdiplus::Rect(drawX, drawY, drawWidth, drawHeight),
                    0, 0,
                    static_cast<INT>(imageWidth),
                    static_cast<INT>(imageHeight),
                    Gdiplus::UnitPixel);
            }

            if(backgroundOverlayPercent > 0)
            {
                const BYTE alpha =
                    static_cast<BYTE>(
                        255.0 *
                        static_cast<double>(backgroundOverlayPercent) /
                        100.0);

                const Gdiplus::Color overlayColor =
                    darkTheme
                        ? Gdiplus::Color(alpha, 0, 0, 0)
                        : Gdiplus::Color(alpha, 255, 255, 255);

                Gdiplus::SolidBrush overlayBrush(overlayColor);
                graphics.FillRectangle(&overlayBrush, 0, 0, width, height);
            }
        }

        const Gdiplus::Color accentColor =
            darkTheme
                ? Gdiplus::Color(220, 174, 129, 255)
                : Gdiplus::Color(220, 126, 91, 196);

        Gdiplus::Pen accentPen(accentColor, 2.0f);
        graphics.DrawLine(&accentPen, 18, 4, width - 18, 4);
    }

    BitBlt(
        hdc,
        0, 0,
        width, height,
        memoryDc,
        0, 0,
        SRCCOPY);

    SelectObject(memoryDc, previousBitmap);
    DeleteObject(frameBitmap);
    DeleteDC(memoryDc);
}

void MainWindow::ToggleSettingsMode()
{
    const bool opening = !settingsMode;
    settingsMode = opening;

    if(opening)
    {
        scientificMode = false;
        tipMode = false;
        programmerMode = false;
    }
    else
    {
        ApplyStartupMode();
    }

    UpdateModeText();
    UpdateWindowLayout();
}

void MainWindow::SetTipPreset(double percent)
{
    if(tipPercentEdit)
        SetWindowTextW(tipPercentEdit, FormatNumber(percent).c_str());

    CalculateTip();
}

void MainWindow::CalculateTip()
{
    double bill = 0.0;
    double tipPercent = 0.0;
    double peopleValue = 0.0;

    if(!TryParseDouble(GetWindowTextString(tipBillEdit), bill) ||
       !TryParseDouble(GetWindowTextString(tipPercentEdit), tipPercent) ||
       !TryParseDouble(GetWindowTextString(tipPeopleEdit), peopleValue) ||
       bill < 0.0 || tipPercent < 0.0 || peopleValue < 1.0)
    {
        SetWindowTextString(tipAmountResult, L"Invalid input");
        SetWindowTextString(tipTotalResult, L"—");
        SetWindowTextString(tipPerPersonResult, L"—");
        return;
    }

    const double peopleRounded = std::floor(peopleValue);
    if(std::fabs(peopleValue - peopleRounded) > 1e-12)
    {
        SetWindowTextString(tipAmountResult, L"People must be whole");
        SetWindowTextString(tipTotalResult, L"—");
        SetWindowTextString(tipPerPersonResult, L"—");
        return;
    }

    const double tip = bill * tipPercent / 100.0;
    const double total = bill + tip;
    const double perPerson = total / peopleRounded;

    auto money = [](double value)
    {
        std::wostringstream out;
        out << L"$" << std::fixed << std::setprecision(2) << value;
        return out.str();
    };

    SetWindowTextString(tipAmountResult, money(tip));
    SetWindowTextString(tipTotalResult, money(total));
    SetWindowTextString(tipPerPersonResult, money(perPerson));

    std::wostringstream historyLine;
    historyLine << L"Tip: " << money(bill)
                << L" + " << FormatNumber(tipPercent) << L"% = "
                << money(total);

    if(peopleRounded > 1.0)
        historyLine << L" (" << money(perPerson) << L" each)";

    AddHistory(historyLine.str());
}

void MainWindow::ConvertProgrammerValue()
{
    unsigned long long value = 0;
    if(!TryParseUnsigned(GetWindowTextString(programmerValueEdit), value))
    {
        SetWindowTextString(programmerDecResult, L"DEC: Invalid integer");
        SetWindowTextString(programmerHexResult, L"HEX: —");
        SetWindowTextString(programmerOctResult, L"OCT: —");
        SetWindowTextString(programmerBinResult, L"BIN: —");
        return;
    }

    SetWindowTextString(programmerDecResult, L"DEC: " + std::to_wstring(value));
    SetWindowTextString(programmerHexResult, L"HEX: " + ToUpperHex(value));
    SetWindowTextString(programmerOctResult, L"OCT: " + ToOctal(value));
    SetWindowTextString(programmerBinResult, L"BIN: " + ToBinaryString(value));
}

void MainWindow::ApplyBitwiseOperation(int operation)
{
    unsigned long long a = 0;
    unsigned long long b = 0;

    if(!TryParseUnsigned(GetWindowTextString(bitwiseAEdit), a))
    {
        SetWindowTextString(bitwiseResult, L"Result: Invalid A");
        return;
    }

    if(operation != BTN_BIT_NOT && operation != BTN_SHIFT_LEFT && operation != BTN_SHIFT_RIGHT &&
       !TryParseUnsigned(GetWindowTextString(bitwiseBEdit), b))
    {
        SetWindowTextString(bitwiseResult, L"Result: Invalid B");
        return;
    }

    unsigned long long result = 0;
    std::wstring label;

    switch(operation)
    {
        case BTN_BIT_AND:
            result = a & b;
            label = L"AND";
            break;
        case BTN_BIT_OR:
            result = a | b;
            label = L"OR";
            break;
        case BTN_BIT_XOR:
            result = a ^ b;
            label = L"XOR";
            break;
        case BTN_BIT_NOT:
            result = ~a;
            label = L"NOT";
            break;
        case BTN_SHIFT_LEFT:
            result = a << 1;
            label = L"<< 1";
            break;
        case BTN_SHIFT_RIGHT:
            result = a >> 1;
            label = L">> 1";
            break;
        default:
            return;
    }

    std::wstring output =
        L"Result: " + std::to_wstring(result) +
        L"  (0x" + ToUpperHex(result) + L", 0b" + ToBinaryString(result) + L")";

    SetWindowTextString(bitwiseResult, output);
    AddHistory(L"Bitwise " + label + L" = " + std::to_wstring(result));
}

void MainWindow::ConvertStorage()
{
    double bytes = 0.0;

    if(!TryParseDouble(GetWindowTextString(storageBytesEdit), bytes) || bytes < 0.0)
    {
        SetWindowTextString(storageResult, L"Invalid byte value");
        return;
    }

    const double kib = bytes / 1024.0;
    const double mib = kib / 1024.0;
    const double gib = mib / 1024.0;
    const double tib = gib / 1024.0;

    std::wostringstream out;
    out << std::setprecision(10)
        << FormatNumber(bytes) << L" B | "
        << FormatNumber(kib) << L" KiB | "
        << FormatNumber(mib) << L" MiB | "
        << FormatNumber(gib) << L" GiB | "
        << FormatNumber(tib) << L" TiB";

    SetWindowTextString(storageResult, out.str());
}

void MainWindow::CalculateCidr()
{
    std::uint32_t ip = 0;
    unsigned long long prefixValue = 0;

    if(!ParseIpv4(GetWindowTextString(cidrIpEdit), ip) ||
       !TryParseUnsigned(GetWindowTextString(cidrPrefixEdit), prefixValue) ||
       prefixValue > 32)
    {
        SetWindowTextString(cidrResult, L"Invalid IPv4 address or prefix");
        return;
    }

    const unsigned int prefix = static_cast<unsigned int>(prefixValue);
    const std::uint32_t mask =
        prefix == 0 ? 0u : (0xFFFFFFFFu << (32u - prefix));

    const std::uint32_t network = ip & mask;
    const std::uint32_t broadcast = network | ~mask;
    const std::uint64_t addresses =
        prefix == 0 ? (1ULL << 32) : (1ULL << (32u - prefix));

    std::wostringstream out;
    out << L"Net " << FormatIpv4(network)
        << L" | Mask " << FormatIpv4(mask)
        << L"\r\nBcast " << FormatIpv4(broadcast)
        << L" | Addresses " << addresses;

    SetWindowTextString(cidrResult, out.str());
}

void MainWindow::ConvertCharacterCode()
{
    const std::wstring text = GetWindowTextString(charCodeEdit);

    if(text.empty())
    {
        SetWindowTextString(charCodeResult, L"Enter a character");
        return;
    }

    std::uint32_t codepoint = static_cast<std::uint32_t>(text[0]);

    if(text.size() >= 2 &&
       codepoint >= 0xD800 && codepoint <= 0xDBFF)
    {
        const std::uint32_t low = static_cast<std::uint32_t>(text[1]);
        if(low >= 0xDC00 && low <= 0xDFFF)
        {
            codepoint =
                0x10000 +
                ((codepoint - 0xD800) << 10) +
                (low - 0xDC00);
        }
    }

    std::wostringstream out;
    out << L"U+" << std::uppercase << std::hex
        << std::setw(codepoint <= 0xFFFF ? 4 : 6)
        << std::setfill(L'0') << codepoint
        << std::dec << L" / " << codepoint
        << L" / 0x" << std::uppercase << std::hex << codepoint;

    SetWindowTextString(charCodeResult, out.str());
}

bool MainWindow::ApplyBinary(double left, double right, wchar_t op, double& result)
{
    switch(op)
    {
        case L'+': result = left + right; break;
        case L'-': result = left - right; break;
        case L'*': result = left * right; break;
        case L'/':
            if(right == 0.0)
            {
                SetError(L"Cannot divide by zero");
                return false;
            }
            result = left / right;
            break;
        case L'^':
            result = std::pow(left, right);
            break;
        default:
            return false;
    }

    if(!std::isfinite(result))
    {
        SetError(L"Result overflow");
        return false;
    }

    return true;
}

double MainWindow::CurrentValue() const
{
    try
    {
        return std::stod(currentInput);
    }
    catch(...)
    {
        return 0.0;
    }
}

std::wstring MainWindow::FormatNumber(double number) const
{
    if(!std::isfinite(number))
        return L"Error";

    if(std::fabs(number) < 1e-15)
        number = 0.0;

    std::wostringstream out;
    out << std::setprecision(15) << number;
    std::wstring text = out.str();

    const auto exponentPosition = text.find_first_of(L"eE");
    const auto decimalPosition = text.find(L'.');

    if(decimalPosition != std::wstring::npos && exponentPosition == std::wstring::npos)
    {
        while(!text.empty() && text.back() == L'0')
            text.pop_back();

        if(!text.empty() && text.back() == L'.')
            text.pop_back();
    }

    return text.empty() ? L"0" : text;
}

std::wstring MainWindow::OperatorText(wchar_t op) const
{
    if(op == L'*') return L"×";
    if(op == L'/') return L"÷";
    if(op == L'-') return L"−";
    if(op == L'^') return L"^";
    return std::wstring(1, op);
}

void MainWindow::SetError(const std::wstring& message)
{
    currentInput = message;
    expression.clear();
    accumulator = 0.0;
    pendingOperator = 0;
    waitingForOperand = true;
    justEvaluated = true;
    errorState = true;
    UpdateDisplay();
}

void MainWindow::UpdateExpressionForCurrentInput()
{
    if(pendingOperator != 0 && !waitingForOperand)
    {
        expression = FormatNumber(accumulator) + L" " + OperatorText(pendingOperator) +
                     L" " + currentInput;
    }
}

void MainWindow::AddHistory(const std::wstring& item)
{
    history.push_back(item);

    if(historyList)
    {
        SendMessageW(
            historyList,
            LB_ADDSTRING,
            0,
            reinterpret_cast<LPARAM>(item.c_str()));

        const LRESULT count = SendMessageW(historyList, LB_GETCOUNT, 0, 0);
        if(count > 0)
            SendMessageW(historyList, LB_SETTOPINDEX, static_cast<WPARAM>(count - 1), 0);
    }
}

void MainWindow::ToggleHistory()
{
    historyVisible = !historyVisible;
    UpdateWindowLayout();

    if(HWND button = GetDlgItem(hwnd, BTN_HISTORY))
        InvalidateRect(button, nullptr, FALSE);
}

void MainWindow::ClearHistory()
{
    history.clear();

    if(historyList)
        SendMessageW(historyList, LB_RESETCONTENT, 0, 0);
}

void MainWindow::UpdateWindowLayout()
{
    for(HWND control : scientificControls)
        ShowWindow(control, scientificMode ? SW_SHOW : SW_HIDE);

    for(HWND control : tipControls)
        ShowWindow(control, tipMode ? SW_SHOW : SW_HIDE);

    for(HWND control : programmerControls)
        ShowWindow(control, programmerMode ? SW_SHOW : SW_HIDE);

    for(HWND control : settingsControls)
        ShowWindow(control, settingsMode ? SW_SHOW : SW_HIDE);

    const bool sidePanelVisible =
        scientificMode || tipMode || programmerMode || settingsMode;
    const int historyX = sidePanelVisible ? 820 : 500;

    if(historyList)
    {
        SetWindowPos(historyList, nullptr, historyX, 42, 220, 464, SWP_NOZORDER);
        ShowWindow(historyList, historyVisible ? SW_SHOW : SW_HIDE);
    }

    if(copyHistoryButton)
    {
        SetWindowPos(copyHistoryButton, nullptr, historyX, 524, 105, 42, SWP_NOZORDER);
        ShowWindow(copyHistoryButton, historyVisible ? SW_SHOW : SW_HIDE);
    }

    if(clearHistoryButton)
    {
        SetWindowPos(clearHistoryButton, nullptr, historyX + 115, 524, 105, 42, SWP_NOZORDER);
        ShowWindow(clearHistoryButton, historyVisible ? SW_SHOW : SW_HIDE);
    }

    int width = 500;
    if(sidePanelVisible) width += 320;
    if(historyVisible) width += 240;

    SetWindowPos(hwnd, nullptr, 0, 0, width, 670, SWP_NOMOVE | SWP_NOZORDER);
}

void MainWindow::UpdateModeText()
{
    if(modeLabel)
    {
        if(scientificMode)
            SetWindowTextW(modeLabel, degreeMode ? L"SCIENTIFIC • DEG" : L"SCIENTIFIC • RAD");
        else if(tipMode)
            SetWindowTextW(modeLabel, L"TIP / BILL SPLIT");
        else if(programmerMode)
            SetWindowTextW(modeLabel, L"PROGRAMMER / COMPUTER TOOLS");
        else if(settingsMode)
            SetWindowTextW(modeLabel, L"SETTINGS / APPEARANCE");
        else
            SetWindowTextW(modeLabel, L"STANDARD");
    }

    if(modeButton)
        SetWindowTextW(modeButton, scientificMode ? L"Standard ◀" : L"Scientific");

    if(tipButton)
        SetWindowTextW(tipButton, tipMode ? L"Standard ◀" : L"Tip");

    if(programmerButton)
        SetWindowTextW(programmerButton, programmerMode ? L"Standard ◀" : L"Programmer");

    if(settingsButton)
        SetWindowTextW(settingsButton, settingsMode ? L"Standard ◀" : L"Settings");

    if(angleButton)
        SetWindowTextW(angleButton, degreeMode ? L"DEG" : L"RAD");
}

void MainWindow::HandleButton(int id)
{
    if(id >= BTN_0 && id <= BTN_9)
    {
        InputDigit(static_cast<wchar_t>(L'0' + (id - BTN_0)));
        return;
    }

    switch(id)
    {
        case BTN_PLUS:          SetOperator(L'+'); break;
        case BTN_MINUS:         SetOperator(L'-'); break;
        case BTN_MULTIPLY:      SetOperator(L'*'); break;
        case BTN_DIVIDE:        SetOperator(L'/'); break;
        case BTN_EQUALS:        Evaluate(); break;
        case BTN_CLEAR:         ClearAll(); break;
        case BTN_DECIMAL:       InputDecimal(); break;
        case BTN_BACKSPACE:     Backspace(); break;
        case BTN_PERCENT:       Percent(); break;
        case BTN_SIGN:          ToggleSign(); break;
        case BTN_CLEAR_ENTRY:   ClearEntry(); break;
        case BTN_RECIPROCAL:    Reciprocal(); break;
        case BTN_SQUARE:        Square(); break;
        case BTN_SQRT:          SquareRoot(); break;
        case BTN_HISTORY:       ToggleHistory(); break;
        case BTN_CLEAR_HISTORY: ClearHistory(); break;
        case BTN_MODE:          ToggleScientificMode(); break;
        case BTN_ANGLE:         ToggleAngleMode(); break;
        case BTN_SIN:           Sine(); break;
        case BTN_COS:           Cosine(); break;
        case BTN_TAN:           Tangent(); break;
        case BTN_ASIN:          ArcSine(); break;
        case BTN_ACOS:          ArcCosine(); break;
        case BTN_ATAN:          ArcTangent(); break;
        case BTN_LOG10:         Log10(); break;
        case BTN_LN:            NaturalLog(); break;
        case BTN_FACTORIAL:     Factorial(); break;
        case BTN_PI:            InsertPi(); break;
        case BTN_E:             InsertE(); break;
        case BTN_POWER:         SetOperator(L'^'); break;
        case BTN_TEN_POWER:     TenPower(); break;
        case BTN_E_POWER:       EPower(); break;
        case BTN_ABS:           AbsoluteValue(); break;

        case BTN_TIP_MODE:       ToggleTipMode(); break;
        case BTN_PROGRAMMER_MODE:ToggleProgrammerMode(); break;
        case BTN_TIP_CALCULATE:  CalculateTip(); break;
        case BTN_TIP_15:         SetTipPreset(15.0); break;
        case BTN_TIP_18:         SetTipPreset(18.0); break;
        case BTN_TIP_20:         SetTipPreset(20.0); break;
        case BTN_TIP_25:         SetTipPreset(25.0); break;

        case BTN_PROGRAMMER_CONVERT: ConvertProgrammerValue(); break;
        case BTN_BIT_AND:
        case BTN_BIT_OR:
        case BTN_BIT_XOR:
        case BTN_BIT_NOT:
        case BTN_SHIFT_LEFT:
        case BTN_SHIFT_RIGHT:
            ApplyBitwiseOperation(id);
            break;
        case BTN_STORAGE_CONVERT: ConvertStorage(); break;
        case BTN_CIDR_CALCULATE:  CalculateCidr(); break;
        case BTN_CHAR_CODE:       ConvertCharacterCode(); break;

        case BTN_SETTINGS_MODE:      ToggleSettingsMode(); break;
        case BTN_THEME_DARK:
            darkTheme = true;
            ApplyTheme();
            SaveSettings();
            break;
        case BTN_THEME_LIGHT:
            darkTheme = false;
            ApplyTheme();
            SaveSettings();
            break;
        case BTN_BACKGROUND_CHOOSE:  ChooseBackground(); break;
        case BTN_BACKGROUND_CLEAR:   ClearBackground(); break;
        case BTN_BACKGROUND_SCALE:   ToggleBackgroundScaleMode(); break;
        case BTN_BACKGROUND_OVERLAY: CycleBackgroundOverlay(); break;
        case BTN_COPY_RESULT:        CopyResult(); break;
        case BTN_COPY_HISTORY:       CopySelectedHistory(); break;
        case BTN_ALWAYS_ON_TOP:      ToggleAlwaysOnTop(); break;
        case BTN_RESET_SETTINGS:      ResetSettings(); break;
        case BTN_ABOUT:              ShowAbout(); break;

        default: break;
    }
}

void MainWindow::HandleCharacter(wchar_t ch)
{
    if(ch >= L'0' && ch <= L'9')
    {
        InputDigit(ch);
        return;
    }

    switch(ch)
    {
        case L'.': InputDecimal(); break;
        case L'+': SetOperator(L'+'); break;
        case L'-': SetOperator(L'-'); break;
        case L'*': SetOperator(L'*'); break;
        case L'/': SetOperator(L'/'); break;
        case L'^': SetOperator(L'^'); break;
        case L'%': Percent(); break;
        case L'=': Evaluate(); break;
        case L'\r': Evaluate(); break;
        case L'c':
        case L'C': ClearAll(); break;
        default: break;
    }
}

void MainWindow::UpdateDisplay()
{
    if(resultDisplay)
        SetWindowTextW(resultDisplay, currentInput.c_str());

    if(expressionDisplay)
        SetWindowTextW(expressionDisplay, expression.c_str());
}

int MainWindow::Run()
{
    MSG msg{};

    while(GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK MainWindow::WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    MainWindow* app = reinterpret_cast<MainWindow*>(
        GetWindowLongPtrW(window, GWLP_USERDATA));

    if(message == WM_NCCREATE)
    {
        auto create = reinterpret_cast<CREATESTRUCTW*>(lParam);
        app = reinterpret_cast<MainWindow*>(create->lpCreateParams);
        SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app));
    }

    switch(message)
    {
        case WM_DRAWITEM:
            if(app)
            {
                const DRAWITEMSTRUCT* drawItem =
                    reinterpret_cast<const DRAWITEMSTRUCT*>(lParam);

                if(drawItem && drawItem->CtlType == ODT_BUTTON)
                {
                    app->DrawOwnerButton(drawItem);
                    return TRUE;
                }
            }
            break;

        case WM_ERASEBKGND:
            return 1;

        case WM_PAINT:
            if(app)
            {
                PAINTSTRUCT paint{};
                HDC hdc = BeginPaint(window, &paint);
                app->PaintBackground(hdc);
                EndPaint(window, &paint);
                return 0;
            }
            break;

        case WM_CTLCOLORSTATIC:
            if(app)
            {
                HDC hdc = reinterpret_cast<HDC>(wParam);
                HWND control = reinterpret_cast<HWND>(lParam);

                const COLORREF text =
                    app->darkTheme ? RGB(242, 242, 248) : RGB(24, 24, 28);

                if(control == app->expressionDisplay || control == app->resultDisplay)
                {
                    const COLORREF edit =
                        app->darkTheme ? RGB(36, 36, 48) : RGB(255, 255, 255);
                    SetTextColor(hdc, text);
                    SetBkColor(hdc, edit);
                    return reinterpret_cast<LRESULT>(app->themeEditBrush);
                }

                // Child-control areas are clipped out of the parent GIF/theme
                // repaint. Give labels/results an explicit themed background so
                // Light mode cannot expose a black/unpainted control rectangle.
                SetTextColor(hdc, text);
                const COLORREF background =
                    app->darkTheme ? RGB(22, 22, 30) : RGB(250, 250, 253);
                SetBkMode(hdc, OPAQUE);
                SetBkColor(hdc, background);
                return reinterpret_cast<LRESULT>(app->themeBackgroundBrush);
            }
            break;

        case WM_CTLCOLORBTN:
            if(app)
            {
                HDC hdc = reinterpret_cast<HDC>(wParam);
                const COLORREF background =
                    app->darkTheme ? RGB(22, 22, 30) : RGB(250, 250, 253);
                SetBkColor(hdc, background);
                return reinterpret_cast<LRESULT>(app->themeBackgroundBrush);
            }
            break;

        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORLISTBOX:
            if(app)
            {
                HDC hdc = reinterpret_cast<HDC>(wParam);
                const COLORREF text =
                    app->darkTheme ? RGB(242, 242, 248) : RGB(24, 24, 28);
                const COLORREF edit =
                    app->darkTheme ? RGB(36, 36, 48) : RGB(255, 255, 255);

                SetTextColor(hdc, text);
                SetBkColor(hdc, edit);
                return reinterpret_cast<LRESULT>(app->themeEditBrush);
            }
            break;

        case WM_TIMER:
            if(app && wParam == kBackgroundTimerId)
            {
                app->AdvanceBackgroundFrame();
                return 0;
            }
            break;

        case WM_CREATE:
            if(app)
            {
                app->hwnd = window;
                app->CreateControls();
            }
            return 0;

        case WM_COMMAND:
            if(app)
                app->HandleButton(LOWORD(wParam));
            return 0;

        case WM_CHAR:
            if(app)
                app->HandleCharacter(static_cast<wchar_t>(wParam));
            return 0;

        case WM_KEYDOWN:
            if(app)
            {
                if((GetKeyState(VK_CONTROL) & 0x8000) && (wParam == L'C' || wParam == L'c'))
                {
                    app->CopyResult();
                    return 0;
                }

                if(wParam == VK_F1)
                {
                    app->ShowAbout();
                    return 0;
                }

                if(wParam == VK_BACK)
                {
                    app->Backspace();
                    return 0;
                }

                if(wParam == VK_ESCAPE)
                {
                    app->ClearAll();
                    return 0;
                }

                if(wParam == VK_DELETE)
                {
                    app->ClearEntry();
                    return 0;
                }
            }
            break;

        case WM_DESTROY:
            if(app)
            {
                app->SaveWindowPosition();
                app->SaveSettings();

                if(app->expressionFont) DeleteObject(app->expressionFont);
                if(app->resultFont) DeleteObject(app->resultFont);
                if(app->buttonFont) DeleteObject(app->buttonFont);

                app->ReleaseBackgroundImage();

                if(app->themeBackgroundBrush)
                {
                    DeleteObject(app->themeBackgroundBrush);
                    app->themeBackgroundBrush = nullptr;
                }

                if(app->themeEditBrush)
                {
                    DeleteObject(app->themeEditBrush);
                    app->themeEditBrush = nullptr;
                }

                if(app->gdiplusToken != 0)
                {
                    Gdiplus::GdiplusShutdown(app->gdiplusToken);
                    app->gdiplusToken = 0;
                }
            }
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcW(window, message, wParam, lParam);
}

}

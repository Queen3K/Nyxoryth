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
#include <numeric>
#include <random>
#include <array>
#include <cstdlib>
#include <stdexcept>

namespace Nyxoryth
{

namespace
{
    constexpr double kPi = 3.141592653589793238462643383279502884;
    constexpr double kE  = 2.718281828459045235360287471352662498;
    constexpr const wchar_t* kNyxorythVersion = L"1.1.0";
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
        BTN_ABOUT,
        BTN_NAV_MENU,

        BTN_SCIENTIFIC_ADVANCED_MODE = 200,
        BTN_STATISTICS_MODE,
        BTN_FRACTIONS_MODE,
        BTN_EQUATIONS_MODE,
        BTN_COMPLEX_MODE,
        BTN_UNIT_MODE,
        BTN_DATE_MODE,
        BTN_BITTOOLS_MODE,
        BTN_COMPUTER_MATH_MODE,

        BTN_SCI_ADV_SINH = 220,
        BTN_SCI_ADV_COSH,
        BTN_SCI_ADV_TANH,
        BTN_SCI_ADV_ASINH,
        BTN_SCI_ADV_ACOSH,
        BTN_SCI_ADV_ATANH,
        BTN_SCI_ADV_CBRT,
        BTN_SCI_ADV_NTH_ROOT,
        BTN_SCI_ADV_TWO_POWER,
        BTN_SCI_ADV_LOG2,
        BTN_SCI_ADV_NCR,
        BTN_SCI_ADV_NPR,
        BTN_SCI_ADV_MOD,
        BTN_SCI_ADV_FLOOR,
        BTN_SCI_ADV_CEIL,
        BTN_SCI_ADV_ROUND,
        BTN_SCI_ADV_RANDOM,

        BTN_STATISTICS_CALCULATE = 245,

        BTN_FRACTION_ADD = 250,
        BTN_FRACTION_SUBTRACT,
        BTN_FRACTION_MULTIPLY,
        BTN_FRACTION_DIVIDE,
        BTN_FRACTION_SIMPLIFY,
        BTN_FRACTION_DECIMAL,
        BTN_FRACTION_TO_MIXED,
        BTN_MIXED_TO_IMPROPER,
        BTN_FRACTION_GCD_LCM,

        BTN_LINEAR_SOLVE = 270,
        BTN_QUADRATIC_SOLVE,

        BTN_COMPLEX_ADD = 280,
        BTN_COMPLEX_SUBTRACT,
        BTN_COMPLEX_MULTIPLY,
        BTN_COMPLEX_DIVIDE,
        BTN_COMPLEX_MAGNITUDE,
        BTN_COMPLEX_CONJUGATE,
        BTN_COMPLEX_ARGUMENT,

        COMBO_UNIT_CATEGORY = 290,
        COMBO_UNIT_FROM,
        COMBO_UNIT_TO,
        BTN_UNIT_CONVERT,

        BTN_DATE_DIFFERENCE = 300,
        BTN_DATE_ADD_DAYS,
        BTN_DATE_INSPECT,
        BTN_DATE_TO_UNIX,
        BTN_DATE_FROM_UNIX,
        BTN_DURATION_CONVERT,

        BTN_BITTOOLS_CONVERT = 320,
        BTN_BITTOOLS_SET,
        BTN_BITTOOLS_CLEAR,
        BTN_BITTOOLS_TOGGLE,
        BTN_BITTOOLS_ROL,
        BTN_BITTOOLS_ROR,
        BTN_BITTOOLS_ENDIAN,
        BTN_UNICODE_LOOKUP,

        BTN_COMPUTER_TRANSFER = 340,
        BTN_COMPUTER_BITRATE,
        BTN_COMPUTER_RESOLUTION,
        BTN_COMPUTER_RAID,

        MENU_STANDARD = 1000
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
        if(length <= 0)
            return L"";

        std::wstring text(static_cast<size_t>(length) + 1, L'\0');
        GetWindowTextW(control, text.data(), length + 1);
        text.resize(static_cast<size_t>(length));
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


    bool TryParseSignedLongLong(const std::wstring& input, long long& value)
    {
        try
        {
            const std::wstring text = Trim(input);
            if(text.empty())
                return false;

            size_t used = 0;
            value = std::stoll(text, &used, 10);
            return used == text.size();
        }
        catch(...)
        {
            return false;
        }
    }

    bool ParseNumberList(
        const std::wstring& input,
        std::vector<double>& values,
        std::wstring& error)
    {
        values.clear();
        error.clear();

        std::wstring normalized = input;
        for(wchar_t& ch : normalized)
        {
            if(ch == L',' || ch == L';' || ch == L'\n' || ch == L'\r' || ch == L'\t')
                ch = L' ';
        }

        std::wistringstream stream(normalized);
        std::wstring token;
        size_t tokenNumber = 0;

        while(stream >> token)
        {
            ++tokenNumber;

            double value = 0.0;
            if(!TryParseDouble(token, value))
            {
                error =
                    L"Value #" + std::to_wstring(tokenNumber) +
                    L" is not a valid finite number: \"" + token + L"\".";
                return false;
            }

            // Keep accidental paste-bombs from turning a simple calculator
            // action into an unbounded sort/allocation operation.
            if(values.size() >= 100000)
            {
                error = L"Statistics accepts up to 100,000 values at a time.";
                return false;
            }

            values.push_back(value);
        }

        if(values.empty())
        {
            error =
                L"Enter at least one number. Separate values with commas, "
                L"semicolons, spaces, tabs, or new lines.";
            return false;
        }

        return true;
    }

    struct CalendarDate
    {
        int year = 1970;
        int month = 1;
        int day = 1;
    };

    bool IsLeapYearValue(int year)
    {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    int DaysInMonthValue(int year, int month)
    {
        static const int days[] =
        {
            31, 28, 31, 30, 31, 30,
            31, 31, 30, 31, 30, 31
        };

        if(month < 1 || month > 12)
            return 0;

        if(month == 2 && IsLeapYearValue(year))
            return 29;

        return days[month - 1];
    }

    bool ParseDateValue(const std::wstring& input, CalendarDate& date)
    {
        const std::wstring text = Trim(input);
        std::wistringstream stream(text);
        int year = 0;
        int month = 0;
        int day = 0;
        wchar_t dash1 = 0;
        wchar_t dash2 = 0;

        if(!(stream >> year >> dash1 >> month >> dash2 >> day))
            return false;

        stream >> std::ws;
        if(!stream.eof() || dash1 != L'-' || dash2 != L'-')
            return false;

        if(year < 1 || year > 9999 || month < 1 || month > 12)
            return false;

        const int maxDay = DaysInMonthValue(year, month);
        if(day < 1 || day > maxDay)
            return false;

        date.year = year;
        date.month = month;
        date.day = day;
        return true;
    }

    long long DateOrdinal(const CalendarDate& date)
    {
        const long long previousYears = static_cast<long long>(date.year) - 1;
        long long days =
            previousYears * 365LL +
            previousYears / 4LL -
            previousYears / 100LL +
            previousYears / 400LL;

        for(int month = 1; month < date.month; ++month)
            days += DaysInMonthValue(date.year, month);

        days += static_cast<long long>(date.day) - 1LL;
        return days;
    }

    bool DateFromOrdinal(long long ordinal, CalendarDate& date)
    {
        if(ordinal < 0)
            return false;

        int low = 1;
        int high = 9999;
        int year = 1;
        bool foundYear = false;

        while(low <= high)
        {
            const int mid = low + (high - low) / 2;
            CalendarDate jan1{mid, 1, 1};
            const long long start = DateOrdinal(jan1);
            CalendarDate nextJan{mid == 9999 ? 9999 : mid + 1, 1, 1};
            const long long nextStart =
                mid == 9999
                    ? start + (IsLeapYearValue(mid) ? 366LL : 365LL)
                    : DateOrdinal(nextJan);

            if(ordinal < start)
            {
                high = mid - 1;
            }
            else if(ordinal >= nextStart)
            {
                low = mid + 1;
            }
            else
            {
                year = mid;
                foundYear = true;
                break;
            }
        }

        if(!foundYear)
            return false;

        CalendarDate jan1{year, 1, 1};
        long long dayOfYear = ordinal - DateOrdinal(jan1);
        int month = 1;

        while(month <= 12)
        {
            const int monthDays = DaysInMonthValue(year, month);
            if(dayOfYear < monthDays)
                break;
            dayOfYear -= monthDays;
            ++month;
        }

        if(month > 12)
            return false;

        date.year = year;
        date.month = month;
        date.day = static_cast<int>(dayOfYear) + 1;
        return true;
    }

    std::wstring FormatDateValue(const CalendarDate& date)
    {
        std::wostringstream out;
        out << std::setfill(L'0')
            << std::setw(4) << date.year << L'-'
            << std::setw(2) << date.month << L'-'
            << std::setw(2) << date.day;
        return out.str();
    }

    bool ParseClockValue(const std::wstring& input, int& hour, int& minute, int& second)
    {
        std::wistringstream stream(Trim(input));
        wchar_t colon1 = 0;
        wchar_t colon2 = 0;

        if(!(stream >> hour >> colon1 >> minute >> colon2 >> second))
            return false;

        stream >> std::ws;
        if(!stream.eof() || colon1 != L':' || colon2 != L':')
            return false;

        return hour >= 0 && hour <= 23 &&
               minute >= 0 && minute <= 59 &&
               second >= 0 && second <= 59;
    }

    std::wstring DayOfWeekName(long long ordinal)
    {
        static const wchar_t* names[] =
        {
            L"Monday", L"Tuesday", L"Wednesday", L"Thursday",
            L"Friday", L"Saturday", L"Sunday"
        };

        // 0001-01-01 was a Monday in the proleptic Gregorian calendar.
        const int index = static_cast<int>((ordinal % 7LL + 7LL) % 7LL);
        return names[index];
    }

    std::wstring GroupBinary(unsigned long long value, int bits)
    {
        if(bits != 8 && bits != 16 && bits != 32 && bits != 64)
            bits = 64;

        std::wstring result;
        result.reserve(static_cast<size_t>(bits + bits / 4));

        for(int bit = bits - 1; bit >= 0; --bit)
        {
            result.push_back(((value >> bit) & 1ULL) ? L'1' : L'0');
            if(bit > 0 && bit % 4 == 0)
                result.push_back(L' ');
        }

        return result;
    }

    unsigned long long MaskForBits(int bits)
    {
        if(bits >= 64)
            return std::numeric_limits<unsigned long long>::max();
        return (1ULL << bits) - 1ULL;
    }

    int CountSetBits(unsigned long long value)
    {
        int count = 0;
        while(value != 0)
        {
            count += static_cast<int>(value & 1ULL);
            value >>= 1;
        }
        return count;
    }

    std::wstring AsciiDescription(int code)
    {
        static const wchar_t* controlNames[32] =
        {
            L"NUL", L"SOH", L"STX", L"ETX", L"EOT", L"ENQ", L"ACK", L"BEL",
            L"BS", L"TAB", L"LF", L"VT", L"FF", L"CR", L"SO", L"SI",
            L"DLE", L"DC1", L"DC2", L"DC3", L"DC4", L"NAK", L"SYN", L"ETB",
            L"CAN", L"EM", L"SUB", L"ESC", L"FS", L"GS", L"RS", L"US"
        };

        if(code >= 0 && code < 32)
            return controlNames[code];
        if(code == 32)
            return L"SPACE";
        if(code == 127)
            return L"DEL";
        if(code >= 33 && code <= 126)
            return std::wstring(1, static_cast<wchar_t>(code));
        return L"";
    }

    bool CheckedFractionValue(long double value, long long& output)
    {
        const long double minimum =
            static_cast<long double>(std::numeric_limits<long long>::min());
        const long double maximum =
            static_cast<long double>(std::numeric_limits<long long>::max());

        if(!std::isfinite(static_cast<double>(value)) || value < minimum || value > maximum)
            return false;

        output = static_cast<long long>(value);
        return true;
    }

    bool NormalizeFraction(long long& numerator, long long& denominator)
    {
        if(denominator == 0 ||
           numerator == std::numeric_limits<long long>::min() ||
           denominator == std::numeric_limits<long long>::min())
        {
            return false;
        }

        if(denominator < 0)
        {
            if(numerator == std::numeric_limits<long long>::min() ||
               denominator == std::numeric_limits<long long>::min())
            {
                return false;
            }
            numerator = -numerator;
            denominator = -denominator;
        }

        const long long common = std::gcd(numerator, denominator);
        if(common != 0)
        {
            numerator /= common;
            denominator /= common;
        }
        return true;
    }

    std::wstring FractionText(long long numerator, long long denominator)
    {
        if(!NormalizeFraction(numerator, denominator))
            return L"Invalid fraction";

        if(denominator == 1)
            return std::to_wstring(numerator);

        return std::to_wstring(numerator) + L"/" + std::to_wstring(denominator);
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
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX |
            WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN,
        savedWindowX,
        savedWindowY,
        std::max(savedWindowWidth, 500),
        std::max(savedWindowHeight, 670),
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
    navigationButton = CreateWindowExW(
        0, L"BUTTON", L"☰",
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        20, 8, 38, 28,
        hwnd,
        reinterpret_cast<HMENU>(static_cast<INT_PTR>(BTN_NAV_MENU)),
        nullptr, nullptr);
    RegisterButton(navigationButton);

    modeLabel = CreateWindowExW(
        0, L"STATIC", L"STANDARD",
        WS_CHILD | WS_VISIBLE,
        70, 12, 275, 24,
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
        {L"%",   BTN_PERCENT,      20, 166, 100, 50},
        {L"CE",  BTN_CLEAR_ENTRY, 130, 166, 100, 50},
        {L"C",   BTN_CLEAR,       240, 166, 100, 50},
        {L"⌫",   BTN_BACKSPACE,   350, 166, 100, 50},

        {L"1/x", BTN_RECIPROCAL,  20, 224, 100, 50},
        {L"x²",  BTN_SQUARE,      130, 224, 100, 50},
        {L"√",   BTN_SQRT,        240, 224, 100, 50},
        {L"÷",   BTN_DIVIDE,      350, 224, 100, 50},

        {L"7",   BTN_7,           20, 282, 100, 50},
        {L"8",   BTN_8,          130, 282, 100, 50},
        {L"9",   BTN_9,          240, 282, 100, 50},
        {L"×",   BTN_MULTIPLY,   350, 282, 100, 50},

        {L"4",   BTN_4,           20, 340, 100, 50},
        {L"5",   BTN_5,          130, 340, 100, 50},
        {L"6",   BTN_6,          240, 340, 100, 50},
        {L"−",   BTN_MINUS,      350, 340, 100, 50},

        {L"1",   BTN_1,           20, 398, 100, 50},
        {L"2",   BTN_2,          130, 398, 100, 50},
        {L"3",   BTN_3,          240, 398, 100, 50},
        {L"+",   BTN_PLUS,       350, 398, 100, 50},

        {L"±",   BTN_SIGN,        20, 456, 100, 50},
        {L"0",   BTN_0,          130, 456, 100, 50},
        {L".",   BTN_DECIMAL,    240, 456, 100, 50},
        {L"=",   BTN_EQUALS,     350, 456, 100, 50}
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
            nullptr);

        RegisterButton(handle);
        standardControls.push_back(handle);
    }

    CreateScientificControls();
    CreateTipControls();
    CreateProgrammerControls();
    CreateScientificAdvancedControls();
    CreateStatisticsControls();
    CreateFractionControls();
    CreateEquationControls();
    CreateComplexControls();
    CreateUnitControls();
    CreateDateControls();
    CreateBitToolsControls();
    CreateComputerMathControls();
    CreateSettingsControls();

    historyList = CreateWindowExW(
        WS_EX_CLIENTEDGE,
        L"LISTBOX",
        L"",
        WS_CHILD | WS_VSCROLL | LBS_NOINTEGRALHEIGHT,
        20, 52, 440, 490,
        hwnd,
        nullptr,
        nullptr,
        nullptr);

    copyHistoryButton = CreateWindowExW(
        0,
        L"BUTTON",
        L"Copy Selected",
        WS_CHILD | BS_OWNERDRAW,
        20, 554, 210, 42,
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
        250, 554, 210, 42,
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
        {L"DEG",  BTN_ANGLE,      20, 160, 80, 46},
        {L"sin",  BTN_SIN,       110, 160, 80, 46},
        {L"cos",  BTN_COS,       200, 160, 80, 46},
        {L"tan",  BTN_TAN,       290, 160, 80, 46},
        {L"asin", BTN_ASIN,      380, 160, 80, 46},

        {L"acos", BTN_ACOS,       20, 214, 80, 46},
        {L"atan", BTN_ATAN,      110, 214, 80, 46},
        {L"log",  BTN_LOG10,     200, 214, 80, 46},
        {L"ln",   BTN_LN,        290, 214, 80, 46},
        {L"n!",   BTN_FACTORIAL, 380, 214, 80, 46},

        {L"π",    BTN_PI,         20, 268, 80, 46},
        {L"e",    BTN_E,         110, 268, 80, 46},
        {L"xʸ",   BTN_POWER,     200, 268, 80, 46},
        {L"10ˣ",  BTN_TEN_POWER, 290, 268, 80, 46},
        {L"eˣ",   BTN_E_POWER,   380, 268, 80, 46},

        {L"|x|",  BTN_ABS,        20, 322, 80, 46}
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
            nullptr);

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

    addStatic(L"TIP / BILL SPLIT", 95, 52, 290, 26);

    addStatic(L"Bill amount", 95, 92, 110, 24);
    tipBillEdit = addEdit(L"0.00", 215, 88, 170, 30);

    addStatic(L"Tip %", 95, 132, 110, 24);
    tipPercentEdit = addEdit(L"20", 215, 128, 170, 30);

    addStatic(L"People", 95, 172, 110, 24);
    tipPeopleEdit = addEdit(L"1", 215, 168, 170, 30);

    addStatic(L"Quick tip", 95, 215, 110, 24);
    addButton(L"15%", BTN_TIP_15, 95, 245, 65, 38);
    addButton(L"18%", BTN_TIP_18, 170, 245, 65, 38);
    addButton(L"20%", BTN_TIP_20, 245, 245, 65, 38);
    addButton(L"25%", BTN_TIP_25, 320, 245, 65, 38);

    addButton(L"Calculate", BTN_TIP_CALCULATE, 95, 298, 290, 42);

    addStatic(L"Tip amount", 95, 364, 115, 24);
    tipAmountResult = addStatic(L"$0.00", 215, 364, 170, 24);

    addStatic(L"Total", 95, 402, 115, 24);
    tipTotalResult = addStatic(L"$0.00", 215, 402, 170, 24);

    addStatic(L"Per person", 95, 440, 115, 24);
    tipPerPersonResult = addStatic(L"$0.00", 215, 440, 170, 24);

    addStatic(L"Tip calculations stay local and are not sent anywhere.", 95, 498, 290, 44);
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

    addStatic(L"PROGRAMMER / COMPUTER TOOLS", 95, 48, 290, 24);

    addStatic(L"Integer (dec, 0x, 0b, 0o)", 95, 76, 180, 22);
    programmerValueEdit = addEdit(L"42", 95, 98, 205, 28);
    addButton(L"Convert", BTN_PROGRAMMER_CONVERT, 308, 98, 77, 28);

    programmerDecResult = addStatic(L"DEC: 42", 95, 130, 290, 20);
    programmerHexResult = addStatic(L"HEX: 2A", 95, 150, 290, 20);
    programmerOctResult = addStatic(L"OCT: 52", 95, 170, 290, 20);
    programmerBinResult = addStatic(L"BIN: 101010", 95, 190, 290, 38);

    addStatic(L"Bitwise A / B", 95, 232, 120, 20);
    bitwiseAEdit = addEdit(L"12", 95, 254, 137, 28);
    bitwiseBEdit = addEdit(L"10", 248, 254, 137, 28);

    addButton(L"AND", BTN_BIT_AND, 95, 288, 52, 28);
    addButton(L"OR",  BTN_BIT_OR, 153, 288, 52, 28);
    addButton(L"XOR", BTN_BIT_XOR, 211, 288, 52, 28);
    addButton(L"NOT", BTN_BIT_NOT, 269, 288, 52, 28);
    addButton(L"<<",  BTN_SHIFT_LEFT, 327, 288, 28, 28);
    addButton(L">>",  BTN_SHIFT_RIGHT, 357, 288, 28, 28);
    bitwiseResult = addStatic(L"Result: —", 95, 320, 290, 22);

    addStatic(L"Storage bytes", 95, 348, 105, 20);
    storageBytesEdit = addEdit(L"1073741824", 205, 344, 105, 28);
    addButton(L"Convert", BTN_STORAGE_CONVERT, 316, 344, 69, 28);
    storageResult = addStatic(L"1 GiB", 95, 376, 290, 42);

    addStatic(L"IPv4 / CIDR", 95, 422, 90, 20);
    cidrIpEdit = addEdit(L"192.168.1.10", 95, 444, 165, 28);
    cidrPrefixEdit = addEdit(L"24", 266, 444, 42, 28);
    addButton(L"Calc", BTN_CIDR_CALCULATE, 314, 444, 71, 28);
    cidrResult = addStatic(L"Network / mask / broadcast / addresses", 95, 476, 290, 66);

    addStatic(L"Character", 95, 552, 75, 20);
    charCodeEdit = addEdit(L"A", 173, 548, 70, 28);
    addButton(L"Code", BTN_CHAR_CODE, 249, 548, 60, 28);
    charCodeResult = addStatic(L"U+0041 / 65 / 0x41", 95, 582, 290, 24);
}

void MainWindow::CreateScientificAdvancedControls()
{
    auto addStatic = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            0, L"STATIC", text,
            WS_CHILD | SS_LEFT,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        scientificAdvancedControls.push_back(control);
        return control;
    };

    auto addEdit = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            WS_EX_CLIENTEDGE, L"EDIT", text,
            WS_CHILD | ES_AUTOHSCROLL | ES_RIGHT,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        scientificAdvancedControls.push_back(control);
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
        scientificAdvancedControls.push_back(control);
        return control;
    };

    addStatic(L"SCIENTIFIC+", 95, 50, 290, 26);
    addStatic(L"X / A / min", 95, 82, 130, 20);
    addStatic(L"Y / B / max / n", 250, 82, 135, 20);
    scientificAdvancedXEdit = addEdit(L"2", 95, 104, 135, 30);
    scientificAdvancedYEdit = addEdit(L"3", 250, 104, 135, 30);

    const ButtonDefinition buttons[] =
    {
        {L"sinh",   BTN_SCI_ADV_SINH,      95, 150, 65, 36},
        {L"cosh",   BTN_SCI_ADV_COSH,     170, 150, 65, 36},
        {L"tanh",   BTN_SCI_ADV_TANH,     245, 150, 65, 36},
        {L"asinh",  BTN_SCI_ADV_ASINH,    320, 150, 65, 36},

        {L"acosh",  BTN_SCI_ADV_ACOSH,     95, 194, 65, 36},
        {L"atanh",  BTN_SCI_ADV_ATANH,    170, 194, 65, 36},
        {L"cbrt",   BTN_SCI_ADV_CBRT,     245, 194, 65, 36},
        {L"y√x",    BTN_SCI_ADV_NTH_ROOT, 320, 194, 65, 36},

        {L"2^x",    BTN_SCI_ADV_TWO_POWER, 95, 238, 65, 36},
        {L"log2",   BTN_SCI_ADV_LOG2,     170, 238, 65, 36},
        {L"nCr",    BTN_SCI_ADV_NCR,      245, 238, 65, 36},
        {L"nPr",    BTN_SCI_ADV_NPR,      320, 238, 65, 36},

        {L"mod",    BTN_SCI_ADV_MOD,       95, 282, 65, 36},
        {L"floor",  BTN_SCI_ADV_FLOOR,    170, 282, 65, 36},
        {L"ceil",   BTN_SCI_ADV_CEIL,     245, 282, 65, 36},
        {L"round",  BTN_SCI_ADV_ROUND,    320, 282, 65, 36}
    };

    for(const auto& button : buttons)
        addButton(button.text, button.id, button.x, button.y, button.width, button.height);

    addButton(L"Random between X and Y", BTN_SCI_ADV_RANDOM, 95, 328, 290, 40);
    addStatic(L"Result", 95, 388, 80, 22);
    scientificAdvancedResult = addStatic(
        L"Select a function. nth root, nCr, nPr, mod, and random use both X and Y.",
        95, 414, 290, 126);
}

void MainWindow::CreateStatisticsControls()
{
    auto addStatic = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            0, L"STATIC", text,
            WS_CHILD | SS_LEFT,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        statisticsControls.push_back(control);
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
        statisticsControls.push_back(control);
        return control;
    };

    addStatic(L"STATISTICS", 95, 52, 290, 26);
    addStatic(L"Numbers (comma, semicolon, or spaces)", 95, 88, 290, 22);

    statisticsDataEdit = CreateWindowExW(
        WS_EX_CLIENTEDGE, L"EDIT", L"1, 2, 3, 4, 5",
        WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
        95, 116, 290, 80,
        hwnd, nullptr, nullptr, nullptr);
    statisticsControls.push_back(statisticsDataEdit);

    addButton(L"Calculate statistics", BTN_STATISTICS_CALCULATE, 95, 210, 290, 42);
    addStatic(L"Results", 95, 270, 100, 22);
    statisticsResult = addStatic(
        L"Count, sum, mean, median, mode, range, variance, and standard deviation will appear here.",
        95, 296, 290, 280);
}

void MainWindow::CreateFractionControls()
{
    auto addStatic = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            0, L"STATIC", text,
            WS_CHILD | SS_LEFT,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        fractionControls.push_back(control);
        return control;
    };

    auto addEdit = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            WS_EX_CLIENTEDGE, L"EDIT", text,
            WS_CHILD | ES_AUTOHSCROLL | ES_RIGHT,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        fractionControls.push_back(control);
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
        fractionControls.push_back(control);
        return control;
    };

    addStatic(L"FRACTIONS / GCD / LCM", 95, 48, 290, 26);

    addStatic(L"Fraction A", 95, 84, 90, 22);
    fractionANumeratorEdit = addEdit(L"1", 185, 80, 70, 30);
    addStatic(L"/", 261, 84, 18, 22);
    fractionADenominatorEdit = addEdit(L"2", 280, 80, 70, 30);

    addStatic(L"Fraction B", 95, 124, 90, 22);
    fractionBNumeratorEdit = addEdit(L"1", 185, 120, 70, 30);
    addStatic(L"/", 261, 124, 18, 22);
    fractionBDenominatorEdit = addEdit(L"3", 280, 120, 70, 30);

    addButton(L"A + B", BTN_FRACTION_ADD, 95, 168, 65, 36);
    addButton(L"A - B", BTN_FRACTION_SUBTRACT, 170, 168, 65, 36);
    addButton(L"A × B", BTN_FRACTION_MULTIPLY, 245, 168, 65, 36);
    addButton(L"A ÷ B", BTN_FRACTION_DIVIDE, 320, 168, 65, 36);

    addButton(L"Simplify A", BTN_FRACTION_SIMPLIFY, 95, 216, 90, 36);
    addButton(L"A → decimal", BTN_FRACTION_DECIMAL, 195, 216, 90, 36);
    addButton(L"A → mixed", BTN_FRACTION_TO_MIXED, 295, 216, 90, 36);

    addStatic(L"Mixed number: whole  numerator / denominator", 95, 272, 290, 22);
    fractionMixedWholeEdit = addEdit(L"1", 95, 298, 70, 30);
    fractionMixedNumeratorEdit = addEdit(L"1", 175, 298, 70, 30);
    addStatic(L"/", 250, 302, 18, 22);
    fractionMixedDenominatorEdit = addEdit(L"2", 270, 298, 70, 30);
    addButton(L"Mixed → improper", BTN_MIXED_TO_IMPROPER, 95, 338, 245, 36);

    addButton(L"GCD / LCM of A.num and B.num", BTN_FRACTION_GCD_LCM, 95, 392, 290, 38);

    addStatic(L"Result", 95, 452, 80, 22);
    fractionResult = addStatic(L"—", 95, 478, 290, 108);
}

void MainWindow::CreateEquationControls()
{
    auto addStatic = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            0, L"STATIC", text,
            WS_CHILD | SS_LEFT,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        equationControls.push_back(control);
        return control;
    };

    auto addEdit = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            WS_EX_CLIENTEDGE, L"EDIT", text,
            WS_CHILD | ES_AUTOHSCROLL | ES_RIGHT,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        equationControls.push_back(control);
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
        equationControls.push_back(control);
        return control;
    };

    addStatic(L"EQUATION SOLVER", 95, 52, 290, 26);

    addStatic(L"Linear: a·x + b = c", 95, 92, 290, 22);
    addStatic(L"a", 95, 126, 18, 22);
    linearAEdit = addEdit(L"2", 115, 122, 70, 30);
    addStatic(L"b", 198, 126, 18, 22);
    linearBEdit = addEdit(L"5", 218, 122, 70, 30);
    addStatic(L"c", 301, 126, 18, 22);
    linearCEdit = addEdit(L"15", 321, 122, 64, 30);
    addButton(L"Solve linear", BTN_LINEAR_SOLVE, 95, 166, 290, 40);

    addStatic(L"Quadratic: a·x² + b·x + c = 0", 95, 236, 290, 22);
    addStatic(L"a", 95, 270, 18, 22);
    quadraticAEdit = addEdit(L"1", 115, 266, 70, 30);
    addStatic(L"b", 198, 270, 18, 22);
    quadraticBEdit = addEdit(L"-3", 218, 266, 70, 30);
    addStatic(L"c", 301, 270, 18, 22);
    quadraticCEdit = addEdit(L"2", 321, 266, 64, 30);
    addButton(L"Solve quadratic", BTN_QUADRATIC_SOLVE, 95, 310, 290, 40);

    addStatic(L"Result", 95, 382, 80, 22);
    equationResult = addStatic(
        L"Linear and quadratic solutions, discriminant, and complex roots appear here.",
        95, 408, 290, 170);
}

void MainWindow::CreateComplexControls()
{
    auto addStatic = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            0, L"STATIC", text,
            WS_CHILD | SS_LEFT,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        complexControls.push_back(control);
        return control;
    };

    auto addEdit = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            WS_EX_CLIENTEDGE, L"EDIT", text,
            WS_CHILD | ES_AUTOHSCROLL | ES_RIGHT,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        complexControls.push_back(control);
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
        complexControls.push_back(control);
        return control;
    };

    addStatic(L"COMPLEX NUMBERS", 95, 52, 290, 26);
    addStatic(L"z1 = real + imag·i", 95, 94, 290, 22);
    complexReal1Edit = addEdit(L"3", 95, 120, 135, 30);
    complexImag1Edit = addEdit(L"4", 250, 120, 135, 30);

    addStatic(L"z2 = real + imag·i", 95, 166, 290, 22);
    complexReal2Edit = addEdit(L"1", 95, 192, 135, 30);
    complexImag2Edit = addEdit(L"-2", 250, 192, 135, 30);

    addButton(L"z1 + z2", BTN_COMPLEX_ADD, 95, 244, 65, 36);
    addButton(L"z1 - z2", BTN_COMPLEX_SUBTRACT, 170, 244, 65, 36);
    addButton(L"z1 × z2", BTN_COMPLEX_MULTIPLY, 245, 244, 65, 36);
    addButton(L"z1 ÷ z2", BTN_COMPLEX_DIVIDE, 320, 244, 65, 36);

    addButton(L"|z1|", BTN_COMPLEX_MAGNITUDE, 95, 296, 90, 36);
    addButton(L"conj(z1)", BTN_COMPLEX_CONJUGATE, 195, 296, 90, 36);
    addButton(L"arg(z1)", BTN_COMPLEX_ARGUMENT, 295, 296, 90, 36);

    addStatic(L"Result", 95, 362, 80, 22);
    complexResult = addStatic(
        L"Argument is displayed in degrees and radians.",
        95, 388, 290, 178);
}

void MainWindow::CreateUnitControls()
{
    auto addStatic = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            0, L"STATIC", text,
            WS_CHILD | SS_LEFT,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        unitControls.push_back(control);
        return control;
    };

    auto addEdit = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            WS_EX_CLIENTEDGE, L"EDIT", text,
            WS_CHILD | ES_AUTOHSCROLL | ES_RIGHT,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        unitControls.push_back(control);
        return control;
    };

    auto addCombo = [&](int id, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            0, L"COMBOBOX", L"",
            WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
            x, y, width, height,
            hwnd,
            reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
            nullptr, nullptr);
        unitControls.push_back(control);
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
        unitControls.push_back(control);
        return control;
    };

    addStatic(L"UNIT CONVERTER", 95, 50, 290, 26);

    addStatic(L"Category", 95, 88, 110, 22);
    unitCategoryCombo = addCombo(COMBO_UNIT_CATEGORY, 95, 112, 290, 240);

    const wchar_t* categories[] =
    {
        L"Length", L"Mass", L"Temperature", L"Area", L"Volume", L"Speed",
        L"Pressure", L"Energy", L"Power", L"Angle", L"Data", L"Manual Rate"
    };
    for(const wchar_t* category : categories)
        SendMessageW(unitCategoryCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(category));
    SendMessageW(unitCategoryCombo, CB_SETCURSEL, 0, 0);

    addStatic(L"From", 95, 162, 110, 22);
    unitFromCombo = addCombo(COMBO_UNIT_FROM, 95, 186, 135, 240);
    addStatic(L"To", 250, 162, 110, 22);
    unitToCombo = addCombo(COMBO_UNIT_TO, 250, 186, 135, 240);

    addStatic(L"Value", 95, 236, 110, 22);
    unitValueEdit = addEdit(L"1", 95, 260, 290, 30);

    addStatic(L"Manual rate (target units per base unit; Manual Rate only)", 95, 306, 290, 38);
    unitRateEdit = addEdit(L"1", 95, 348, 290, 30);

    addButton(L"Convert", BTN_UNIT_CONVERT, 95, 394, 290, 42);
    addStatic(L"Result", 95, 458, 80, 22);
    unitResult = addStatic(L"—", 95, 484, 290, 76);

    UpdateUnitChoices();
}

void MainWindow::CreateDateControls()
{
    auto addStatic = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            0, L"STATIC", text,
            WS_CHILD | SS_LEFT,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        dateControls.push_back(control);
        return control;
    };

    auto addEdit = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            WS_EX_CLIENTEDGE, L"EDIT", text,
            WS_CHILD | ES_AUTOHSCROLL,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        dateControls.push_back(control);
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
        dateControls.push_back(control);
        return control;
    };

    addStatic(L"DATE / TIME TOOLS", 95, 48, 290, 26);

    addStatic(L"Date A (YYYY-MM-DD)", 95, 82, 145, 20);
    dateAEdit = addEdit(L"2026-10-03", 95, 104, 135, 28);
    addStatic(L"Date B", 250, 82, 90, 20);
    dateBEdit = addEdit(L"2027-01-01", 250, 104, 135, 28);

    addButton(L"Days between", BTN_DATE_DIFFERENCE, 95, 144, 135, 34);
    addButton(L"Inspect Date A", BTN_DATE_INSPECT, 250, 144, 135, 34);

    addStatic(L"Add days to Date A", 95, 192, 130, 20);
    dateOffsetEdit = addEdit(L"30", 230, 188, 65, 28);
    addButton(L"Add", BTN_DATE_ADD_DAYS, 305, 188, 80, 28);

    addStatic(L"Time for Date A (HH:MM:SS)", 95, 230, 190, 20);
    dateTimeEdit = addEdit(L"00:00:00", 95, 252, 135, 28);
    addButton(L"→ Unix UTC", BTN_DATE_TO_UNIX, 250, 252, 135, 28);

    addStatic(L"Unix timestamp", 95, 294, 110, 20);
    unixTimestampEdit = addEdit(L"0", 95, 316, 135, 28);
    addButton(L"Unix → UTC", BTN_DATE_FROM_UNIX, 250, 316, 135, 28);

    addStatic(L"Duration seconds", 95, 358, 120, 20);
    durationSecondsEdit = addEdit(L"3661", 95, 380, 135, 28);
    addButton(L"→ H:M:S", BTN_DURATION_CONVERT, 250, 380, 135, 28);

    addStatic(L"Result", 95, 430, 80, 22);
    dateResult = addStatic(
        L"Date calculations use the proleptic Gregorian calendar. Unix conversions are UTC.",
        95, 456, 290, 126);
}

void MainWindow::CreateBitToolsControls()
{
    auto addStatic = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            0, L"STATIC", text,
            WS_CHILD | SS_LEFT,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        bitToolsControls.push_back(control);
        return control;
    };

    auto addEdit = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            WS_EX_CLIENTEDGE, L"EDIT", text,
            WS_CHILD | ES_AUTOHSCROLL,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        bitToolsControls.push_back(control);
        return control;
    };

    auto addCombo = [&](int id, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            0, L"COMBOBOX", L"",
            WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
            x, y, width, height,
            hwnd,
            reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
            nullptr, nullptr);
        bitToolsControls.push_back(control);
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
        bitToolsControls.push_back(control);
        return control;
    };

    addStatic(L"ADVANCED BIT TOOLS", 95, 44, 290, 24);
    addStatic(L"Value (dec, 0x, 0b, 0o)", 95, 72, 180, 20);
    bitToolValueEdit = addEdit(L"255", 95, 94, 190, 28);
    addButton(L"Convert", BTN_BITTOOLS_CONVERT, 295, 94, 90, 28);

    bitToolWordSizeCombo = addCombo(0, 95, 130, 135, 180);
    for(const wchar_t* text : {L"8-bit", L"16-bit", L"32-bit", L"64-bit"})
        SendMessageW(bitToolWordSizeCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(text));
    SendMessageW(bitToolWordSizeCombo, CB_SETCURSEL, 3, 0);

    bitToolSignedCombo = addCombo(0, 250, 130, 135, 120);
    SendMessageW(bitToolSignedCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Unsigned"));
    SendMessageW(bitToolSignedCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Signed"));
    SendMessageW(bitToolSignedCombo, CB_SETCURSEL, 0, 0);

    bitToolResult = addStatic(L"Grouped binary / signed interpretation / popcount", 95, 170, 290, 118);

    addStatic(L"Bit index", 95, 296, 70, 20);
    bitToolIndexEdit = addEdit(L"0", 165, 292, 55, 28);
    addButton(L"Set", BTN_BITTOOLS_SET, 230, 292, 48, 28);
    addButton(L"Clear", BTN_BITTOOLS_CLEAR, 282, 292, 48, 28);
    addButton(L"Toggle", BTN_BITTOOLS_TOGGLE, 334, 292, 51, 28);

    addButton(L"ROL 1", BTN_BITTOOLS_ROL, 95, 330, 90, 30);
    addButton(L"ROR 1", BTN_BITTOOLS_ROR, 195, 330, 90, 30);
    addButton(L"Endian reverse", BTN_BITTOOLS_ENDIAN, 295, 330, 90, 30);

    addStatic(L"Unicode code point (decimal or 0x hex)", 95, 372, 240, 20);
    unicodeCodeEdit = addEdit(L"65", 95, 394, 135, 28);
    addButton(L"Lookup", BTN_UNICODE_LOOKUP, 250, 394, 135, 28);
    unicodeResult = addStatic(L"U+0041 = A", 95, 428, 290, 24);

    addStatic(L"ASCII browser (0–127)", 95, 458, 180, 20);
    asciiList = CreateWindowExW(
        WS_EX_CLIENTEDGE, L"LISTBOX", L"",
        WS_CHILD | WS_VSCROLL | LBS_NOINTEGRALHEIGHT,
        95, 480, 290, 104,
        hwnd, nullptr, nullptr, nullptr);
    bitToolsControls.push_back(asciiList);

    for(int code = 0; code <= 127; ++code)
    {
        std::wostringstream item;
        item << std::setw(3) << code << L"   0x"
             << std::uppercase << std::hex << std::setw(2) << std::setfill(L'0') << code
             << std::dec << std::setfill(L' ') << L"   " << AsciiDescription(code);
        const std::wstring label = item.str();
        SendMessageW(asciiList, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(label.c_str()));
    }
}

void MainWindow::CreateComputerMathControls()
{
    auto addStatic = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            0, L"STATIC", text,
            WS_CHILD | SS_LEFT,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        computerMathControls.push_back(control);
        return control;
    };

    auto addEdit = [&](const wchar_t* text, int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            WS_EX_CLIENTEDGE, L"EDIT", text,
            WS_CHILD | ES_AUTOHSCROLL | ES_RIGHT,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        computerMathControls.push_back(control);
        return control;
    };

    auto addCombo = [&](int x, int y, int width, int height) -> HWND
    {
        HWND control = CreateWindowExW(
            0, L"COMBOBOX", L"",
            WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
            x, y, width, height,
            hwnd, nullptr, nullptr, nullptr);
        computerMathControls.push_back(control);
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
        computerMathControls.push_back(control);
        return control;
    };

    addStatic(L"COMPUTER / STORAGE MATH", 95, 42, 290, 24);

    addStatic(L"Transfer time", 95, 70, 110, 20);
    transferSizeEdit = addEdit(L"10", 95, 92, 70, 28);
    transferSizeUnitCombo = addCombo(170, 92, 75, 160);
    for(const wchar_t* u : {L"MB", L"GB", L"TB", L"MiB", L"GiB"})
        SendMessageW(transferSizeUnitCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(u));
    SendMessageW(transferSizeUnitCombo, CB_SETCURSEL, 1, 0);

    transferRateEdit = addEdit(L"100", 250, 92, 70, 28);
    transferRateUnitCombo = addCombo(325, 92, 60, 160);
    for(const wchar_t* u : {L"Mbps", L"Gbps", L"MB/s", L"GB/s"})
        SendMessageW(transferRateUnitCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(u));
    SendMessageW(transferRateUnitCombo, CB_SETCURSEL, 0, 0);
    addButton(L"Calculate transfer", BTN_COMPUTER_TRANSFER, 95, 126, 290, 30);

    addStatic(L"Bitrate → file size (seconds / Mbps)", 95, 168, 250, 20);
    bitrateDurationEdit = addEdit(L"3600", 95, 190, 135, 28);
    bitrateRateEdit = addEdit(L"8", 250, 190, 135, 28);
    addButton(L"Calculate file size", BTN_COMPUTER_BITRATE, 95, 224, 290, 30);

    addStatic(L"Resolution / aspect / PPI", 95, 266, 200, 20);
    resolutionWidthEdit = addEdit(L"1920", 95, 288, 85, 28);
    resolutionHeightEdit = addEdit(L"1080", 190, 288, 85, 28);
    resolutionDiagonalEdit = addEdit(L"24", 285, 288, 100, 28);
    addStatic(L"width", 95, 318, 70, 18);
    addStatic(L"height", 190, 318, 70, 18);
    addStatic(L"diagonal in", 285, 318, 100, 18);
    addButton(L"Calculate display", BTN_COMPUTER_RESOLUTION, 95, 340, 290, 30);

    addStatic(L"RAID usable capacity estimate", 95, 382, 210, 20);
    raidDriveCountEdit = addEdit(L"4", 95, 404, 65, 28);
    raidDriveSizeEdit = addEdit(L"4", 170, 404, 65, 28);
    raidModeCombo = addCombo(245, 404, 140, 160);
    for(const wchar_t* mode : {L"RAID 0", L"RAID 1", L"RAID 5", L"RAID 6", L"RAID 10"})
        SendMessageW(raidModeCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(mode));
    SendMessageW(raidModeCombo, CB_SETCURSEL, 2, 0);
    addStatic(L"drives", 95, 434, 65, 18);
    addStatic(L"TB each", 170, 434, 65, 18);
    addButton(L"Calculate RAID", BTN_COMPUTER_RAID, 95, 456, 290, 30);

    addStatic(L"Result", 95, 500, 80, 20);
    computerMathResult = addStatic(
        L"Transfer, bitrate, display, or RAID results appear here.",
        95, 522, 290, 70);
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

    addStatic(L"SETTINGS / APPEARANCE", 95, 52, 290, 26);

    addStatic(L"Theme", 95, 96, 90, 24);
    addButton(L"Dark", BTN_THEME_DARK, 95, 126, 135, 40);
    addButton(L"Light", BTN_THEME_LIGHT, 250, 126, 135, 40);

    addStatic(L"Local background", 95, 192, 150, 24);
    addButton(L"Choose image / GIF", BTN_BACKGROUND_CHOOSE, 95, 222, 185, 40);
    addButton(L"Clear", BTN_BACKGROUND_CLEAR, 290, 222, 95, 40);

    backgroundStatus = addStatic(L"No background selected", 95, 274, 290, 44);

    scaleModeButton = addButton(L"Scale: Cover", BTN_BACKGROUND_SCALE, 95, 336, 135, 40);
    overlayButton = addButton(L"Overlay: 45%", BTN_BACKGROUND_OVERLAY, 250, 336, 135, 40);

    addStatic(
        L"Supported local files: PNG, JPG/JPEG, GIF, BMP. "
        L"Files stay on your computer and are never bundled with Nyxoryth.",
        95, 400, 290, 72);

    alwaysOnTopButton = addButton(
        L"Always on top: Off", BTN_ALWAYS_ON_TOP, 95, 468, 290, 38);

    resetSettingsButton = addButton(
        L"Reset Settings", BTN_RESET_SETTINGS, 95, 516, 290, 38);

    aboutButton = addButton(
        L"About Nyxoryth", BTN_ABOUT, 95, 564, 290, 38);

    addStatic(L"MIT License • Local / offline calculator", 95, 608, 290, 18);

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
        (id == BTN_THEME_DARK && darkTheme) ||
        (id == BTN_THEME_LIGHT && !darkTheme) ||
        (id == BTN_ALWAYS_ON_TOP && alwaysOnTop);

    const bool accent =
        activeState ||
        id == BTN_EQUALS ||
        id == BTN_TIP_CALCULATE ||
        id == BTN_PROGRAMMER_CONVERT ||
        id == BTN_CIDR_CALCULATE ||
        id == BTN_BACKGROUND_CHOOSE ||
        id == BTN_STATISTICS_CALCULATE ||
        id == BTN_UNIT_CONVERT ||
        id == BTN_LINEAR_SOLVE ||
        id == BTN_QUADRATIC_SOLVE ||
        id == BTN_COMPUTER_TRANSFER ||
        id == BTN_COMPUTER_BITRATE ||
        id == BTN_COMPUTER_RESOLUTION ||
        id == BTN_COMPUTER_RAID;

    const bool operation =
        id == BTN_PLUS || id == BTN_MINUS ||
        id == BTN_MULTIPLY || id == BTN_DIVIDE ||
        id == BTN_POWER;

    const bool navigation =
        id == BTN_NAV_MENU || id == BTN_COPY_RESULT ||
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
                 lstrcmpiW(className, L"LISTBOX") == 0 ||
                 lstrcmpiW(className, L"COMBOBOX") == 0))
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

void MainWindow::ShowNavigationMenu()
{
    if(!hwnd || !navigationButton)
        return;

    HMENU menu = CreatePopupMenu();
    if(!menu)
        return;

    auto addItem = [&](UINT id, const wchar_t* label, bool checked)
    {
        AppendMenuW(
            menu,
            MF_STRING | (checked ? MF_CHECKED : MF_UNCHECKED),
            id,
            label);
    };

    const bool standardMode =
        !scientificMode && !scientificAdvancedMode && !statisticsMode &&
        !fractionMode && !equationMode && !complexMode && !tipMode &&
        !unitMode && !dateMode && !programmerMode && !bitToolsMode &&
        !computerMathMode && !settingsMode && !historyVisible;

    addItem(MENU_STANDARD, L"Standard", standardMode);
    addItem(BTN_MODE, L"Scientific", scientificMode);
    addItem(BTN_SCIENTIFIC_ADVANCED_MODE, L"Scientific+", scientificAdvancedMode);
    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
    addItem(BTN_STATISTICS_MODE, L"Statistics", statisticsMode);
    addItem(BTN_FRACTIONS_MODE, L"Fractions / GCD / LCM", fractionMode);
    addItem(BTN_EQUATIONS_MODE, L"Equation Solver", equationMode);
    addItem(BTN_COMPLEX_MODE, L"Complex Numbers", complexMode);
    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
    addItem(BTN_TIP_MODE, L"Tip / Bill Split", tipMode);
    addItem(BTN_UNIT_MODE, L"Unit Converter", unitMode);
    addItem(BTN_DATE_MODE, L"Date / Time Tools", dateMode);
    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
    addItem(BTN_PROGRAMMER_MODE, L"Programmer / Computer Tools", programmerMode);
    addItem(BTN_BITTOOLS_MODE, L"Advanced Bit Tools", bitToolsMode);
    addItem(BTN_COMPUTER_MATH_MODE, L"Computer / Storage Math", computerMathMode);
    addItem(BTN_HISTORY, L"History", historyVisible);
    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
    addItem(BTN_SETTINGS_MODE, L"Settings / Appearance", settingsMode);

    RECT buttonRect{};
    GetWindowRect(navigationButton, &buttonRect);

    const UINT selected = TrackPopupMenu(
        menu,
        TPM_RETURNCMD | TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON,
        buttonRect.left,
        buttonRect.bottom + 2,
        0,
        hwnd,
        nullptr);

    DestroyMenu(menu);

    if(selected != 0)
        ActivateScreen(static_cast<int>(selected));
}

void MainWindow::ActivateScreen(int commandId)
{
    scientificMode = false;
    scientificAdvancedMode = false;
    statisticsMode = false;
    fractionMode = false;
    equationMode = false;
    complexMode = false;
    tipMode = false;
    unitMode = false;
    dateMode = false;
    programmerMode = false;
    bitToolsMode = false;
    computerMathMode = false;
    settingsMode = false;
    historyVisible = false;

    switch(commandId)
    {
        case BTN_MODE:
            scientificMode = true;
            lastMode = L"Scientific";
            break;

        case BTN_SCIENTIFIC_ADVANCED_MODE:
            scientificAdvancedMode = true;
            lastMode = L"ScientificAdvanced";
            break;

        case BTN_STATISTICS_MODE:
            statisticsMode = true;
            lastMode = L"Statistics";
            break;

        case BTN_FRACTIONS_MODE:
            fractionMode = true;
            lastMode = L"Fractions";
            break;

        case BTN_EQUATIONS_MODE:
            equationMode = true;
            lastMode = L"Equations";
            break;

        case BTN_COMPLEX_MODE:
            complexMode = true;
            lastMode = L"Complex";
            break;

        case BTN_TIP_MODE:
            tipMode = true;
            lastMode = L"Tip";
            break;

        case BTN_UNIT_MODE:
            unitMode = true;
            lastMode = L"Units";
            break;

        case BTN_DATE_MODE:
            dateMode = true;
            lastMode = L"DateTime";
            break;

        case BTN_PROGRAMMER_MODE:
            programmerMode = true;
            lastMode = L"Programmer";
            break;

        case BTN_BITTOOLS_MODE:
            bitToolsMode = true;
            lastMode = L"BitTools";
            break;

        case BTN_COMPUTER_MATH_MODE:
            computerMathMode = true;
            lastMode = L"ComputerMath";
            break;

        case BTN_SETTINGS_MODE:
            settingsMode = true;
            lastMode = L"Settings";
            UpdateSettingsText();
            break;

        case BTN_HISTORY:
            historyVisible = true;
            lastMode = L"History";
            break;

        case MENU_STANDARD:
        default:
            lastMode = L"Standard";
            break;
    }

    SaveSettings();
    UpdateModeText();
    UpdateWindowLayout();
    FocusPrimaryControlForCurrentScreen();

    if(navigationButton)
        InvalidateRect(navigationButton, nullptr, FALSE);
}

void MainWindow::ToggleScientificMode()
{
    ActivateScreen(scientificMode ? MENU_STANDARD : BTN_MODE);
}

void MainWindow::ToggleTipMode()
{
    ActivateScreen(tipMode ? MENU_STANDARD : BTN_TIP_MODE);
}

void MainWindow::ToggleProgrammerMode()
{
    ActivateScreen(programmerMode ? MENU_STANDARD : BTN_PROGRAMMER_MODE);
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

    savedWindowWidth = GetPrivateProfileIntW(
        L"General", L"WindowWidth", 500, settingsPath.c_str());

    savedWindowHeight = GetPrivateProfileIntW(
        L"General", L"WindowHeight", 670, settingsPath.c_str());

    // Layout version 2 replaces the old side-by-side mode panels with a
    // single-screen hamburger menu. Reset only the stored size once so a
    // width that was automatically enlarged by an older build does not carry
    // into the new layout. User resizing is saved normally after this upgrade.
    const int layoutVersion = GetPrivateProfileIntW(
        L"General", L"LayoutVersion", 0, settingsPath.c_str());
    if(layoutVersion < 2)
    {
        savedWindowWidth = 500;
        savedWindowHeight = 670;
    }

    // Keep corrupted/manual settings from creating unusable windows.
    if(savedWindowWidth < 500 || savedWindowWidth > 10000)
        savedWindowWidth = 500;
    if(savedWindowHeight < 670 || savedWindowHeight > 10000)
        savedWindowHeight = 670;

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

    WritePrivateProfileStringW(
        L"General",
        L"LayoutVersion",
        L"2",
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

    const std::wstring width = std::to_wstring(savedWindowWidth);
    WritePrivateProfileStringW(
        L"General", L"WindowWidth", width.c_str(), settingsPath.c_str());

    const std::wstring height = std::to_wstring(savedWindowHeight);
    WritePrivateProfileStringW(
        L"General", L"WindowHeight", height.c_str(), settingsPath.c_str());
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

    MarkBackgroundDirty();

    if(hwnd)
    {
        RedrawWindow(
            hwnd,
            nullptr,
            nullptr,
            RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
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
    MarkBackgroundDirty();
    SaveSettings();
    UpdateSettingsText();

    if(hwnd)
        InvalidateRect(hwnd, nullptr, FALSE);
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

                StartBackgroundTimer();
            }
        }
    }

    MarkBackgroundDirty();
    if(hwnd)
        InvalidateRect(hwnd, nullptr, FALSE);

    return true;
}

void MainWindow::ReleaseBackgroundImage()
{
    StopBackgroundTimer();

    delete backgroundImage;
    backgroundImage = nullptr;
    backgroundFrameDelays.clear();
    backgroundFrameCount = 0;
    backgroundFrameIndex = 0;
    MarkBackgroundDirty();
}

void MainWindow::StartBackgroundTimer()
{
    if(!hwnd || backgroundAnimationPaused || windowMinimized ||
       !backgroundImage || backgroundFrameCount <= 1)
    {
        return;
    }

    SetTimer(
        hwnd,
        kBackgroundTimerId,
        CurrentBackgroundFrameDelay(),
        nullptr);
}

void MainWindow::StopBackgroundTimer()
{
    if(hwnd)
        KillTimer(hwnd, kBackgroundTimerId);
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
    if(backgroundAnimationPaused || windowMinimized ||
       !backgroundImage || backgroundFrameCount <= 1)
    {
        return;
    }

    backgroundFrameIndex =
        (backgroundFrameIndex + 1) % backgroundFrameCount;

    backgroundImage->SelectActiveFrame(
        &backgroundFrameDimension,
        backgroundFrameIndex);

    MarkBackgroundDirty();

    // GIF delays can vary per frame, so refresh the timer interval. The paint
    // itself remains asynchronous so input/move/resize messages are not blocked
    // by a forced full-window render on every timer tick.
    SetTimer(
        hwnd,
        kBackgroundTimerId,
        CurrentBackgroundFrameDelay(),
        nullptr);

    InvalidateRect(hwnd, nullptr, FALSE);
}

void MainWindow::ToggleBackgroundScaleMode()
{
    backgroundCover = !backgroundCover;
    MarkBackgroundDirty();
    SaveSettings();
    UpdateSettingsText();

    if(hwnd)
        InvalidateRect(hwnd, nullptr, FALSE);
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
    MarkBackgroundDirty();
    SaveSettings();
    UpdateSettingsText();

    if(hwnd)
        InvalidateRect(hwnd, nullptr, FALSE);
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
    scientificAdvancedMode = false;
    statisticsMode = false;
    fractionMode = false;
    equationMode = false;
    complexMode = false;
    tipMode = false;
    unitMode = false;
    dateMode = false;
    programmerMode = false;
    bitToolsMode = false;
    computerMathMode = false;
    settingsMode = false;
    historyVisible = false;
    lastMode = L"Standard";

    savedWindowX = CW_USEDEFAULT;
    savedWindowY = CW_USEDEFAULT;
    savedWindowWidth = 500;
    savedWindowHeight = 670;

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
        L"Native Windows calculator with Standard, Scientific, Statistics, Fractions, "
        L"Equations, Complex Numbers, Unit/Date tools, Programmer/Computer tools, "
        L"History, themes, and user-selected local backgrounds.\r\n\r\n"
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

    WINDOWPLACEMENT placement{};
    placement.length = sizeof(placement);

    if(GetWindowPlacement(hwnd, &placement))
    {
        // rcNormalPosition preserves the user's normal restored size even when
        // Nyxoryth is currently maximized.
        const RECT& rect = placement.rcNormalPosition;
        savedWindowX = rect.left;
        savedWindowY = rect.top;
        savedWindowWidth = std::max(500, static_cast<int>(rect.right - rect.left));
        savedWindowHeight = std::max(670, static_cast<int>(rect.bottom - rect.top));
    }
}

void MainWindow::ApplyStartupMode()
{
    scientificMode = false;
    scientificAdvancedMode = false;
    statisticsMode = false;
    fractionMode = false;
    equationMode = false;
    complexMode = false;
    tipMode = false;
    unitMode = false;
    dateMode = false;
    programmerMode = false;
    bitToolsMode = false;
    computerMathMode = false;
    settingsMode = false;
    historyVisible = false;

    if(lastMode == L"Scientific")
        scientificMode = true;
    else if(lastMode == L"ScientificAdvanced")
        scientificAdvancedMode = true;
    else if(lastMode == L"Statistics")
        statisticsMode = true;
    else if(lastMode == L"Fractions")
        fractionMode = true;
    else if(lastMode == L"Equations")
        equationMode = true;
    else if(lastMode == L"Complex")
        complexMode = true;
    else if(lastMode == L"Tip")
        tipMode = true;
    else if(lastMode == L"Units")
        unitMode = true;
    else if(lastMode == L"DateTime")
        dateMode = true;
    else if(lastMode == L"Programmer")
        programmerMode = true;
    else if(lastMode == L"BitTools")
        bitToolsMode = true;
    else if(lastMode == L"ComputerMath")
        computerMathMode = true;
    else if(lastMode == L"Settings")
        settingsMode = true;
    else if(lastMode == L"History")
        historyVisible = true;
    else
        lastMode = L"Standard";
}

void MainWindow::MarkBackgroundDirty()
{
    backgroundFrameDirty = true;
}

bool MainWindow::EnsureBackgroundBuffer(HDC targetDc, int width, int height)
{
    if(!targetDc || width <= 0 || height <= 0)
        return false;

    if(backgroundBufferDc && backgroundBufferBitmap &&
       backgroundBufferWidth == width && backgroundBufferHeight == height)
    {
        return true;
    }

    ReleaseBackgroundBuffer();

    backgroundBufferDc = CreateCompatibleDC(targetDc);
    if(!backgroundBufferDc)
        return false;

    backgroundBufferBitmap = CreateCompatibleBitmap(targetDc, width, height);
    if(!backgroundBufferBitmap)
    {
        DeleteDC(backgroundBufferDc);
        backgroundBufferDc = nullptr;
        return false;
    }

    backgroundBufferPreviousBitmap =
        SelectObject(backgroundBufferDc, backgroundBufferBitmap);

    if(!backgroundBufferPreviousBitmap ||
       backgroundBufferPreviousBitmap == HGDI_ERROR)
    {
        DeleteObject(backgroundBufferBitmap);
        backgroundBufferBitmap = nullptr;
        DeleteDC(backgroundBufferDc);
        backgroundBufferDc = nullptr;
        backgroundBufferPreviousBitmap = nullptr;
        return false;
    }

    backgroundBufferWidth = width;
    backgroundBufferHeight = height;
    MarkBackgroundDirty();
    return true;
}

void MainWindow::ReleaseBackgroundBuffer()
{
    if(backgroundBufferDc && backgroundBufferPreviousBitmap &&
       backgroundBufferPreviousBitmap != HGDI_ERROR)
    {
        SelectObject(backgroundBufferDc, backgroundBufferPreviousBitmap);
    }

    if(backgroundBufferBitmap)
        DeleteObject(backgroundBufferBitmap);

    if(backgroundBufferDc)
        DeleteDC(backgroundBufferDc);

    backgroundBufferDc = nullptr;
    backgroundBufferBitmap = nullptr;
    backgroundBufferPreviousBitmap = nullptr;
    backgroundBufferWidth = 0;
    backgroundBufferHeight = 0;
    backgroundFrameDirty = true;
}

int MainWindow::RequiredClientWidth() const
{
    return 480;
}

int MainWindow::RequiredClientHeight() const
{
    return 630;
}

void MainWindow::EnsureMinimumWindowSize()
{
    if(!hwnd)
        return;

    RECT required{0, 0, RequiredClientWidth(), RequiredClientHeight()};
    const DWORD style = static_cast<DWORD>(GetWindowLongPtrW(hwnd, GWL_STYLE));
    const DWORD exStyle = static_cast<DWORD>(GetWindowLongPtrW(hwnd, GWL_EXSTYLE));

    if(!AdjustWindowRectEx(&required, style, FALSE, exStyle))
        return;

    const int minimumWidth = required.right - required.left;
    const int minimumHeight = required.bottom - required.top;

    RECT current{};
    if(!GetWindowRect(hwnd, &current))
        return;

    const int currentWidth = current.right - current.left;
    const int currentHeight = current.bottom - current.top;
    const int nextWidth = std::max(currentWidth, minimumWidth);
    const int nextHeight = std::max(currentHeight, minimumHeight);

    if(nextWidth != currentWidth || nextHeight != currentHeight)
    {
        SetWindowPos(
            hwnd,
            nullptr,
            0, 0,
            nextWidth,
            nextHeight,
            SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

void MainWindow::PaintBackground(HDC hdc)
{
    RECT client{};
    GetClientRect(hwnd, &client);

    const int width = client.right - client.left;
    const int height = client.bottom - client.top;

    if(width <= 0 || height <= 0)
        return;

    if(!EnsureBackgroundBuffer(hdc, width, height))
        return;

    // Re-render only when something actually changed: GIF frame, theme,
    // background options, image, or client size. Repaint requests caused by
    // moving/covering the window can then be served by a fast BitBlt.
    if(backgroundFrameDirty)
    {
        Gdiplus::Graphics graphics(backgroundBufferDc);
        graphics.SetCompositingMode(Gdiplus::CompositingModeSourceOver);

        const bool animated = backgroundImage && backgroundFrameCount > 1;
        graphics.SetCompositingQuality(
            animated
                ? Gdiplus::CompositingQualityHighSpeed
                : Gdiplus::CompositingQualityHighQuality);
        graphics.SetInterpolationMode(
            animated
                ? Gdiplus::InterpolationModeHighQualityBilinear
                : Gdiplus::InterpolationModeHighQualityBicubic);
        graphics.SetPixelOffsetMode(
            animated
                ? Gdiplus::PixelOffsetModeHalf
                : Gdiplus::PixelOffsetModeHighQuality);
        graphics.SetSmoothingMode(
            animated
                ? Gdiplus::SmoothingModeNone
                : Gdiplus::SmoothingModeHighQuality);

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

        backgroundFrameDirty = false;
    }

    BitBlt(
        hdc,
        0, 0,
        width, height,
        backgroundBufferDc,
        0, 0,
        SRCCOPY);
}

void MainWindow::ToggleSettingsMode()
{
    ActivateScreen(settingsMode ? MENU_STANDARD : BTN_SETTINGS_MODE);
}

void MainWindow::SetTipPreset(double percent)
{
    if(tipPercentEdit)
        SetWindowTextW(tipPercentEdit, FormatNumber(percent).c_str());

    CalculateTip();
}

void MainWindow::RunAdvancedScientific(int operation)
{
    double x = 0.0;
    double y = 0.0;

    if(!TryParseDouble(GetWindowTextString(scientificAdvancedXEdit), x))
    {
        SetWindowTextString(scientificAdvancedResult, L"Enter a valid finite X value.");
        return;
    }

    const bool needsY =
        operation == BTN_SCI_ADV_NTH_ROOT ||
        operation == BTN_SCI_ADV_NCR ||
        operation == BTN_SCI_ADV_NPR ||
        operation == BTN_SCI_ADV_MOD ||
        operation == BTN_SCI_ADV_RANDOM;

    if(needsY && !TryParseDouble(GetWindowTextString(scientificAdvancedYEdit), y))
    {
        SetWindowTextString(scientificAdvancedResult, L"Enter a valid finite Y value.");
        return;
    }

    double result = 0.0;
    std::wstring label;

    switch(operation)
    {
        case BTN_SCI_ADV_SINH:
            result = std::sinh(x);
            label = L"sinh(X)";
            break;

        case BTN_SCI_ADV_COSH:
            result = std::cosh(x);
            label = L"cosh(X)";
            break;

        case BTN_SCI_ADV_TANH:
            result = std::tanh(x);
            label = L"tanh(X)";
            break;

        case BTN_SCI_ADV_ASINH:
            result = std::asinh(x);
            label = L"asinh(X)";
            break;

        case BTN_SCI_ADV_ACOSH:
            if(x < 1.0)
            {
                SetWindowTextString(scientificAdvancedResult, L"acosh(X) requires X ≥ 1.");
                return;
            }
            result = std::acosh(x);
            label = L"acosh(X)";
            break;

        case BTN_SCI_ADV_ATANH:
            if(x <= -1.0 || x >= 1.0)
            {
                SetWindowTextString(scientificAdvancedResult, L"atanh(X) requires -1 < X < 1.");
                return;
            }
            result = std::atanh(x);
            label = L"atanh(X)";
            break;

        case BTN_SCI_ADV_CBRT:
            result = std::cbrt(x);
            label = L"cbrt(X)";
            break;

        case BTN_SCI_ADV_NTH_ROOT:
        {
            if(y == 0.0)
            {
                SetWindowTextString(scientificAdvancedResult, L"The root degree Y cannot be zero.");
                return;
            }

            if(x < 0.0)
            {
                const double roundedDegree = std::round(y);
                if(std::fabs(y - roundedDegree) > 1e-12 ||
                   (static_cast<long long>(std::llround(roundedDegree)) % 2LL) == 0)
                {
                    SetWindowTextString(
                        scientificAdvancedResult,
                        L"A negative X needs an odd integer root degree.");
                    return;
                }
                result = -std::pow(-x, 1.0 / y);
            }
            else
            {
                result = std::pow(x, 1.0 / y);
            }
            label = L"Yth root of X";
            break;
        }

        case BTN_SCI_ADV_TWO_POWER:
            result = std::pow(2.0, x);
            label = L"2^X";
            break;

        case BTN_SCI_ADV_LOG2:
            if(x <= 0.0)
            {
                SetWindowTextString(scientificAdvancedResult, L"log2(X) requires X > 0.");
                return;
            }
            result = std::log2(x);
            label = L"log2(X)";
            break;

        case BTN_SCI_ADV_NCR:
        case BTN_SCI_ADV_NPR:
        {
            const double roundedN = std::round(x);
            const double roundedR = std::round(y);
            if(std::fabs(x - roundedN) > 1e-12 ||
               std::fabs(y - roundedR) > 1e-12 ||
               roundedN < 0.0 || roundedR < 0.0 ||
               roundedR > roundedN || roundedN > 170.0)
            {
                SetWindowTextString(
                    scientificAdvancedResult,
                    L"nCr/nPr require integer 0 ≤ R ≤ N ≤ 170. Use X=N and Y=R.");
                return;
            }

            const unsigned int n = static_cast<unsigned int>(roundedN);
            const unsigned int r = static_cast<unsigned int>(roundedR);
            long double value = 1.0L;

            if(operation == BTN_SCI_ADV_NCR)
            {
                const unsigned int k = std::min(r, n - r);
                for(unsigned int i = 1; i <= k; ++i)
                {
                    value *= static_cast<long double>(n - k + i);
                    value /= static_cast<long double>(i);
                }
                label = L"N choose R";
            }
            else
            {
                for(unsigned int i = 0; i < r; ++i)
                    value *= static_cast<long double>(n - i);
                label = L"N permute R";
            }

            result = static_cast<double>(value);
            break;
        }

        case BTN_SCI_ADV_MOD:
            if(y == 0.0)
            {
                SetWindowTextString(scientificAdvancedResult, L"Modulo by zero is undefined.");
                return;
            }
            result = std::fmod(x, y);
            label = L"X mod Y";
            break;

        case BTN_SCI_ADV_FLOOR:
            result = std::floor(x);
            label = L"floor(X)";
            break;

        case BTN_SCI_ADV_CEIL:
            result = std::ceil(x);
            label = L"ceil(X)";
            break;

        case BTN_SCI_ADV_ROUND:
            result = std::round(x);
            label = L"round(X)";
            break;

        case BTN_SCI_ADV_RANDOM:
        {
            double minimum = x;
            double maximum = y;
            if(minimum > maximum)
                std::swap(minimum, maximum);

            static std::mt19937_64 generator{std::random_device{}()};
            std::uniform_real_distribution<double> distribution(minimum, maximum);
            result = distribution(generator);
            label = L"random(X, Y)";
            break;
        }

        default:
            return;
    }

    if(!std::isfinite(result))
    {
        SetWindowTextString(scientificAdvancedResult, L"Result overflow or undefined result.");
        return;
    }

    SetWindowTextString(
        scientificAdvancedResult,
        label + L" = " + FormatNumber(result));
}

void MainWindow::CalculateStatistics()
{
    std::vector<double> values;
    std::wstring parseError;
    if(!ParseNumberList(GetWindowTextString(statisticsDataEdit), values, parseError))
    {
        SetWindowTextString(statisticsResult, parseError);
        return;
    }

    std::sort(values.begin(), values.end());

    long double sum = 0.0L;
    for(double value : values)
        sum += static_cast<long double>(value);

    const long double mean = sum / static_cast<long double>(values.size());

    double median = 0.0;
    if(values.size() % 2 == 1)
    {
        median = values[values.size() / 2];
    }
    else
    {
        const size_t right = values.size() / 2;
        const long double middle =
            (static_cast<long double>(values[right - 1]) +
             static_cast<long double>(values[right])) / 2.0L;
        median = static_cast<double>(middle);
    }

    size_t bestCount = 1;
    std::vector<double> modes;
    for(size_t i = 0; i < values.size();)
    {
        size_t j = i + 1;
        while(j < values.size() && values[j] == values[i])
            ++j;

        const size_t count = j - i;
        if(count > bestCount)
        {
            bestCount = count;
            modes.clear();
            modes.push_back(values[i]);
        }
        else if(count == bestCount && count > 1)
        {
            modes.push_back(values[i]);
        }
        i = j;
    }

    long double squared = 0.0L;
    for(double value : values)
    {
        const long double delta = static_cast<long double>(value) - mean;
        squared += delta * delta;
    }

    const long double populationVariance =
        squared / static_cast<long double>(values.size());
    const long double populationStdDev = std::sqrt(populationVariance);

    const double displaySum = static_cast<double>(sum);
    const double displayMean = static_cast<double>(mean);
    const double displayPopulationVariance = static_cast<double>(populationVariance);
    const double displayPopulationStdDev = static_cast<double>(populationStdDev);

    if(!std::isfinite(displaySum) ||
       !std::isfinite(displayMean) ||
       !std::isfinite(median) ||
       !std::isfinite(displayPopulationVariance) ||
       !std::isfinite(displayPopulationStdDev))
    {
        SetWindowTextString(
            statisticsResult,
            L"The data is valid, but one or more statistics exceed Nyxoryth's display range.");
        return;
    }

    std::wostringstream out;
    out << L"Count: " << values.size() << L"\r\n"
        << L"Sum: " << FormatNumber(static_cast<double>(sum)) << L"\r\n"
        << L"Mean: " << FormatNumber(static_cast<double>(mean)) << L"\r\n"
        << L"Median: " << FormatNumber(median) << L"\r\n";

    if(modes.empty())
    {
        out << L"Mode: none (all values occur once)\r\n";
    }
    else
    {
        out << L"Mode: ";
        for(size_t i = 0; i < modes.size(); ++i)
        {
            if(i != 0) out << L", ";
            if(i >= 8)
            {
                out << L"…";
                break;
            }
            out << FormatNumber(modes[i]);
        }
        out << L"  (frequency " << bestCount << L")\r\n";
    }

    out << L"Min / Max: " << FormatNumber(values.front()) << L" / "
        << FormatNumber(values.back()) << L"\r\n"
        << L"Range: " << FormatNumber(values.back() - values.front()) << L"\r\n"
        << L"Population variance: " << FormatNumber(static_cast<double>(populationVariance)) << L"\r\n"
        << L"Population std dev: " << FormatNumber(static_cast<double>(populationStdDev));

    if(values.size() > 1)
    {
        const long double sampleVariance =
            squared / static_cast<long double>(values.size() - 1);
        out << L"\r\nSample variance: "
            << FormatNumber(static_cast<double>(sampleVariance))
            << L"\r\nSample std dev: "
            << FormatNumber(static_cast<double>(std::sqrt(sampleVariance)));
    }
    else
    {
        out << L"\r\nSample variance/std dev: need at least 2 values";
    }

    SetWindowTextString(statisticsResult, out.str());
}

void MainWindow::CalculateFraction(int operation)
{
    long long aNum = 0;
    long long aDen = 0;
    long long bNum = 0;
    long long bDen = 0;

    const bool hasANumerator =
        TryParseSignedLongLong(GetWindowTextString(fractionANumeratorEdit), aNum);
    const bool hasADenominator =
        TryParseSignedLongLong(GetWindowTextString(fractionADenominatorEdit), aDen);
    const bool hasBNumerator =
        TryParseSignedLongLong(GetWindowTextString(fractionBNumeratorEdit), bNum);
    const bool hasBDenominator =
        TryParseSignedLongLong(GetWindowTextString(fractionBDenominatorEdit), bDen);

    const bool hasA = hasANumerator && hasADenominator;
    const bool hasB = hasBNumerator && hasBDenominator;

    if(operation == BTN_MIXED_TO_IMPROPER)
    {
        long long whole = 0;
        long long numerator = 0;
        long long denominator = 0;
        if(!TryParseSignedLongLong(GetWindowTextString(fractionMixedWholeEdit), whole) ||
           !TryParseSignedLongLong(GetWindowTextString(fractionMixedNumeratorEdit), numerator) ||
           !TryParseSignedLongLong(GetWindowTextString(fractionMixedDenominatorEdit), denominator) ||
           denominator == 0)
        {
            SetWindowTextString(fractionResult, L"Enter a valid mixed number and a nonzero denominator.");
            return;
        }

        if(denominator < 0)
        {
            if(denominator == std::numeric_limits<long long>::min() ||
               numerator == std::numeric_limits<long long>::min())
            {
                SetWindowTextString(fractionResult, L"Mixed-number components are outside the supported integer range.");
                return;
            }
            denominator = -denominator;
            numerator = -numerator;
        }

        const long double magnitude = std::fabs(static_cast<long double>(numerator));
        const long double combined =
            whole < 0
                ? static_cast<long double>(whole) * denominator - magnitude
                : static_cast<long double>(whole) * denominator + magnitude;

        long long improper = 0;
        if(!CheckedFractionValue(combined, improper))
        {
            SetWindowTextString(fractionResult, L"Mixed-number result is outside the supported integer range.");
            return;
        }

        SetWindowTextString(
            fractionResult,
            L"Improper fraction: " + FractionText(improper, denominator));
        return;
    }

    if(operation == BTN_FRACTION_GCD_LCM)
    {
        if(!TryParseSignedLongLong(GetWindowTextString(fractionANumeratorEdit), aNum) ||
           !TryParseSignedLongLong(GetWindowTextString(fractionBNumeratorEdit), bNum))
        {
            SetWindowTextString(fractionResult, L"Enter valid integer numerators in A and B for GCD/LCM.");
            return;
        }

        if(aNum == std::numeric_limits<long long>::min() ||
           bNum == std::numeric_limits<long long>::min())
        {
            SetWindowTextString(fractionResult, L"That integer is outside the supported GCD/LCM range.");
            return;
        }

        const long long left = std::llabs(aNum);
        const long long right = std::llabs(bNum);
        const long long gcd = std::gcd(left, right);

        long long lcm = 0;
        if(left != 0 && right != 0)
        {
            const long double lcmValue =
                (static_cast<long double>(left) / static_cast<long double>(gcd)) *
                static_cast<long double>(right);
            if(!CheckedFractionValue(lcmValue, lcm))
            {
                SetWindowTextString(fractionResult, L"LCM is outside the supported integer range.");
                return;
            }
        }

        SetWindowTextString(
            fractionResult,
            L"GCD: " + std::to_wstring(gcd) + L"\r\nLCM: " + std::to_wstring(lcm));
        return;
    }

    if(!hasANumerator)
    {
        SetWindowTextString(fractionResult, L"Fraction A numerator must be a whole integer.");
        return;
    }

    if(!hasADenominator)
    {
        SetWindowTextString(fractionResult, L"Fraction A denominator must be a whole integer.");
        return;
    }

    if(aDen == 0)
    {
        SetWindowTextString(fractionResult, L"Fraction A denominator cannot be zero.");
        return;
    }

    if(operation == BTN_FRACTION_SIMPLIFY)
    {
        SetWindowTextString(fractionResult, L"Simplified A: " + FractionText(aNum, aDen));
        return;
    }

    if(operation == BTN_FRACTION_DECIMAL)
    {
        SetWindowTextString(
            fractionResult,
            L"A as decimal: " + FormatNumber(static_cast<double>(aNum) / static_cast<double>(aDen)));
        return;
    }

    if(operation == BTN_FRACTION_TO_MIXED)
    {
        if(!NormalizeFraction(aNum, aDen))
        {
            SetWindowTextString(fractionResult, L"Invalid Fraction A.");
            return;
        }

        const long long whole = aNum / aDen;
        const long long remainder = std::llabs(aNum % aDen);
        if(remainder == 0)
        {
            SetWindowTextString(fractionResult, L"A as mixed number: " + std::to_wstring(whole));
        }
        else if(whole == 0 && aNum < 0)
        {
            SetWindowTextString(
                fractionResult,
                L"A as mixed number: -" + std::to_wstring(remainder) + L"/" + std::to_wstring(aDen));
        }
        else
        {
            SetWindowTextString(
                fractionResult,
                L"A as mixed number: " + std::to_wstring(whole) + L" " +
                std::to_wstring(remainder) + L"/" + std::to_wstring(aDen));
        }
        return;
    }

    if(!hasBNumerator)
    {
        SetWindowTextString(fractionResult, L"Fraction B numerator must be a whole integer.");
        return;
    }

    if(!hasBDenominator)
    {
        SetWindowTextString(fractionResult, L"Fraction B denominator must be a whole integer.");
        return;
    }

    if(bDen == 0)
    {
        SetWindowTextString(fractionResult, L"Fraction B denominator cannot be zero.");
        return;
    }

    long double numeratorValue = 0.0L;
    long double denominatorValue = 1.0L;
    std::wstring operationLabel;

    switch(operation)
    {
        case BTN_FRACTION_ADD:
            numeratorValue =
                static_cast<long double>(aNum) * bDen +
                static_cast<long double>(bNum) * aDen;
            denominatorValue = static_cast<long double>(aDen) * bDen;
            operationLabel = L"A + B";
            break;

        case BTN_FRACTION_SUBTRACT:
            numeratorValue =
                static_cast<long double>(aNum) * bDen -
                static_cast<long double>(bNum) * aDen;
            denominatorValue = static_cast<long double>(aDen) * bDen;
            operationLabel = L"A - B";
            break;

        case BTN_FRACTION_MULTIPLY:
            numeratorValue = static_cast<long double>(aNum) * bNum;
            denominatorValue = static_cast<long double>(aDen) * bDen;
            operationLabel = L"A × B";
            break;

        case BTN_FRACTION_DIVIDE:
            if(bNum == 0)
            {
                SetWindowTextString(fractionResult, L"Cannot divide by a zero fraction.");
                return;
            }
            numeratorValue = static_cast<long double>(aNum) * bDen;
            denominatorValue = static_cast<long double>(aDen) * bNum;
            operationLabel = L"A ÷ B";
            break;

        default:
            return;
    }

    long long numerator = 0;
    long long denominator = 0;
    if(!CheckedFractionValue(numeratorValue, numerator) ||
       !CheckedFractionValue(denominatorValue, denominator) ||
       denominator == 0)
    {
        SetWindowTextString(fractionResult, L"Fraction result is outside the supported integer range.");
        return;
    }

    if(!NormalizeFraction(numerator, denominator))
    {
        SetWindowTextString(fractionResult, L"Unable to normalize the fraction result.");
        return;
    }

    const double decimal = static_cast<double>(numerator) / static_cast<double>(denominator);
    SetWindowTextString(
        fractionResult,
        operationLabel + L" = " + FractionText(numerator, denominator) +
        L"\r\nDecimal: " + FormatNumber(decimal));
}

void MainWindow::SolveEquation(int operation)
{
    if(operation == BTN_LINEAR_SOLVE)
    {
        double a = 0.0;
        double b = 0.0;
        double c = 0.0;
        if(!TryParseDouble(GetWindowTextString(linearAEdit), a) ||
           !TryParseDouble(GetWindowTextString(linearBEdit), b) ||
           !TryParseDouble(GetWindowTextString(linearCEdit), c))
        {
            SetWindowTextString(equationResult, L"Enter valid finite linear coefficients.");
            return;
        }

        if(a == 0.0)
        {
            SetWindowTextString(
                equationResult,
                b == c ? L"Linear equation has infinitely many solutions."
                       : L"Linear equation has no solution.");
            return;
        }

        const double x = (c - b) / a;
        if(!std::isfinite(x))
        {
            SetWindowTextString(
                equationResult,
                L"The linear coefficients are valid, but the solution exceeds the supported numeric range.");
            return;
        }

        SetWindowTextString(equationResult, L"Linear solution\r\nx = " + FormatNumber(x));
        return;
    }

    if(operation == BTN_QUADRATIC_SOLVE)
    {
        double a = 0.0;
        double b = 0.0;
        double c = 0.0;
        if(!TryParseDouble(GetWindowTextString(quadraticAEdit), a) ||
           !TryParseDouble(GetWindowTextString(quadraticBEdit), b) ||
           !TryParseDouble(GetWindowTextString(quadraticCEdit), c))
        {
            SetWindowTextString(equationResult, L"Enter valid finite quadratic coefficients.");
            return;
        }

        if(a == 0.0)
        {
            if(b == 0.0)
            {
                SetWindowTextString(
                    equationResult,
                    c == 0.0 ? L"All real values satisfy this equation."
                             : L"No solution.");
            }
            else
            {
                SetWindowTextString(
                    equationResult,
                    L"a = 0, so this is linear.\r\nx = " + FormatNumber(-c / b));
            }
            return;
        }

        const long double discriminantWide =
            static_cast<long double>(b) * static_cast<long double>(b) -
            4.0L * static_cast<long double>(a) * static_cast<long double>(c);

        const double discriminant = static_cast<double>(discriminantWide);
        if(!std::isfinite(discriminant))
        {
            SetWindowTextString(
                equationResult,
                L"The quadratic coefficients are valid, but the discriminant exceeds the supported display range.");
            return;
        }

        std::wstring text = L"Discriminant: " + FormatNumber(discriminant) + L"\r\n";

        if(discriminantWide > 0.0L)
        {
            const long double root = std::sqrt(discriminantWide);
            const long double denominator = 2.0L * static_cast<long double>(a);
            const double x1 = static_cast<double>((-static_cast<long double>(b) + root) / denominator);
            const double x2 = static_cast<double>((-static_cast<long double>(b) - root) / denominator);

            if(!std::isfinite(x1) || !std::isfinite(x2))
            {
                SetWindowTextString(equationResult, L"Quadratic roots exceed the supported numeric range.");
                return;
            }

            text += L"x1 = " + FormatNumber(x1) + L"\r\nx2 = " + FormatNumber(x2);
        }
        else if(discriminantWide == 0.0L)
        {
            const double root =
                static_cast<double>(-static_cast<long double>(b) /
                                    (2.0L * static_cast<long double>(a)));
            if(!std::isfinite(root))
            {
                SetWindowTextString(equationResult, L"The repeated root exceeds the supported numeric range.");
                return;
            }
            text += L"Repeated root\r\nx = " + FormatNumber(root);
        }
        else
        {
            const long double denominator = 2.0L * static_cast<long double>(a);
            const double real =
                static_cast<double>(-static_cast<long double>(b) / denominator);
            const double imaginary =
                static_cast<double>(std::sqrt(-discriminantWide) / std::fabs(denominator));

            if(!std::isfinite(real) || !std::isfinite(imaginary))
            {
                SetWindowTextString(equationResult, L"Complex roots exceed the supported numeric range.");
                return;
            }

            text +=
                L"Complex roots\r\nx1 = " + FormatNumber(real) + L" + " +
                FormatNumber(imaginary) + L"i\r\nx2 = " + FormatNumber(real) + L" - " +
                FormatNumber(imaginary) + L"i";
        }

        SetWindowTextString(equationResult, text);
    }
}

void MainWindow::CalculateComplex(int operation)
{
    double a = 0.0;
    double b = 0.0;
    double c = 0.0;
    double d = 0.0;

    if(!TryParseDouble(GetWindowTextString(complexReal1Edit), a) ||
       !TryParseDouble(GetWindowTextString(complexImag1Edit), b))
    {
        SetWindowTextString(complexResult, L"Enter valid real and imaginary parts for z1.");
        return;
    }

    const bool needsSecond =
        operation == BTN_COMPLEX_ADD ||
        operation == BTN_COMPLEX_SUBTRACT ||
        operation == BTN_COMPLEX_MULTIPLY ||
        operation == BTN_COMPLEX_DIVIDE;

    if(needsSecond &&
       (!TryParseDouble(GetWindowTextString(complexReal2Edit), c) ||
        !TryParseDouble(GetWindowTextString(complexImag2Edit), d)))
    {
        SetWindowTextString(complexResult, L"Enter valid real and imaginary parts for z2.");
        return;
    }

    auto formatComplex = [&](double real, double imag, std::wstring& text) -> bool
    {
        if(!std::isfinite(real) || !std::isfinite(imag))
            return false;

        const wchar_t* sign = imag < 0.0 ? L" - " : L" + ";
        text = FormatNumber(real) + sign + FormatNumber(std::fabs(imag)) + L"i";
        return true;
    };

    auto showComplexResult =
        [&](const std::wstring& label, double real, double imag) -> bool
        {
            std::wstring formatted;
            if(!formatComplex(real, imag, formatted))
            {
                SetWindowTextString(
                    complexResult,
                    L"The complex result exceeds the supported numeric range.");
                return false;
            }

            SetWindowTextString(complexResult, label + formatted);
            return true;
        };

    switch(operation)
    {
        case BTN_COMPLEX_ADD:
            showComplexResult(L"z1 + z2 = ", a + c, b + d);
            break;

        case BTN_COMPLEX_SUBTRACT:
            showComplexResult(L"z1 - z2 = ", a - c, b - d);
            break;

        case BTN_COMPLEX_MULTIPLY:
            showComplexResult(L"z1 × z2 = ", a * c - b * d, a * d + b * c);
            break;

        case BTN_COMPLEX_DIVIDE:
        {
            const double denominator = c * c + d * d;
            if(denominator == 0.0)
            {
                SetWindowTextString(complexResult, L"Cannot divide by 0 + 0i.");
                return;
            }
            const double real = (a * c + b * d) / denominator;
            const double imag = (b * c - a * d) / denominator;
            showComplexResult(L"z1 ÷ z2 = ", real, imag);
            break;
        }

        case BTN_COMPLEX_MAGNITUDE:
        {
            const double magnitude = std::hypot(a, b);
            if(!std::isfinite(magnitude))
            {
                SetWindowTextString(complexResult, L"Magnitude exceeds the supported numeric range.");
                return;
            }
            SetWindowTextString(complexResult, L"|z1| = " + FormatNumber(magnitude));
            break;
        }

        case BTN_COMPLEX_CONJUGATE:
            showComplexResult(L"conj(z1) = ", a, -b);
            break;

        case BTN_COMPLEX_ARGUMENT:
        {
            if(a == 0.0 && b == 0.0)
            {
                SetWindowTextString(
                    complexResult,
                    L"The argument of 0 + 0i is undefined.");
                return;
            }

            const double radians = std::atan2(b, a);
            const double degrees = radians * 180.0 / kPi;
            SetWindowTextString(
                complexResult,
                L"arg(z1)\r\nRadians: " + FormatNumber(radians) +
                L"\r\nDegrees: " + FormatNumber(degrees));
            break;
        }

        default:
            break;
    }
}

void MainWindow::UpdateUnitChoices()
{
    if(!unitCategoryCombo || !unitFromCombo || !unitToCombo)
        return;

    int category = static_cast<int>(SendMessageW(unitCategoryCombo, CB_GETCURSEL, 0, 0));
    if(category < 0)
        category = 0;

    SendMessageW(unitFromCombo, CB_RESETCONTENT, 0, 0);
    SendMessageW(unitToCombo, CB_RESETCONTENT, 0, 0);

    const wchar_t* const* items = nullptr;
    int count = 0;

    static const wchar_t* lengthUnits[] = {L"mm", L"cm", L"m", L"km", L"in", L"ft", L"yd", L"mi"};
    static const wchar_t* massUnits[] = {L"mg", L"g", L"kg", L"oz", L"lb", L"tonne"};
    static const wchar_t* temperatureUnits[] = {L"°C", L"°F", L"K"};
    static const wchar_t* areaUnits[] = {L"mm²", L"cm²", L"m²", L"km²", L"in²", L"ft²", L"acre", L"hectare"};
    static const wchar_t* volumeUnits[] = {L"mL", L"L", L"m³", L"tsp", L"tbsp", L"fl oz (US)", L"cup (US)", L"pt (US)", L"gal (US)"};
    static const wchar_t* speedUnits[] = {L"m/s", L"km/h", L"mph", L"knot", L"ft/s"};
    static const wchar_t* pressureUnits[] = {L"Pa", L"kPa", L"MPa", L"bar", L"psi", L"atm"};
    static const wchar_t* energyUnits[] = {L"J", L"kJ", L"MJ", L"Wh", L"kWh", L"cal", L"kcal", L"BTU"};
    static const wchar_t* powerUnits[] = {L"W", L"kW", L"MW", L"hp"};
    static const wchar_t* angleUnits[] = {L"degree", L"radian", L"grad"};
    static const wchar_t* dataUnits[] = {L"bit", L"byte", L"KB", L"MB", L"GB", L"TB", L"KiB", L"MiB", L"GiB", L"TiB"};
    static const wchar_t* manualUnits[] = {L"Base", L"Target"};

    switch(category)
    {
        case 0: items = lengthUnits; count = 8; break;
        case 1: items = massUnits; count = 6; break;
        case 2: items = temperatureUnits; count = 3; break;
        case 3: items = areaUnits; count = 8; break;
        case 4: items = volumeUnits; count = 9; break;
        case 5: items = speedUnits; count = 5; break;
        case 6: items = pressureUnits; count = 6; break;
        case 7: items = energyUnits; count = 8; break;
        case 8: items = powerUnits; count = 4; break;
        case 9: items = angleUnits; count = 3; break;
        case 10: items = dataUnits; count = 10; break;
        case 11: items = manualUnits; count = 2; break;
        default: items = lengthUnits; count = 8; break;
    }

    for(int i = 0; i < count; ++i)
    {
        SendMessageW(unitFromCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(items[i]));
        SendMessageW(unitToCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(items[i]));
    }

    SendMessageW(unitFromCombo, CB_SETCURSEL, 0, 0);
    SendMessageW(unitToCombo, CB_SETCURSEL, count > 1 ? 1 : 0, 0);
}

void MainWindow::ConvertUnits()
{
    double value = 0.0;
    if(!TryParseDouble(GetWindowTextString(unitValueEdit), value))
    {
        SetWindowTextString(unitResult, L"Enter a valid finite value.");
        return;
    }

    const int category = static_cast<int>(SendMessageW(unitCategoryCombo, CB_GETCURSEL, 0, 0));
    const int from = static_cast<int>(SendMessageW(unitFromCombo, CB_GETCURSEL, 0, 0));
    const int to = static_cast<int>(SendMessageW(unitToCombo, CB_GETCURSEL, 0, 0));
    if(category < 0 || from < 0 || to < 0)
    {
        SetWindowTextString(unitResult, L"Choose a category and units.");
        return;
    }

    auto selectedText = [](HWND combo) -> std::wstring
    {
        const int index = static_cast<int>(SendMessageW(combo, CB_GETCURSEL, 0, 0));
        if(index < 0)
            return L"";
        const int length = static_cast<int>(SendMessageW(combo, CB_GETLBTEXTLEN, index, 0));
        if(length < 0)
            return L"";
        std::wstring text(static_cast<size_t>(length) + 1, L'\0');
        SendMessageW(combo, CB_GETLBTEXT, index, reinterpret_cast<LPARAM>(text.data()));
        text.resize(static_cast<size_t>(length));
        return text;
    };

    double result = value;

    if(category == 2)
    {
        double celsius = value;
        if(from == 1) celsius = (value - 32.0) * 5.0 / 9.0;
        else if(from == 2) celsius = value - 273.15;

        if(celsius < -273.15)
        {
            SetWindowTextString(
                unitResult,
                L"Temperature cannot be below absolute zero (-273.15 °C / 0 K).");
            return;
        }

        if(to == 0) result = celsius;
        else if(to == 1) result = celsius * 9.0 / 5.0 + 32.0;
        else result = celsius + 273.15;
    }
    else if(category == 11)
    {
        double rate = 0.0;
        if(!TryParseDouble(GetWindowTextString(unitRateEdit), rate) || rate <= 0.0)
        {
            SetWindowTextString(unitResult, L"Manual Rate needs a positive target-per-base rate.");
            return;
        }

        if(from == to)
            result = value;
        else if(from == 0 && to == 1)
            result = value * rate;
        else
            result = value / rate;
    }
    else
    {
        const double* factors = nullptr;
        int count = 0;

        static const double length[] = {0.001, 0.01, 1.0, 1000.0, 0.0254, 0.3048, 0.9144, 1609.344};
        static const double mass[] = {1e-6, 1e-3, 1.0, 0.028349523125, 0.45359237, 1000.0};
        static const double area[] = {1e-6, 1e-4, 1.0, 1e6, 0.00064516, 0.09290304, 4046.8564224, 10000.0};
        static const double volume[] = {0.001, 1.0, 1000.0, 0.00492892159375, 0.01478676478125, 0.0295735295625, 0.2365882365, 0.473176473, 3.785411784};
        static const double speed[] = {1.0, 1000.0 / 3600.0, 0.44704, 0.5144444444444444, 0.3048};
        static const double pressure[] = {1.0, 1000.0, 1e6, 100000.0, 6894.757293168, 101325.0};
        static const double energy[] = {1.0, 1000.0, 1e6, 3600.0, 3.6e6, 4.184, 4184.0, 1055.05585262};
        static const double power[] = {1.0, 1000.0, 1e6, 745.6998715822702};
        static const double angle[] = {kPi / 180.0, 1.0, kPi / 200.0};
        static const double data[] = {1.0, 8.0, 8000.0, 8e6, 8e9, 8e12, 8192.0, 8388608.0, 8589934592.0, 8796093022208.0};

        switch(category)
        {
            case 0: factors = length; count = 8; break;
            case 1: factors = mass; count = 6; break;
            case 3: factors = area; count = 8; break;
            case 4: factors = volume; count = 9; break;
            case 5: factors = speed; count = 5; break;
            case 6: factors = pressure; count = 6; break;
            case 7: factors = energy; count = 8; break;
            case 8: factors = power; count = 4; break;
            case 9: factors = angle; count = 3; break;
            case 10: factors = data; count = 10; break;
            default: break;
        }

        if(!factors || from >= count || to >= count)
        {
            SetWindowTextString(unitResult, L"That conversion is unavailable.");
            return;
        }

        const double base = value * factors[from];
        result = base / factors[to];
    }

    if(!std::isfinite(result))
    {
        SetWindowTextString(unitResult, L"Conversion overflow.");
        return;
    }

    SetWindowTextString(
        unitResult,
        FormatNumber(value) + L" " + selectedText(unitFromCombo) + L" =\r\n" +
        FormatNumber(result) + L" " + selectedText(unitToCombo));
}

void MainWindow::RunDateTool(int operation)
{
    const CalendarDate epoch{1970, 1, 1};
    const long long epochOrdinal = DateOrdinal(epoch);

    if(operation == BTN_DATE_FROM_UNIX)
    {
        long long timestamp = 0;
        if(!TryParseSignedLongLong(GetWindowTextString(unixTimestampEdit), timestamp))
        {
            SetWindowTextString(dateResult, L"Enter a valid whole-number Unix timestamp.");
            return;
        }

        long long days = timestamp / 86400LL;
        long long secondsOfDay = timestamp % 86400LL;
        if(secondsOfDay < 0)
        {
            secondsOfDay += 86400LL;
            --days;
        }

        CalendarDate date;
        if(!DateFromOrdinal(epochOrdinal + days, date))
        {
            SetWindowTextString(dateResult, L"Timestamp falls outside the supported years 0001–9999.");
            return;
        }

        const int hour = static_cast<int>(secondsOfDay / 3600LL);
        const int minute = static_cast<int>((secondsOfDay % 3600LL) / 60LL);
        const int second = static_cast<int>(secondsOfDay % 60LL);

        std::wostringstream out;
        out << FormatDateValue(date) << L' '
            << std::setfill(L'0') << std::setw(2) << hour << L':'
            << std::setw(2) << minute << L':' << std::setw(2) << second
            << L" UTC\r\n" << DayOfWeekName(DateOrdinal(date));
        SetWindowTextString(dateResult, out.str());
        return;
    }

    if(operation == BTN_DURATION_CONVERT)
    {
        long long seconds = 0;
        if(!TryParseSignedLongLong(GetWindowTextString(durationSecondsEdit), seconds))
        {
            SetWindowTextString(dateResult, L"Enter a whole-number duration in seconds.");
            return;
        }

        const bool negative = seconds < 0;
        unsigned long long magnitude =
            negative
                ? static_cast<unsigned long long>(-(seconds + 1LL)) + 1ULL
                : static_cast<unsigned long long>(seconds);

        const unsigned long long hours = magnitude / 3600ULL;
        const unsigned long long minutes = (magnitude % 3600ULL) / 60ULL;
        const unsigned long long remaining = magnitude % 60ULL;

        std::wostringstream out;
        if(negative) out << L'-';
        out << hours << L" h " << minutes << L" m " << remaining << L" s";
        SetWindowTextString(dateResult, out.str());
        return;
    }

    CalendarDate dateA;
    if(!ParseDateValue(GetWindowTextString(dateAEdit), dateA))
    {
        SetWindowTextString(dateResult, L"Date A must be a valid YYYY-MM-DD date.");
        return;
    }

    const long long ordinalA = DateOrdinal(dateA);

    if(operation == BTN_DATE_DIFFERENCE)
    {
        CalendarDate dateB;
        if(!ParseDateValue(GetWindowTextString(dateBEdit), dateB))
        {
            SetWindowTextString(dateResult, L"Date B must be a valid YYYY-MM-DD date.");
            return;
        }

        const long long difference = DateOrdinal(dateB) - ordinalA;
        const unsigned long long absoluteDifference =
            difference < 0
                ? static_cast<unsigned long long>(-(difference + 1LL)) + 1ULL
                : static_cast<unsigned long long>(difference);

        SetWindowTextString(
            dateResult,
            L"B - A: " + std::to_wstring(difference) + L" days\r\nAbsolute difference: " +
            std::to_wstring(absoluteDifference) + L" days");
        return;
    }

    if(operation == BTN_DATE_ADD_DAYS)
    {
        long long offset = 0;
        if(!TryParseSignedLongLong(GetWindowTextString(dateOffsetEdit), offset))
        {
            SetWindowTextString(dateResult, L"Enter a whole-number day offset.");
            return;
        }

        const long double targetOrdinalValue =
            static_cast<long double>(ordinalA) + static_cast<long double>(offset);
        const long long maximumOrdinal = DateOrdinal(CalendarDate{9999, 12, 31});
        if(targetOrdinalValue < 0.0L ||
           targetOrdinalValue > static_cast<long double>(maximumOrdinal))
        {
            SetWindowTextString(dateResult, L"Result falls outside the supported years 0001–9999.");
            return;
        }

        CalendarDate resultDate;
        if(!DateFromOrdinal(static_cast<long long>(targetOrdinalValue), resultDate))
        {
            SetWindowTextString(dateResult, L"Result falls outside the supported years 0001–9999.");
            return;
        }

        const unsigned long long offsetMagnitude =
            offset < 0
                ? static_cast<unsigned long long>(-(offset + 1LL)) + 1ULL
                : static_cast<unsigned long long>(offset);

        SetWindowTextString(
            dateResult,
            FormatDateValue(dateA) + (offset >= 0 ? L" + " : L" - ") +
            std::to_wstring(offsetMagnitude) + L" days =\r\n" +
            FormatDateValue(resultDate) + L" • " + DayOfWeekName(DateOrdinal(resultDate)));
        return;
    }

    if(operation == BTN_DATE_INSPECT)
    {
        SetWindowTextString(
            dateResult,
            FormatDateValue(dateA) + L"\r\n" +
            DayOfWeekName(ordinalA) + L"\r\nLeap year: " +
            (IsLeapYearValue(dateA.year) ? std::wstring(L"Yes") : std::wstring(L"No")) +
            L"\r\nDay ordinal: " + std::to_wstring(ordinalA));
        return;
    }

    if(operation == BTN_DATE_TO_UNIX)
    {
        int hour = 0;
        int minute = 0;
        int second = 0;
        if(!ParseClockValue(GetWindowTextString(dateTimeEdit), hour, minute, second))
        {
            SetWindowTextString(dateResult, L"Time must be HH:MM:SS using 00:00:00–23:59:59.");
            return;
        }

        const long long timestamp =
            (ordinalA - epochOrdinal) * 86400LL +
            static_cast<long long>(hour) * 3600LL +
            static_cast<long long>(minute) * 60LL +
            second;

        SetWindowTextString(
            dateResult,
            L"Unix timestamp (UTC):\r\n" + std::to_wstring(timestamp));
    }
}

void MainWindow::RunBitTool(int operation)
{
    std::wstring input = Trim(GetWindowTextString(bitToolValueEdit));
    unsigned long long raw = 0;
    bool signedNegativeInput = false;

    if(!TryParseUnsigned(input, raw))
    {
        long long signedInput = 0;
        if(!TryParseSignedLongLong(input, signedInput))
        {
            SetWindowTextString(bitToolResult, L"Enter a valid integer (decimal, 0x, 0b, or 0o).");
            return;
        }

        signedNegativeInput = signedInput < 0;
        raw = static_cast<unsigned long long>(signedInput);
    }

    const int sizeIndex = static_cast<int>(SendMessageW(bitToolWordSizeCombo, CB_GETCURSEL, 0, 0));
    static const int sizes[] = {8, 16, 32, 64};
    const int bits = (sizeIndex >= 0 && sizeIndex < 4) ? sizes[sizeIndex] : 64;
    const unsigned long long mask = MaskForBits(bits);
    const bool positiveInputWasMasked =
        !signedNegativeInput && bits < 64 && (raw & ~mask) != 0ULL;
    unsigned long long value = raw & mask;

    if(operation == BTN_BITTOOLS_SET ||
       operation == BTN_BITTOOLS_CLEAR ||
       operation == BTN_BITTOOLS_TOGGLE)
    {
        long long index = 0;
        if(!TryParseSignedLongLong(GetWindowTextString(bitToolIndexEdit), index) ||
           index < 0 || index >= bits)
        {
            SetWindowTextString(
                bitToolResult,
                L"Bit index must be between 0 and " + std::to_wstring(bits - 1) + L".");
            return;
        }

        const unsigned long long bit = 1ULL << static_cast<unsigned int>(index);
        if(operation == BTN_BITTOOLS_SET) value |= bit;
        else if(operation == BTN_BITTOOLS_CLEAR) value &= ~bit;
        else value ^= bit;
        value &= mask;
    }
    else if(operation == BTN_BITTOOLS_ROL)
    {
        value = ((value << 1) | (value >> (bits - 1))) & mask;
    }
    else if(operation == BTN_BITTOOLS_ROR)
    {
        value = ((value >> 1) | ((value & 1ULL) << (bits - 1))) & mask;
    }
    else if(operation == BTN_BITTOOLS_ENDIAN)
    {
        const int byteCount = bits / 8;
        unsigned long long reversed = 0;
        for(int i = 0; i < byteCount; ++i)
        {
            reversed <<= 8;
            reversed |= (value >> (i * 8)) & 0xFFULL;
        }
        value = reversed & mask;
    }

    if(operation != BTN_BITTOOLS_CONVERT)
        SetWindowTextString(bitToolValueEdit, L"0x" + ToUpperHex(value));

    long long signedValue = 0;
    if(bits == 64)
    {
        signedValue = static_cast<long long>(value);
    }
    else
    {
        signedValue = static_cast<long long>(value);
        const unsigned long long signBit = 1ULL << (bits - 1);
        if(value & signBit)
            signedValue -= static_cast<long long>(1ULL << bits);
    }

    const bool showSigned =
        SendMessageW(bitToolSignedCombo, CB_GETCURSEL, 0, 0) == 1;

    std::wostringstream out;
    out << bits << L"-bit • HEX 0x" << ToUpperHex(value) << L"\r\n"
        << L"BIN / two's complement: " << GroupBinary(value, bits) << L"\r\n"
        << L"Unsigned: " << value << L" • Signed: " << signedValue << L"\r\n"
        << L"Interpretation: " << (showSigned ? std::to_wstring(signedValue) : std::to_wstring(value))
        << L" • Popcount: " << CountSetBits(value);

    if(positiveInputWasMasked)
        out << L"\r\nNote: the input exceeded " << bits << L" bits and was masked to the selected word size.";

    SetWindowTextString(bitToolResult, out.str());
}

void MainWindow::LookupUnicodeCodepoint()
{
    std::wstring text = Trim(GetWindowTextString(unicodeCodeEdit));
    if(text.empty())
    {
        SetWindowTextString(unicodeResult, L"Enter a Unicode code point.");
        return;
    }

    unsigned long long code = 0;
    try
    {
        int base = 10;
        if(text.size() > 2 && text[0] == L'0' && (text[1] == L'x' || text[1] == L'X'))
        {
            base = 16;
            text = text.substr(2);
        }
        size_t used = 0;
        code = std::stoull(text, &used, base);
        if(used != text.size())
            throw std::invalid_argument("trailing");
    }
    catch(...)
    {
        SetWindowTextString(unicodeResult, L"Use decimal or 0x-prefixed hexadecimal.");
        return;
    }

    if(code > 0x10FFFFULL || (code >= 0xD800ULL && code <= 0xDFFFULL))
    {
        SetWindowTextString(unicodeResult, L"That is not a valid Unicode scalar value.");
        return;
    }

    std::wstring glyph;
    if(code >= 32ULL && code != 127ULL)
    {
        if(code <= 0xFFFFULL)
        {
            glyph.push_back(static_cast<wchar_t>(code));
        }
        else
        {
            const unsigned long long adjusted = code - 0x10000ULL;
            glyph.push_back(static_cast<wchar_t>(0xD800ULL + (adjusted >> 10)));
            glyph.push_back(static_cast<wchar_t>(0xDC00ULL + (adjusted & 0x3FFULL)));
        }
    }
    else
    {
        glyph = L"(control/non-printing)";
    }

    std::wostringstream out;
    out << L"U+" << std::uppercase << std::hex
        << std::setw(code <= 0xFFFFULL ? 4 : 6) << std::setfill(L'0') << code
        << std::dec << std::setfill(L' ') << L" / " << code << L" / " << glyph;
    SetWindowTextString(unicodeResult, out.str());
}

void MainWindow::RunComputerMath(int operation)
{
    if(operation == BTN_COMPUTER_TRANSFER)
    {
        double size = 0.0;
        double rate = 0.0;
        if(!TryParseDouble(GetWindowTextString(transferSizeEdit), size) || size < 0.0 ||
           !TryParseDouble(GetWindowTextString(transferRateEdit), rate) || rate <= 0.0)
        {
            SetWindowTextString(computerMathResult, L"Transfer size must be ≥ 0 and rate must be > 0.");
            return;
        }

        const int sizeUnit = static_cast<int>(SendMessageW(transferSizeUnitCombo, CB_GETCURSEL, 0, 0));
        const int rateUnit = static_cast<int>(SendMessageW(transferRateUnitCombo, CB_GETCURSEL, 0, 0));
        static const double sizeFactors[] = {1e6, 1e9, 1e12, 1048576.0, 1073741824.0};
        static const double rateFactors[] = {1e6 / 8.0, 1e9 / 8.0, 1e6, 1e9};

        if(sizeUnit < 0 || sizeUnit >= 5 || rateUnit < 0 || rateUnit >= 4)
        {
            SetWindowTextString(computerMathResult, L"Choose valid transfer size and rate units.");
            return;
        }

        const double seconds = size * sizeFactors[sizeUnit] / (rate * rateFactors[rateUnit]);
        if(!std::isfinite(seconds))
        {
            SetWindowTextString(computerMathResult, L"Transfer calculation overflow.");
            return;
        }

        if(seconds > static_cast<double>(std::numeric_limits<unsigned long long>::max()))
        {
            SetWindowTextString(
                computerMathResult,
                L"Transfer time: " + FormatNumber(seconds) + L" seconds (too large for H:M:S formatting)");
            return;
        }

        const unsigned long long wholeSeconds =
            seconds <= 0.0 ? 0ULL : static_cast<unsigned long long>(std::floor(seconds));
        const unsigned long long hours = wholeSeconds / 3600ULL;
        const unsigned long long minutes = (wholeSeconds % 3600ULL) / 60ULL;
        const unsigned long long remain = wholeSeconds % 60ULL;

        SetWindowTextString(
            computerMathResult,
            L"Transfer time: " + FormatNumber(seconds) + L" s\r\n≈ " +
            std::to_wstring(hours) + L" h " + std::to_wstring(minutes) + L" m " +
            std::to_wstring(remain) + L" s");
        return;
    }

    if(operation == BTN_COMPUTER_BITRATE)
    {
        double duration = 0.0;
        double megabitsPerSecond = 0.0;
        if(!TryParseDouble(GetWindowTextString(bitrateDurationEdit), duration) || duration < 0.0 ||
           !TryParseDouble(GetWindowTextString(bitrateRateEdit), megabitsPerSecond) || megabitsPerSecond < 0.0)
        {
            SetWindowTextString(computerMathResult, L"Duration and bitrate must be nonnegative numbers.");
            return;
        }

        const double bytes = duration * megabitsPerSecond * 1e6 / 8.0;
        if(!std::isfinite(bytes))
        {
            SetWindowTextString(computerMathResult, L"Bitrate/file-size calculation overflow.");
            return;
        }

        SetWindowTextString(
            computerMathResult,
            L"File size: " + FormatNumber(bytes / 1e6) + L" MB\r\n" +
            FormatNumber(bytes / 1073741824.0) + L" GiB");
        return;
    }

    if(operation == BTN_COMPUTER_RESOLUTION)
    {
        long long width = 0;
        long long height = 0;
        double diagonal = 0.0;
        if(!TryParseSignedLongLong(GetWindowTextString(resolutionWidthEdit), width) || width <= 0 ||
           !TryParseSignedLongLong(GetWindowTextString(resolutionHeightEdit), height) || height <= 0 ||
           !TryParseDouble(GetWindowTextString(resolutionDiagonalEdit), diagonal) || diagonal <= 0.0)
        {
            SetWindowTextString(computerMathResult, L"Width, height, and diagonal must all be positive.");
            return;
        }

        const long long divisor = std::gcd(width, height);
        const long double pixels = static_cast<long double>(width) * static_cast<long double>(height);
        const double ppi = std::hypot(static_cast<double>(width), static_cast<double>(height)) / diagonal;

        SetWindowTextString(
            computerMathResult,
            L"Pixels: " + FormatNumber(static_cast<double>(pixels)) +
            L" (" + FormatNumber(static_cast<double>(pixels / 1000000.0L)) + L" MP)\r\nAspect: " +
            std::to_wstring(width / divisor) + L":" + std::to_wstring(height / divisor) +
            L" • PPI: " + FormatNumber(ppi));
        return;
    }

    if(operation == BTN_COMPUTER_RAID)
    {
        long long drives = 0;
        double driveSize = 0.0;
        if(!TryParseSignedLongLong(GetWindowTextString(raidDriveCountEdit), drives) || drives <= 0 ||
           !TryParseDouble(GetWindowTextString(raidDriveSizeEdit), driveSize) || driveSize <= 0.0)
        {
            SetWindowTextString(computerMathResult, L"Enter a positive drive count and positive TB-per-drive size.");
            return;
        }

        const int mode = static_cast<int>(SendMessageW(raidModeCombo, CB_GETCURSEL, 0, 0));
        double usable = 0.0;
        std::wstring tolerance;
        std::wstring requirement;

        switch(mode)
        {
            case 0:
                usable = static_cast<double>(drives) * driveSize;
                tolerance = L"0 drive failures";
                requirement = L"minimum 1 drive";
                break;

            case 1:
                if(drives < 2)
                {
                    SetWindowTextString(computerMathResult, L"RAID 1 requires at least 2 drives.");
                    return;
                }
                usable = driveSize;
                tolerance = std::to_wstring(drives - 1) + L" failures in a full mirror";
                requirement = L"minimum 2 drives";
                break;

            case 2:
                if(drives < 3)
                {
                    SetWindowTextString(computerMathResult, L"RAID 5 requires at least 3 drives.");
                    return;
                }
                usable = static_cast<double>(drives - 1) * driveSize;
                tolerance = L"1 drive failure";
                requirement = L"minimum 3 drives";
                break;

            case 3:
                if(drives < 4)
                {
                    SetWindowTextString(computerMathResult, L"RAID 6 requires at least 4 drives.");
                    return;
                }
                usable = static_cast<double>(drives - 2) * driveSize;
                tolerance = L"2 drive failures";
                requirement = L"minimum 4 drives";
                break;

            case 4:
                if(drives < 4 || (drives % 2) != 0)
                {
                    SetWindowTextString(computerMathResult, L"RAID 10 requires an even number of at least 4 drives.");
                    return;
                }
                usable = static_cast<double>(drives / 2) * driveSize;
                tolerance = L"at least 1; potentially one per mirror pair";
                requirement = L"even drive count, minimum 4";
                break;

            default:
                SetWindowTextString(computerMathResult, L"Choose a valid RAID mode.");
                return;
        }

        if(!std::isfinite(usable))
        {
            SetWindowTextString(computerMathResult, L"RAID capacity calculation overflow.");
            return;
        }

        SetWindowTextString(
            computerMathResult,
            L"Usable estimate: " + FormatNumber(usable) + L" TB\r\nFault tolerance: " +
            tolerance + L"\r\n" + requirement);
    }
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
    ActivateScreen(historyVisible ? MENU_STANDARD : BTN_HISTORY);
}

void MainWindow::ClearHistory()
{
    history.clear();

    if(historyList)
        SendMessageW(historyList, LB_RESETCONTENT, 0, 0);
}

void MainWindow::UpdateWindowLayout()
{
    const bool standardMode =
        !scientificMode && !scientificAdvancedMode && !statisticsMode &&
        !fractionMode && !equationMode && !complexMode && !tipMode &&
        !unitMode && !dateMode && !programmerMode && !bitToolsMode &&
        !computerMathMode && !settingsMode && !historyVisible;
    const bool calculatorScreen = standardMode || scientificMode;

    if(expressionDisplay)
        ShowWindow(expressionDisplay, calculatorScreen ? SW_SHOW : SW_HIDE);
    if(resultDisplay)
        ShowWindow(resultDisplay, calculatorScreen ? SW_SHOW : SW_HIDE);
    if(copyResultButton)
        ShowWindow(copyResultButton, calculatorScreen ? SW_SHOW : SW_HIDE);

    if(standardMode)
    {
        for(size_t i = 0; i < standardControls.size(); ++i)
        {
            const int column = static_cast<int>(i % 4);
            const int row = static_cast<int>(i / 4);
            SetWindowPos(
                standardControls[i], nullptr,
                20 + (column * 110),
                166 + (row * 58),
                100, 50,
                SWP_NOZORDER | SWP_NOACTIVATE);
            ShowWindow(standardControls[i], SW_SHOW);
        }
    }
    else if(scientificMode)
    {
        std::vector<HWND> scientificGrid;
        scientificGrid.reserve(scientificControls.size() + standardControls.size());
        scientificGrid.insert(
            scientificGrid.end(), scientificControls.begin(), scientificControls.end());
        scientificGrid.insert(
            scientificGrid.end(), standardControls.begin(), standardControls.end());

        for(size_t i = 0; i < scientificGrid.size(); ++i)
        {
            const int column = static_cast<int>(i % 5);
            const int row = static_cast<int>(i / 5);
            SetWindowPos(
                scientificGrid[i], nullptr,
                20 + (column * 90),
                160 + (row * 54),
                80, 46,
                SWP_NOZORDER | SWP_NOACTIVATE);
            ShowWindow(scientificGrid[i], SW_SHOW);
        }
    }
    else
    {
        for(HWND control : standardControls)
            ShowWindow(control, SW_HIDE);
        for(HWND control : scientificControls)
            ShowWindow(control, SW_HIDE);
    }

    if(!scientificMode)
    {
        for(HWND control : scientificControls)
            ShowWindow(control, SW_HIDE);
    }

    auto showGroup = [](const std::vector<HWND>& controls, bool visible)
    {
        for(HWND control : controls)
            ShowWindow(control, visible ? SW_SHOW : SW_HIDE);
    };

    showGroup(tipControls, tipMode);
    showGroup(programmerControls, programmerMode);
    showGroup(scientificAdvancedControls, scientificAdvancedMode);
    showGroup(statisticsControls, statisticsMode);
    showGroup(fractionControls, fractionMode);
    showGroup(equationControls, equationMode);
    showGroup(complexControls, complexMode);
    showGroup(unitControls, unitMode);
    showGroup(dateControls, dateMode);
    showGroup(bitToolsControls, bitToolsMode);
    showGroup(computerMathControls, computerMathMode);
    showGroup(settingsControls, settingsMode);

    if(historyList)
    {
        SetWindowPos(historyList, nullptr, 20, 52, 440, 490, SWP_NOZORDER | SWP_NOACTIVATE);
        ShowWindow(historyList, historyVisible ? SW_SHOW : SW_HIDE);
    }

    if(copyHistoryButton)
    {
        SetWindowPos(copyHistoryButton, nullptr, 20, 554, 210, 42, SWP_NOZORDER | SWP_NOACTIVATE);
        ShowWindow(copyHistoryButton, historyVisible ? SW_SHOW : SW_HIDE);
    }

    if(clearHistoryButton)
    {
        SetWindowPos(clearHistoryButton, nullptr, 250, 554, 210, 42, SWP_NOZORDER | SWP_NOACTIVATE);
        ShowWindow(clearHistoryButton, historyVisible ? SW_SHOW : SW_HIDE);
    }

    EnsureMinimumWindowSize();
    MarkBackgroundDirty();

    if(hwnd)
        InvalidateRect(hwnd, nullptr, FALSE);
}

void MainWindow::UpdateModeText()
{
    if(modeLabel)
    {
        if(scientificMode)
            SetWindowTextW(modeLabel, degreeMode ? L"SCIENTIFIC • DEG" : L"SCIENTIFIC • RAD");
        else if(scientificAdvancedMode)
            SetWindowTextW(modeLabel, L"SCIENTIFIC+");
        else if(statisticsMode)
            SetWindowTextW(modeLabel, L"STATISTICS");
        else if(fractionMode)
            SetWindowTextW(modeLabel, L"FRACTIONS / GCD / LCM");
        else if(equationMode)
            SetWindowTextW(modeLabel, L"EQUATION SOLVER");
        else if(complexMode)
            SetWindowTextW(modeLabel, L"COMPLEX NUMBERS");
        else if(tipMode)
            SetWindowTextW(modeLabel, L"TIP / BILL SPLIT");
        else if(unitMode)
            SetWindowTextW(modeLabel, L"UNIT CONVERTER");
        else if(dateMode)
            SetWindowTextW(modeLabel, L"DATE / TIME TOOLS");
        else if(programmerMode)
            SetWindowTextW(modeLabel, L"PROGRAMMER / COMPUTER TOOLS");
        else if(bitToolsMode)
            SetWindowTextW(modeLabel, L"ADVANCED BIT TOOLS");
        else if(computerMathMode)
            SetWindowTextW(modeLabel, L"COMPUTER / STORAGE MATH");
        else if(settingsMode)
            SetWindowTextW(modeLabel, L"SETTINGS / APPEARANCE");
        else if(historyVisible)
            SetWindowTextW(modeLabel, L"HISTORY");
        else
            SetWindowTextW(modeLabel, L"STANDARD");
    }

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
        case BTN_NAV_MENU:      ShowNavigationMenu(); break;
        case BTN_HISTORY:       ToggleHistory(); break;
        case BTN_CLEAR_HISTORY: ClearHistory(); break;
        case BTN_MODE:          ToggleScientificMode(); break;
        case BTN_SCIENTIFIC_ADVANCED_MODE: ActivateScreen(BTN_SCIENTIFIC_ADVANCED_MODE); break;
        case BTN_STATISTICS_MODE:           ActivateScreen(BTN_STATISTICS_MODE); break;
        case BTN_FRACTIONS_MODE:            ActivateScreen(BTN_FRACTIONS_MODE); break;
        case BTN_EQUATIONS_MODE:            ActivateScreen(BTN_EQUATIONS_MODE); break;
        case BTN_COMPLEX_MODE:              ActivateScreen(BTN_COMPLEX_MODE); break;
        case BTN_UNIT_MODE:                 ActivateScreen(BTN_UNIT_MODE); break;
        case BTN_DATE_MODE:                 ActivateScreen(BTN_DATE_MODE); break;
        case BTN_BITTOOLS_MODE:             ActivateScreen(BTN_BITTOOLS_MODE); break;
        case BTN_COMPUTER_MATH_MODE:        ActivateScreen(BTN_COMPUTER_MATH_MODE); break;
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

        case BTN_SCI_ADV_SINH:
        case BTN_SCI_ADV_COSH:
        case BTN_SCI_ADV_TANH:
        case BTN_SCI_ADV_ASINH:
        case BTN_SCI_ADV_ACOSH:
        case BTN_SCI_ADV_ATANH:
        case BTN_SCI_ADV_CBRT:
        case BTN_SCI_ADV_NTH_ROOT:
        case BTN_SCI_ADV_TWO_POWER:
        case BTN_SCI_ADV_LOG2:
        case BTN_SCI_ADV_NCR:
        case BTN_SCI_ADV_NPR:
        case BTN_SCI_ADV_MOD:
        case BTN_SCI_ADV_FLOOR:
        case BTN_SCI_ADV_CEIL:
        case BTN_SCI_ADV_ROUND:
        case BTN_SCI_ADV_RANDOM:
            RunAdvancedScientific(id);
            break;

        case BTN_STATISTICS_CALCULATE:
            CalculateStatistics();
            break;

        case BTN_FRACTION_ADD:
        case BTN_FRACTION_SUBTRACT:
        case BTN_FRACTION_MULTIPLY:
        case BTN_FRACTION_DIVIDE:
        case BTN_FRACTION_SIMPLIFY:
        case BTN_FRACTION_DECIMAL:
        case BTN_FRACTION_TO_MIXED:
        case BTN_MIXED_TO_IMPROPER:
        case BTN_FRACTION_GCD_LCM:
            CalculateFraction(id);
            break;

        case BTN_LINEAR_SOLVE:
        case BTN_QUADRATIC_SOLVE:
            SolveEquation(id);
            break;

        case BTN_COMPLEX_ADD:
        case BTN_COMPLEX_SUBTRACT:
        case BTN_COMPLEX_MULTIPLY:
        case BTN_COMPLEX_DIVIDE:
        case BTN_COMPLEX_MAGNITUDE:
        case BTN_COMPLEX_CONJUGATE:
        case BTN_COMPLEX_ARGUMENT:
            CalculateComplex(id);
            break;

        case COMBO_UNIT_CATEGORY:
            UpdateUnitChoices();
            break;
        case BTN_UNIT_CONVERT:
            ConvertUnits();
            break;

        case BTN_DATE_DIFFERENCE:
        case BTN_DATE_ADD_DAYS:
        case BTN_DATE_INSPECT:
        case BTN_DATE_TO_UNIX:
        case BTN_DATE_FROM_UNIX:
        case BTN_DURATION_CONVERT:
            RunDateTool(id);
            break;

        case BTN_BITTOOLS_CONVERT:
        case BTN_BITTOOLS_SET:
        case BTN_BITTOOLS_CLEAR:
        case BTN_BITTOOLS_TOGGLE:
        case BTN_BITTOOLS_ROL:
        case BTN_BITTOOLS_ROR:
        case BTN_BITTOOLS_ENDIAN:
            RunBitTool(id);
            break;
        case BTN_UNICODE_LOOKUP:
            LookupUnicodeCodepoint();
            break;

        case BTN_COMPUTER_TRANSFER:
        case BTN_COMPUTER_BITRATE:
        case BTN_COMPUTER_RESOLUTION:
        case BTN_COMPUTER_RAID:
            RunComputerMath(id);
            break;

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

void MainWindow::FocusPrimaryControlForCurrentScreen()
{
    HWND target = nullptr;

    if(scientificAdvancedMode) target = scientificAdvancedXEdit;
    else if(statisticsMode) target = statisticsDataEdit;
    else if(fractionMode) target = fractionANumeratorEdit;
    else if(equationMode) target = linearAEdit;
    else if(complexMode) target = complexReal1Edit;
    else if(tipMode) target = tipBillEdit;
    else if(unitMode) target = unitValueEdit;
    else if(dateMode) target = dateAEdit;
    else if(programmerMode) target = programmerValueEdit;
    else if(bitToolsMode) target = bitToolValueEdit;
    else if(computerMathMode) target = transferSizeEdit;
    else if(historyVisible) target = historyList;

    if(target && IsWindowVisible(target) && IsWindowEnabled(target))
    {
        SetFocus(target);

        wchar_t className[32]{};
        if(GetClassNameW(target, className, 32) > 0 &&
           _wcsicmp(className, L"EDIT") == 0)
        {
            SendMessageW(target, EM_SETSEL, 0, -1);
        }
    }
}

bool MainWindow::HandleGlobalKey(const MSG& message)
{
    if(message.message != WM_KEYDOWN && message.message != WM_SYSKEYDOWN)
        return false;

    const WPARAM key = message.wParam;
    const bool controlDown = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
    const bool altDown = (GetKeyState(VK_MENU) & 0x8000) != 0;
    HWND focus = GetFocus();

    // Alt+M opens the screen menu from anywhere in Nyxoryth.
    if(altDown && (key == L'M' || key == L'm'))
    {
        ShowNavigationMenu();
        return true;
    }

    // F6 jumps to the primary input on the active tool screen.
    if(key == VK_F6)
    {
        FocusPrimaryControlForCurrentScreen();
        return true;
    }

    auto runPrimaryAction = [&]() -> bool
    {
        if(statisticsMode)
        {
            CalculateStatistics();
            return true;
        }

        if(fractionMode)
        {
            CalculateFraction(BTN_FRACTION_ADD);
            return true;
        }

        if(equationMode)
        {
            const bool quadraticFocus =
                focus == quadraticAEdit ||
                focus == quadraticBEdit ||
                focus == quadraticCEdit;

            SolveEquation(quadraticFocus ? BTN_QUADRATIC_SOLVE : BTN_LINEAR_SOLVE);
            return true;
        }

        if(complexMode)
        {
            CalculateComplex(BTN_COMPLEX_ADD);
            return true;
        }

        if(tipMode)
        {
            CalculateTip();
            return true;
        }

        if(unitMode)
        {
            ConvertUnits();
            return true;
        }

        if(dateMode)
        {
            if(focus == dateOffsetEdit)
                RunDateTool(BTN_DATE_ADD_DAYS);
            else if(focus == dateTimeEdit)
                RunDateTool(BTN_DATE_TO_UNIX);
            else if(focus == unixTimestampEdit)
                RunDateTool(BTN_DATE_FROM_UNIX);
            else if(focus == durationSecondsEdit)
                RunDateTool(BTN_DURATION_CONVERT);
            else
                RunDateTool(BTN_DATE_DIFFERENCE);
            return true;
        }

        if(programmerMode)
        {
            if(focus == storageBytesEdit)
                ConvertStorage();
            else if(focus == cidrIpEdit || focus == cidrPrefixEdit)
                CalculateCidr();
            else if(focus == charCodeEdit)
                ConvertCharacterCode();
            else
                ConvertProgrammerValue();
            return true;
        }

        if(bitToolsMode)
        {
            if(focus == unicodeCodeEdit)
                LookupUnicodeCodepoint();
            else
                RunBitTool(BTN_BITTOOLS_CONVERT);
            return true;
        }

        if(computerMathMode)
        {
            if(focus == bitrateDurationEdit || focus == bitrateRateEdit)
                RunComputerMath(BTN_COMPUTER_BITRATE);
            else if(focus == resolutionWidthEdit ||
                    focus == resolutionHeightEdit ||
                    focus == resolutionDiagonalEdit)
                RunComputerMath(BTN_COMPUTER_RESOLUTION);
            else if(focus == raidDriveCountEdit || focus == raidDriveSizeEdit)
                RunComputerMath(BTN_COMPUTER_RAID);
            else
                RunComputerMath(BTN_COMPUTER_TRANSFER);
            return true;
        }

        return false;
    };

    // F5 is a consistent "calculate/run this screen" shortcut.
    if(key == VK_F5)
        return runPrimaryAction();

    if(key != VK_RETURN)
        return false;

    // Statistics is deliberately multiline. Plain Enter remains a line break;
    // Ctrl+Enter performs the calculation.
    if(statisticsMode && focus == statisticsDataEdit)
    {
        if(controlDown)
        {
            CalculateStatistics();
            return true;
        }
        return false;
    }

    // Do not steal Enter from an owner-drawn button; Windows should activate
    // whichever button the user actually tabbed to.
    wchar_t className[32]{};
    if(focus && GetClassNameW(focus, className, 32) > 0 &&
       _wcsicmp(className, L"BUTTON") == 0)
    {
        return false;
    }

    // Single-line tool inputs use Enter for the context-appropriate action.
    return runPrimaryAction();
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
        // This sees key messages targeted at child EDIT/COMBO controls before
        // they are dispatched, allowing screen-level shortcuts without
        // subclassing every input control.
        if(HandleGlobalKey(msg))
            continue;

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
        case WM_GETMINMAXINFO:
            if(app)
            {
                MINMAXINFO* info = reinterpret_cast<MINMAXINFO*>(lParam);
                RECT required{0, 0, app->RequiredClientWidth(), app->RequiredClientHeight()};
                const DWORD style = static_cast<DWORD>(GetWindowLongPtrW(window, GWL_STYLE));
                const DWORD exStyle = static_cast<DWORD>(GetWindowLongPtrW(window, GWL_EXSTYLE));

                if(AdjustWindowRectEx(&required, style, FALSE, exStyle))
                {
                    info->ptMinTrackSize.x = required.right - required.left;
                    info->ptMinTrackSize.y = required.bottom - required.top;
                }
                return 0;
            }
            break;

        case WM_ENTERSIZEMOVE:
            if(app)
            {
                // Moving/resizing should feel immediate. Freeze the animation
                // temporarily and reuse the already-rendered frame.
                app->backgroundAnimationPaused = true;
                app->StopBackgroundTimer();
            }
            return 0;

        case WM_EXITSIZEMOVE:
            if(app)
            {
                app->backgroundAnimationPaused = false;
                app->MarkBackgroundDirty();
                app->StartBackgroundTimer();
                app->SaveWindowPosition();
                app->SaveSettings();
                InvalidateRect(window, nullptr, FALSE);
            }
            return 0;

        case WM_SIZE:
            if(app)
            {
                const bool minimized = (wParam == SIZE_MINIMIZED);
                const bool wasMinimized = app->windowMinimized;
                app->windowMinimized = minimized;

                if(minimized)
                {
                    app->StopBackgroundTimer();
                }
                else
                {
                    app->MarkBackgroundDirty();
                    if(wasMinimized && !app->backgroundAnimationPaused)
                        app->StartBackgroundTimer();
                }
            }
            break;

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
                app->ReleaseBackgroundBuffer();

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

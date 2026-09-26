#pragma once
#include <string>

namespace Nyxoryth {

class CalculatorState
{
public:
    std::wstring display = L"0";
    double storedValue = 0;
    wchar_t operation = 0;
    bool newEntry = true;

    void InputNumber(wchar_t value);
    void Clear();
    void SetOperation(wchar_t op);
    std::wstring Calculate();
};

}
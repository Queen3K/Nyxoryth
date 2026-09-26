#pragma once
#include <string>
#include <vector>

namespace Nyxoryth {

class CalculatorEngine
{
public:
    void Clear();
    void InputDigit(wchar_t digit);
    void InputDecimal();

    void SetOperator(wchar_t op);
    void Equals();

    std::wstring Display() const;
    const std::vector<std::wstring>& History() const;

private:
    std::wstring display=L"0";
    double storedValue=0;
    wchar_t operation=0;
    bool waiting=true;
    bool decimalUsed=false;

    std::vector<std::wstring> history;

    double Apply(double a,double b);
};

}
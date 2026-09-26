#include "CalculatorState.h"
#include <sstream>

namespace Nyxoryth {

void CalculatorState::InputNumber(wchar_t value)
{
    if(newEntry || display == L"0")
        display = std::wstring(1,value);
    else
        display += value;

    newEntry = false;
}

void CalculatorState::Clear()
{
    display=L"0";
    storedValue=0;
    operation=0;
    newEntry=true;
}

void CalculatorState::SetOperation(wchar_t op)
{
    storedValue = std::stod(display);
    operation = op;
    newEntry=true;
}

std::wstring CalculatorState::Calculate()
{
    double current = std::stod(display);

    if(operation==L'+') storedValue += current;
    if(operation==L'-') storedValue -= current;
    if(operation==L'*') storedValue *= current;
    if(operation==L'/')
        storedValue = current==0 ? 0 : storedValue/current;

    std::wstringstream out;
    out << storedValue;
    display = out.str();
    newEntry=true;

    return display;
}

}
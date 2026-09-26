#include "CalculatorEngine.h"
#include <sstream>

namespace Nyxoryth {

void CalculatorEngine::Clear()
{
    display=L"0";
    storedValue=0;
    operation=0;
    waiting=true;
    decimalUsed=false;
}

void CalculatorEngine::InputDigit(wchar_t digit)
{
    if(waiting || display==L"0")
        display=std::wstring(1,digit);
    else
        display+=digit;

    waiting=false;
}

void CalculatorEngine::InputDecimal()
{
    if(!decimalUsed)
    {
        display+=L".";
        decimalUsed=true;
    }
}

void CalculatorEngine::SetOperator(wchar_t op)
{
    storedValue=std::stod(display);
    operation=op;
    waiting=true;
    decimalUsed=false;
}

double CalculatorEngine::Apply(double a,double b)
{
    switch(operation)
    {
        case L'+': return a+b;
        case L'-': return a-b;
        case L'*': return a*b;
        case L'/': return b==0 ? 0 : a/b;
        case L'%': return a/100.0*b;
        default: return b;
    }
}

void CalculatorEngine::Equals()
{
    double current=std::stod(display);
    double result=Apply(storedValue,current);

    std::wstringstream out;
    out<<result;

    history.push_back(std::to_wstring(storedValue)+L" "+operation+L" "+std::to_wstring(current)+L" = "+out.str());

    display=out.str();
    waiting=true;
}

std::wstring CalculatorEngine::Display() const
{
    return display;
}

const std::vector<std::wstring>& CalculatorEngine::History() const
{
    return history;
}

}
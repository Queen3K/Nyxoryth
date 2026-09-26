
#include <string>

namespace Nyxoryth {

std::wstring CurrentDisplay=L"0";

void AppendInput(std::wstring value)
{
    if(CurrentDisplay==L"0")
        CurrentDisplay=value;
    else
        CurrentDisplay+=value;
}

void ClearDisplay()
{
    CurrentDisplay=L"0";
}

}

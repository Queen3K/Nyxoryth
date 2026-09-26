#include <vector>
#include <string>

namespace Nyxoryth {

std::vector<std::wstring> History;

void AddHistory(std::wstring item)
{
    History.push_back(item);
}

}
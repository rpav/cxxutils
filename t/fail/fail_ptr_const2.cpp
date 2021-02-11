#include <rpav/ptr.hpp>

using namespace rpav;

int main()
{
    int i = 42;
    const auto cpc = ptr<const int>(&i);

    *cpc = 4; // error
}

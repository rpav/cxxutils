#include <rpav/ptr.hpp>

using namespace rpav;

int main()
{
    int i = 42;
    auto pc = ptr<const int>(&i);

    *pc = 2; // error
}

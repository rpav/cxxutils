#include <rpav/ptr.hpp>

using namespace rpav;

int main()
{
    ptr<const int> pc2;
    ptr<int> p2 = pc2; // error
}

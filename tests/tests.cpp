//TODO: add google tests
#include <cassert>
#include "easyConfig.h"

void tests()
{
    ConfigReader config("config.txt");
    
    auto a = config.get<int>("someField");
    assert(a);
    assert(a == 2);

    auto b = config.get<std::string>("someField2");
    assert(b && *b == "Hello");
}
int main()
{
    tests();
    return 0;
}

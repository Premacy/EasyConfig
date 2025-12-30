//TODO: add google tests
#include <cassert>
#include <iostream>
#include "easyConfig.h"

void tests()
{
    ConfigReader config("config.txt");
    
    auto int_value = config.get<int>("someField");
    assert(int_value && int_value == 2);

    auto str_value = config.get<std::string>("someField2");
    assert(str_value && str_value == "Hello");

    auto double_value = config.get<double>("doubleField");
    assert(double_value && double_value == 2.5);
}
int main()
{
    std::cout << "Tests stared..." << std::endl;
    tests();
    std::cout << "Tests finished!" << std::endl;
    return 0;
}

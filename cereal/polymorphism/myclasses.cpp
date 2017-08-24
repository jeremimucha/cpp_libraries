#include "myclasses.hpp"
#include <iostream>


void DerivedClassOne::say_type()
{
    std::cout << "DerivedClassOne" << std::endl;
}

void EmbarrassingDerivedClass::say_type()
{
    std::cout << "EmbarrassingDerivedClass. Wait... I mean DerivedClassTwo!"
              << std::endl;
}

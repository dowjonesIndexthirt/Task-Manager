#include "procRead.hpp"

#include <iostream>
#include <vector>

int main(void){
    auto v = readAllPIDs();

    for (const auto& p : v)
        std::cout << p << std::endl;

    return 0;
}
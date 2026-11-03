#include <iostream>
#include <string>
#include <algorithm>
#include <cassert>

/* Taken from src/procRead.cpp */
bool isNumber(const std::string& str){
        return !(str.empty()) && std::all_of(str.begin(), str.end(), [](unsigned char c){
                                                                        return std::isdigit(c);
                                                                     }); // Better with lambda
}

int main(void){
    assert(isNumber("1010101FFAB") == false);
    assert(isNumber("") == false);
    assert(isNumber("10293") == true);
    assert(isNumber("\0") == false);
    return 0;
}

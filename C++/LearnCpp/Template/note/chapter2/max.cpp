//
// Created by ¶­å· on 2018/6/10.
//

#include "max.hpp"
#include <iostream>

int main(){
    double a = max(1.0, 2.0);
    std::cout << a << std::endl;

    std::cout << add<int, 2>(1) << std::endl;
    return 0;
}
//
// Created by ¶­å· on 2018/4/13.
//

#include <iostream>
#include <Vector3.hpp>

int main(){

    Vector3 a = Vector3(1, 1, 1);
    Vector3 b = Vector3(2, 2, 2);
    Vector3 c = a + b;
    std::cout << "x:" << c.x << std::endl;
    return 0;
}
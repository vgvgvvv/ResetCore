//
// Created by ¶­å· on 2018/6/10.
//

#include <iostream>
#include "Stack.hpp"


int main(){

    Stack<int, 100> stack;
    stack.push(100);
    std::cout << stack.top() << std::endl;
    return 1;
}
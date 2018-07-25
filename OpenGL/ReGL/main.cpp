#include <iostream>
#include "Application.hpp"

using namespace ReGL;

int main() {
    Application App;
    if(!App.Init()){
        return 1;
    }
    if(!App.Run()){
        return 1;
    }
    if(!App.Quit()){
        return 1;
    }
    return 0;
}
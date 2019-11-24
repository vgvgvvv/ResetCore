//
// Created by 35207 on 2019/11/15 0015.
//

#include "PolicyClass.h"

int main(){
    TestManager<Test, OpNewCreator2> Test;
    Test.Create();
    return 0;
}
//
// Created by ¶­å· on 2018/11/29.
//

#include "gtest/gtest.h"
#include <iostream>

using namespace std;

namespace CPP_11_FEATURE_CHAPTER_8{

    /**
     * Êý¾Ý¶ÔÆë
     */

    struct HowManyBytes{
        char a;
        int b;
    };

    TEST(CPP_11_FEATURE, CHAPTER_8_OFFSETOF){
        cout << sizeof(char) << endl;
        cout << sizeof(int) << endl;
        cout << sizeof(HowManyBytes) << endl;

        cout << offsetof(HowManyBytes, a) << endl;
        cout << offsetof(HowManyBytes, b) << endl;
    }

    struct alignas(32) ColorVector{
        double r;
        double g;
        double b;
        double a;
    };

    TEST(CPP_11_FEATURE, CHAPTER_8_ALIGNOF){
        cout << alignof(ColorVector) << endl;
    }
}
//
// Created by 35207 on 2019/8/31 0031.
//

#include <cstdio>

#define foo foo a bar b bar baz c
#define bar foo 12
#define baz bar 13

//foo
//a bar b bar baz c
//a foo 12 b foo 12 bar 13 c
//a foo 12 b foo 12 foo 12 13 c
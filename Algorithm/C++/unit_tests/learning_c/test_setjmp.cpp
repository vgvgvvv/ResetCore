//
// Created by 董宸 on 2019/2/3.
//

#include "gtest/gtest.h"
#include <setjmp.h>
#include <stdio.h>

namespace LearningC {


    jmp_buf ebuf;
    //测试long jmp
    TEST(JMP, TRY){
        int i;
        printf("\n");
        printf("1 ");
        i = setjmp(ebuf);
        printf("i = %d\n",i);//注意它不会输出
        if (i == 0){
            printf("2 ");
            longjmp(ebuf, 3);//返回setjmp执行点
            printf("This will not be printed.");//注意它不会输出
        }
        printf("%d", i);
    }

    
}

//
// Created by 董宸 on 2018/10/17.
//

#include "ndkdemo_reset_mytest_ndkdemo_NDKHelper.h"

JNIEXPORT jint JNICALL Java_ndkdemo_reset_mytest_ndkdemo_NDKHelper_Add
        (JNIEnv *env, jobject obj, jint a, jint b){
    return a+b;
}
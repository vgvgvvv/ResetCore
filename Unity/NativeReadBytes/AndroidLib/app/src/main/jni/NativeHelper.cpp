//
// Created by 董宸 on 2018/10/21.
//

#include "nativeread_reset_com_nativeread_NativeHelper.h"
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include <string>
#include <jni.h>
#include <android/log.h>

#ifdef __cplusplus
extern "C" {
#endif


#define TAG    "native-read-bytes" // 这个是自定义的LOG的标识
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__) // 定义LOGD类型

static AAssetManager *assetManager = nullptr;


JNIEXPORT void JNICALL Java_nativeread_reset_com_nativeread_NativeHelper_SetAssetManager
        (JNIEnv *env, jobject jobj, jobject jassetManager) {
    LOGD("set asset manager");
    assetManager = AAssetManager_fromJava(env, jassetManager);
}

JNIEXPORT int32_t JNICALL ReadBytes(char* fileName, unsigned char** result){

    if(assetManager == nullptr){
        return -1;
    }
    AAsset* asset = AAssetManager_open(assetManager, fileName, AASSET_MODE_UNKNOWN);
    if(asset == nullptr){
        return -1;
    }
    off_t size = AAsset_getLength(asset);
    if(size > 0){
        *result = new unsigned char[size];
        AAsset_read(asset, *result, size);
    }
    AAsset_close(asset);

    return (int32_t)size;
}

JNIEXPORT void JNICALL ReleaseBytes(unsigned char* bytes){
    delete[] bytes;
}

JNIEXPORT int JNICALL Add(int a , int b){
    return a + b;
}

#ifdef __cplusplus
}
#endif
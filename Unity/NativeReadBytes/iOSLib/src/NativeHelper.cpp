//
// Created by ¶­å· on 2018/11/7.
//
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
    int32_t ReadRawBytes(char* fileName, unsigned char** result){
        if(fileName == nullptr){
            return -1;
        }
        FILE* file = fopen(fileName, "r");
        if(file == nullptr){
            return -2;
        }
        fseek(file, 0L, SEEK_END);
        int64_t size = ftell(file);
        if(size <= 0){
            return -3;
        }
        *result = new uint8_t[size];
        fseek(file, 0, SEEK_SET);
        fread(*result, sizeof(uint8_t), static_cast<size_t>(size), file);
        fclose(file);
        return (int32_t)size;
    }

    void ReleaseBytes(unsigned char* bytes){
        delete[] bytes;
    }

#ifdef __cplusplus
}
#endif

//
// Created by 董宸 on 2018/11/7.
//

#include <queue>
#include <jni.h>
#include "PngLoader.h"
#include "nativeread_reset_com_nativeread_NativeHelper.h"

using namespace RONDK;
using UnityRenderEvent = void(*)(int);

std::queue<PngLoader*> g_updateQueue;

extern "C"{

    JNIEXPORT void* JNICALL CreateLoader(){
        return new PngLoader();
    }

    JNIEXPORT void JNICALL DestroyLoader(PngLoader *pPngLoader){
        if(pPngLoader == nullptr){
            return;
        }
        delete pPngLoader;
    }

    JNIEXPORT bool JNICALL Load(PngLoader *pPngLoader, const void *pData, size_t dataSize){
        if(pPngLoader == nullptr){
            return false;
        }
        pPngLoader->Load(pData, dataSize);
        return true;
    }

    JNIEXPORT bool JNICALL LoadWithPath(PngLoader *pPngLoader, char* fileName){
        if(fileName == nullptr){
            return false;
        }
        FILE* file = fopen(fileName, "r");
        if(file == nullptr){
            return false;
        }
        fseek(file, 0L, SEEK_END);
        int32_t size = ftell(file);
        if(size <= 0){
            return false;
        }
        uint8_t* pData = new uint8_t[size];
        fseek(file, 0, SEEK_SET);
        fread(pData, sizeof(uint8_t), static_cast<size_t>(size), file);
        fclose(file);
        Load(pPngLoader, pData, static_cast<size_t>(size));
        delete[] pData;
        return true;
    }

    JNIEXPORT bool JNICALL LoadFromStreamingAssets(PngLoader *pPngLoader, char* fileName){
        if(fileName == nullptr){
            return false;
        }
        unsigned char* bytes = nullptr;
        int size = ReadAssetsBytes(fileName, &bytes);
        if(bytes != nullptr){
            Load(pPngLoader, bytes, static_cast<size_t>(size));
            delete[] bytes;
            return true;
        }else{
            return false;
        }
    }

    JNIEXPORT void JNICALL SetTexture(PngLoader *pPngLoader, GLuint texture){
        if(pPngLoader == nullptr){
            return;
        }
        pPngLoader->SetTexture(texture);
        g_updateQueue.push(pPngLoader);
    }

    JNIEXPORT void JNICALL UpdateTextureImmediate(PngLoader *pPngLoader){
        if(pPngLoader == nullptr){
            return;
        }
        pPngLoader->UpdateTexture();
    }

    JNIEXPORT int32_t JNICALL GetWidth(PngLoader *pPngLoader){
        if(pPngLoader == nullptr){
            return 0;
        }
        return pPngLoader->GetWidth();
    }

    JNIEXPORT int32_t JNICALL GetHeight(PngLoader *pPngLoader){
        if(pPngLoader == nullptr){
            return 0;
        }
        return pPngLoader->GetHeight();
    }

    void OnRenderEvent(int eventId){
        while(!g_updateQueue.empty()){
            g_updateQueue.front()->UpdateTexture();
            g_updateQueue.pop();
        }
    }

    JNIEXPORT UnityRenderEvent JNICALL GetPngRenderEventFunc(){
        return OnRenderEvent;
    }
}
//
//  NativeTexture.m
//  NativeLib
//
//  Created by 董宸 on 2018/11/8.
//  Copyright © 2018 董宸. All rights reserved.
//

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

#include "UnityMetalSupport.h"
#include "PngLoader.h"

#include <stdlib.h>
#include <stdint.h>



static UIImage* LoadImage(const char* filename)
{
    NSString* imageName = [NSString stringWithUTF8String: filename];
    NSString* imagePath = [[NSBundle mainBundle] pathForResource: imageName ofType: @"png"];
    
    return [UIImage imageWithContentsOfFile: imagePath];
}

static UIImage* LoadImageWithFullPath(const char* filePath)
{
    
    if(filePath == nullptr){
        NSLog(@"filePath is null!!!%@", [NSThread callStackSymbols]);
        return nullptr;
    }
    NSString* imagePath = [NSString stringWithUTF8String: filePath];
    NSLog(@"Load From %@", imagePath);
    UIImage* img = [UIImage imageWithContentsOfFile: imagePath];
    return img;
}

// you need to free this pointer
static void* LoadDataFromImage(UIImage* image)
{
    if(image == nullptr){
        NSLog(@"Image is null!!!%@", [NSThread callStackSymbols]);
        return nullptr;
    }
    CGImageRef imageData    = image.CGImage;
    unsigned   imageW       = CGImageGetWidth(imageData);
    unsigned   imageH       = CGImageGetHeight(imageData);
    
//    // for the sake of the sample we enforce 128x128 textures
//    assert(imageW == 128 && imageH == 128);
    
    void* textureData = ::malloc(imageW * imageH * 4);
    ::memset(textureData, 0x00, imageW * imageH * 4);
    
    CGContextRef textureContext = CGBitmapContextCreate(textureData, imageW, imageH, 8, imageW * 4, CGImageGetColorSpace(imageData), kCGImageAlphaPremultipliedLast);
    CGContextSetBlendMode(textureContext, kCGBlendModeCopy);
    CGContextDrawImage(textureContext, CGRectMake(0, 0, imageW, imageH), imageData);
    CGContextRelease(textureContext);
    
    return textureData;
}

static uintptr_t CreateGlesTexture(void* data, unsigned w, unsigned h)
{
    GLuint texture = 0;
    glGenTextures(1, &texture);
    
    GLint curGLTex = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &curGLTex);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    glBindTexture(GL_TEXTURE_2D, curGLTex);
    
    return texture;
}

static void DestroyGlesTexture(uintptr_t tex)
{
    GLint curGLTex = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &curGLTex);
    
    GLuint glTex = tex;
    glDeleteTextures(1, &glTex);
    
    glBindTexture(GL_TEXTURE_2D, curGLTex);
}

static uintptr_t CreateMetalTexture(void* data, unsigned w, unsigned h)
{
#if defined(__IPHONE_8_0) && !TARGET_IPHONE_SIMULATOR
    Class MTLTextureDescriptorClass = [UnityGetMetalBundle() classNamed: @"MTLTextureDescriptor"];
    
    MTLTextureDescriptor* texDesc =
    [MTLTextureDescriptorClass texture2DDescriptorWithPixelFormat: MTLPixelFormatRGBA8Unorm width: w height: h mipmapped: NO];
    
    id<MTLTexture> tex = [UnityGetMetalDevice() newTextureWithDescriptor: texDesc];
    
    MTLRegion r = MTLRegionMake3D(0, 0, 0, w, h, 1);
    [tex replaceRegion: r mipmapLevel: 0 withBytes: data bytesPerRow: w * 4];
    
    return (uintptr_t)(__bridge_retained void*)tex;
#else
    return 0;
#endif
}

static void DestroyMetalTexture(uintptr_t tex)
{
    id<MTLTexture> mtltex = (__bridge_transfer id<MTLTexture>)(void*) tex;
    mtltex = nil;
}

extern "C" {
    
    intptr_t CreateNativeTexture(const char* filename)
    {
        UIImage*    image       = LoadImage(filename);
        if(image == nullptr){
            NSLog(@"Image is null!!!%@", [NSThread callStackSymbols]);
            return 0;
        }
        void*       textureData = LoadDataFromImage(image);
        if(textureData == nullptr){
            NSLog(@"textureData is null!!!%@", [NSThread callStackSymbols]);
            return 0;
        }
        
        uintptr_t ret = 0;
        if (UnitySelectedRenderingAPI() == apiMetal)
            ret = CreateMetalTexture(textureData, image.size.width, image.size.height);
        else
            ret = CreateGlesTexture(textureData, image.size.width, image.size.height);
        
        ::free(textureData);
        return ret;
    }
    
    intptr_t CreateNativeTextureWithFullPath(const char* filename)
    {
        UIImage*    image       = LoadImageWithFullPath(filename);
        if(image == nullptr){
            NSLog(@"image is null!!!%@", [NSThread callStackSymbols]);
            return 0;
        }
        void*       textureData = LoadDataFromImage(image);
        if(textureData == nullptr){
            NSLog(@"textureData is null!!!%@", [NSThread callStackSymbols]);
            return 0;
        }

        uintptr_t ret = 0;
        if (UnitySelectedRenderingAPI() == apiMetal)
            ret = CreateMetalTexture(textureData, image.size.width, image.size.height);
        else
            ret = CreateGlesTexture(textureData, image.size.width, image.size.height);
        
        ::free(textureData);
        return ret;
    }
    
    void DestroyNativeTexture(uintptr_t tex)
    {
        if(tex == 0){
            return;
        }
        if (UnitySelectedRenderingAPI() == apiMetal)
            DestroyMetalTexture(tex);
        else
            DestroyGlesTexture(tex);
    }

    PngLoader* CreateLoader(){
        return new PngLoader();
    }
    
    bool Load(PngLoader* loader, const char* filename){
        
        if(filename == nullptr){
            NSLog(@"filename is null!!!%@", [NSThread callStackSymbols]);
            return false;
        }
        
        UIImage*    image       = LoadImageWithFullPath(filename);
        if(image == nullptr){
            NSLog(@"image is null!!!%@", [NSThread callStackSymbols]);
            return 0;
        }
        void*       textureData = LoadDataFromImage(image);
        if(textureData == nullptr){
            NSLog(@"textureData is null!!!%@", [NSThread callStackSymbols]);
            return 0;
        }
        
        uintptr_t tex = 0;
        if (UnitySelectedRenderingAPI() == apiMetal)
        {
            tex = CreateMetalTexture(textureData, image.size.width, image.size.height);
        }
        else
        {
            tex = CreateGlesTexture(textureData, image.size.width, image.size.height);
        }
        
        ::free(textureData);
        
        loader->texture = tex;
        loader->width = image.size.width;
        loader->height = image.size.height;
        
        return true;
    }
    
    void DestroyLoader(PngLoader* loader){
        if(loader == nullptr){
            return;
        }
        DestroyNativeTexture(loader->texture);
        delete loader;
    }
    
    int GetWidth(PngLoader* loader){
        if(loader == nullptr){
            return 0;
        }
        return loader->width;
    }
    
    int GetHeight(PngLoader* loader){
        if(loader == nullptr){
            return 0;
        }
        return loader->height;
    }
    
    intptr_t GetTexturePtr(PngLoader* loader){
        return loader->texture;
    }
    
    

}

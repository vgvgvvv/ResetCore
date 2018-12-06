/**
 * OpenGL渲染相关函数
 */

#ifndef __GLMANAGER_H__
#define __GLMANAGER_H__
#include "Canvas.h"

namespace ReGL
{
    class GLManager
    {
    public:
        static bool Init();
        static bool Update();
        static bool LateUpdate();
        static bool Uninit();
    private:
        static Canvas canvas_;
    };

}

#endif


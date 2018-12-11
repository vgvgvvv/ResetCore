/**
 * OpenGL渲染相关函数
 */

#ifndef __GLMANAGER_H__
#define __GLMANAGER_H__
#include "Camera.h"
#include "looper/Scene.h"

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
        static Camera* camera_;
        static Scene* scene_;
    };

}

#endif


/**
 * 绘制相关函数
 */
#ifndef __CANVAS_H__
#define __CANVAS_H__
#include <vector>
#include "renderer/Renderer.h"

namespace ReGL
{
    class Camera
    {
    public:
        bool Render();
        bool Clear();

    private:
        //TODO 使用链表
        //std::vector<Renderer*> renderers_;
    };

}

#endif
/**
 * 绘制相关函数
 */
#ifndef __CANVAS_H__
#define __CANVAS_H__
#include "script/Component.h"

namespace ReGL
{
    class Camera : public Component
    {
    public:
        bool Render() const;
        bool Clear() const;

    };

}

#endif
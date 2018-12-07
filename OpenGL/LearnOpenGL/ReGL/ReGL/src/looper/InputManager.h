/**
 * 处理所有输入事件
 */

#ifndef __INPUTMANAGER_H__
#define __INPUTMANAGER_H__

namespace ReGL
{
    class InputManager
    {
    public:
        static bool ProcessInput();
        static bool PullInput();
    };
}

#endif
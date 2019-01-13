#ifndef __GLFWINPUTMANAGER_H__
#define __GLFWINPUTMANAGER_H__
#include "looper/InputManager.h"

namespace ReGL
{
    class GLFWInputManager : public InputManager
    {
    public:
        bool ProcessInput() override;
        bool PullInput() override;
    };
}

#endif // GLFWINPUTMANAGER_H

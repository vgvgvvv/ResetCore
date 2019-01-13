#include "GLManager.h"
#include "GLADManager.h"
#include "GLFWManager.h"
#include "utility/Context.h"
#include "Camera.h"

namespace ReGL
{
    bool GLManager::Init()
    {
        if (!GLFWManager::Init())
        {
            Context::GetLogger().Error(" GLFWManager::Init with error !!!");
            return false;
        }
        if (!GLADManager::Init())
        {
            Context::GetLogger().Error(" GLADManager::Init with error !!!");
            return false;
        }
        return true;
    }


    bool GLManager::WillRender()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        return true;
    }

    bool GLManager::Update()
    {
        if(GLFWManager::ShouldFinish())
        {
            return false;
        }
        if (!GLFWManager::Update())
        {
            Context::GetLogger().Error(" GLFWManager::Update with error !!!");
            return false;
        }
        if(!CurrentCamera().Render())
        {
            Context::GetLogger().Error(" Canvas::Render with error !!!");
            return false;
        }

        return true;
    }

    bool GLManager::LateUpdate()
    {
        if(!CurrentCamera().Clear())
        {
            Context::GetLogger().Error(" Canvas::Clear with error !!!");
            return false;
        }
        if(!GLFWManager::LateUpdate())
        {
            Context::GetLogger().Error(" GLFWManager::LateUpdate with error !!!");
            return false;
        }
        return true;
    }


    bool GLManager::Uninit()
    {
        if (!GLADManager::Uninit())
        {
            Context::GetLogger().Error(" GLADManager::Uninit with error !!!");
            return false;
        }
        if (!GLFWManager::Uninit())
        {
            Context::GetLogger().Error(" GLFWManager::Uninit with error !!!");
            return false;
        }

        return true;
    }


    const Camera& GLManager::CurrentCamera()
    {
        return *camera_;
    }

    const Scene& GLManager::CurrentScene()
    {
        return *scene_;
    }
}

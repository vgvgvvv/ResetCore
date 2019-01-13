#ifndef __IRENDERMANAGER_H__
#define __IRENDERMANAGER_H__

namespace ReGL
{
    class Camera;
    class Scene;
    class IRenderManager
    {
    public:
        virtual bool Init() = 0;
        virtual bool WillRender() = 0;
        virtual bool Update() = 0;
        virtual bool LateUpdate() = 0;
        virtual bool Uninit() = 0;

        virtual const Camera& CurrentCamera() = 0;
        virtual const Scene& CurrentScene() = 0;

    };
}


#endif
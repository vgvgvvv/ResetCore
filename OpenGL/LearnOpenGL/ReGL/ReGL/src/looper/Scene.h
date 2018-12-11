#ifndef __SCENE_H__
#define __SCENE_H__

#include <assimp/scene.h>

namespace ReGL
{
    class Scene
    {
    public:
        bool Init();
        bool Update();
        bool Uninit();
    private:
        aiScene* scene_{nullptr};
    };
}

#endif

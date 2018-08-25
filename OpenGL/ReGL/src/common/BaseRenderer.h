//
// Created by 董宸 on 2018/8/26.
//

#ifndef REGL_BASERENDERER_H
#define REGL_BASERENDERER_H

#include "Singleton.h"
#include "Component.h"
#include "Camera.h"
#include <vector>

using namespace Utility;

namespace ReGL{

    class BaseRenderer;

    /**
     * 遍历所有的Renderer和Camera以进行渲染
     */
    class RendererManager : public Singleton<RendererManager>{

    public:
        void AddRenderer(const BaseRenderer& renderer);

        void RemoveRenderer(const BaseRenderer& renderer);

        bool Init();

        bool Render();

        bool UnInit();

    private:
        std::vector<Camera> camera_list;
        std::vector<BaseRenderer> renderer_list_;
    };

    /**
     * 基础渲染对象
     */
    class BaseRenderer : public Component {



    };
}


#endif //REGL_BASERENDERER_H

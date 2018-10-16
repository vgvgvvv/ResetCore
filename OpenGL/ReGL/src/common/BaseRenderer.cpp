//
// Created by ¶­å· on 2018/8/26.
//

#include "BaseRenderer.h"

namespace ReGL{

    void RendererManager::AddRenderer(const BaseRenderer &renderer) {

    }

    void RendererManager::RemoveRenderer(const BaseRenderer &renderer) {

    }

    void RendererManager::AddCamera(const Camera &camera) {

    }

    void RendererManager::RemoveCamera(const Camera &camera) {

    }

    bool RendererManager::Init() {
        return true;
    }

    bool RendererManager::Render() {

        for(Camera& camera : camera_list){
            for(BaseRenderer& renderer : renderer_list_){
                renderer.Render(camera);
            }
        }

        return true;
    }

    bool RendererManager::UnInit() {
        return true;
    }



    void BaseRenderer::Render(Camera &camera) {

    }
}
#ifndef __MESHRENDERER_H__
#define __MESHRENDERER_H__
#include "Renderer.h"

namespace ReGL
{
    class MeshRenderer : public Renderer
    {
        bool Render() override;
    };
}

#endif
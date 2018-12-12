#ifndef __MESHRENDERER_H__
#define __MESHRENDERER_H__
#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"

namespace ReGL
{
    class MeshRenderer : public Renderer
    {
    public:
        ~MeshRenderer() override = default;
        bool Render() override;
    private:
        Mesh* mesh_;
        Material* material_;
    };
}

#endif
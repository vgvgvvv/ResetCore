#ifndef __RENDERER_H__
#define __RENDERER_H__
#include <list>

namespace ReGL
{
    class Renderer
    {
    public:
        virtual ~Renderer() = default;
        virtual bool Render() = 0;
    };

    class RendererManager
    {
    public:
    private:
        static std::list<Renderer*> renderers_;
    };
}

#endif
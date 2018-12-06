#ifndef __RENDERER_H__
#define __RENDERER_H__

namespace ReGL
{
    class Renderer
    {
    public:
        virtual bool Render() = 0;
    };
}

#endif
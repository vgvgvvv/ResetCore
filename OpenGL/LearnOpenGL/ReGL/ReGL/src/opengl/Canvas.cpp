#include "Canvas.h"
#include <glad/glad.h>

namespace ReGL
{
    bool Canvas::Render()
    {
        
    }


    bool Canvas::Clear()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        return true;
    }
}

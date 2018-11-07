//
// Created by 董宸 on 2018/11/7.
//

#ifndef ANDROIDLIB_PNGLOADER_H
#define ANDROIDLIB_PNGLOADER_H


#include <cstddef>
#include <png.h>
#include <memory>

#include <GLES2/gl2.h>

namespace RONDK{
    class PngLoader {
    public:
        void Load(const void *pData, size_t size);
        void SetTexture(GLuint texture) { m_texture = texture; }
        void UpdateTexture();
        bool HasLoaded() const { return m_hasLoaded; }
        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; };

    private:
        std::unique_ptr<unsigned char[]> m_data;
        bool m_hasLoaded = false;
        GLuint m_texture = 0;
        GLenum m_format = 0;
        GLint m_alignment = 1;
        size_t m_dataSize = 0;
        uint32_t m_width = 0;
        uint32_t m_height = 0;
    };
}

#endif //ANDROIDLIB_PNGLOADER_H

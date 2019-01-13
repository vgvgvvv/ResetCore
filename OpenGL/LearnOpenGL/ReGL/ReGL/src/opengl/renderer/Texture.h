#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <cstdint>

namespace ReGL
{
    class Texture
    {
    public:
        Texture(int height, int width) : texture_id_(0), height(height), width(width) {}
        ~Texture() = default;
        bool Load(const uint8_t* bytes);

    private:
        uint32_t texture_id_;
        int height;
        int width;
    };
}


#endif // __TEXTURE_H__

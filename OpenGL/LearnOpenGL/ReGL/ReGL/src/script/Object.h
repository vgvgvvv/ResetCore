#ifndef __OBJECT_H__
#define __OBJECT_H__
#include <cstdint>

namespace ReGL
{
    class Object
    {
    public:
        uint64_t UID() const { return uid_; }
        Object()
        {
            uid_ = ++current_uid_;
        }
        virtual ~Object() = default;
        virtual bool Destroy();
    protected:
        bool destroyed_{false};
    private:
        static uint64_t current_uid_;
        uint64_t uid_{0};
    };

}

#endif // __OBJECT_H__

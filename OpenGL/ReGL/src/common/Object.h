//
// Created by ¶­å· on 2018/7/29.
//

#ifndef REGL_OBJECT_HPP
#define REGL_OBJECT_HPP

#include <string>

namespace ReGL{
    class Object {
    public:
        Object(){
            guid_ = ++current_guid_;
        }
        virtual ~Object() = default;
        virtual void Destroy() = 0;
        virtual std::string ToString() = 0;

    uint64_t GUID(){ return guid_;}

    private:
        uint64_t guid_;
        static uint64_t current_guid_;
    };

}



#endif //REGL_OBJECT_HPP

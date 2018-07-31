//
// Created by ¶­å· on 2018/7/29.
//

#ifndef REGL_OBJECT_HPP
#define REGL_OBJECT_HPP

#include <string>

namespace ReGL{
    class Object {
    public:
        Object() = default;
        virtual ~Object() = default;
        virtual std::string ToString() = 0;
    };
}



#endif //REGL_OBJECT_HPP

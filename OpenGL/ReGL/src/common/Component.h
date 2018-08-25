//
// Created by 董宸 on 2018/8/26.
//

#ifndef REGL_COMPONENT_H
#define REGL_COMPONENT_H

#include "Object.h"

namespace ReGL{
    /**
     * 基础组件，在GameObject中进行组合
     */
    class Component : public Object{
    public:
        Component() = default;
    };
}



#endif //REGL_COMPONENT_H

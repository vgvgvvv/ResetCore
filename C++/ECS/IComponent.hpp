//
// Created by 董宸 on 19/01/2018.
//

#ifndef RESETCORE_ICOMPONENT_HPP
#define RESETCORE_ICOMPONENT_HPP

namespace ResetCore{
    class IComponent{
    public:
        friend class Entity;
    protected:
        IComponent() = default;

    };
}


#endif //RESETCORE_ICOMPONENT_HPP

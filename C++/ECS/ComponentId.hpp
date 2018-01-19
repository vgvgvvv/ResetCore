//
// Created by 董宸 on 19/01/2018.
//

#ifndef RESETCORE_COMPONENTID_HPP
#define RESETCORE_COMPONENTID_HPP

#include <TypeDefine.hpp>
#include <vector>
#include "IComponent.hpp"

namespace ResetCore{

    typedef Ruint ComponentId;
    typedef std::vector<ComponentId> ComponentIdList;

    class ComponentTypeId {
    public:
        /**
         * 获取ComponentID
         * @tparam T
         * @return
         */
        template <typename T>
        static const ComponentId Get(){
            //判断为IComponent的子类并且不是IComponent
            static_assert(std::is_base_of<IComponent, T>::value && !std::is_same<IComponent, T>::value,
                          "Class type must be derived from IComponent");
            static ComponentId id = mCounter++;
            return id;
        }

        /**
         * 获取当前组件数量
         * @return
         */
        static const Ruint Count(){
            return mCounter;
        }

    private:
        static Ruint mCounter;
    };

    Ruint ComponentTypeId::mCounter = 0;
}



#endif //RESETCORE_COMPONENTID_HPP

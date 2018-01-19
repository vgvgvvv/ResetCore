//
// Created by 董宸 on 19/01/2018.
//

#ifndef RESETCORE_SYSTEMCONTAINER_HPP
#define RESETCORE_SYSTEMCONTAINER_HPP


#include <vector>
#include "ISystem.hpp"

namespace ResetCore{
    class SystemContainer {
    public:
        SystemContainer() = default;
        /**
         * 添加系统
         * @param system
         * @return
         */
        auto Add(std::shared_ptr<ISystem> system) -> SystemContainer*;
        /**
         * 快捷添加系统
         * @tparam T
         * @return
         */
        template <typename T> inline auto Add() -> SystemContainer*;

        void Init();
        void Update();
        void FixedUpdate();
        void UnInit();
    private:
        std::vector<std::shared_ptr<ISystem>> systems;
    };


    template<typename T>
    auto SystemContainer::Add() -> SystemContainer * {
        return Add(std::shared_ptr<T>(new T()));
    }
}



#endif //RESETCORE_SYSTEMCONTAINER_HPP

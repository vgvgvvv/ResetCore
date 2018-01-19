//
// Created by 董宸 on 19/01/2018.
//

#ifndef RESETCORE_ISYSTEM_HPP
#define RESETCORE_ISYSTEM_HPP

namespace ResetCore{
    class ISystem{
    protected:
        ISystem() = default;

    public:
        virtual ~ISystem() = default;
        /**
         * 初始化
         */
        virtual void Init() = 0;
        /**
         * 更新
         */
        virtual void Update() = 0;
        /**
         * 后更新
         */
        virtual void FixedUpdate() = 0;
        /**
         * 卸载
         */
        virtual void UnInit() = 0;
    };

}

#endif //RESETCORE_ISYSTEM_HPP

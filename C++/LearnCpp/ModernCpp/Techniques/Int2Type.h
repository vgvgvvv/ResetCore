//
// Created by 35207 on 2019/11/30 0030.
//

#ifndef RESETCORE_INT2TYPE_H
#define RESETCORE_INT2TYPE_H

// 将数字映射到一个类型当中
// 可以通过一个编译时常量做到静态分发
template <int v>
struct Int2Type{
    enum {value = v};
};

/*
//如果这么写的话编译器会编译错误，因为一个类只能用一种写法
template <typename T, bool isPolymorphic>
class NiftyContainer{
 public:
    void DoSomething(){
        T* pSomeObj = ...;
        if(isPolymorphic){
            T* pNewObj = pSomeObj->Clone();
            // Do Something
        }else{
            T* pNewObj = new T(*pSomeObj);
            // Do Something
        }
    }
};
*/

/*
template <typename T, bool isPolymorphic>
class NiftyContainer{
    void DoSomething(Int2Type<false>){

    }

    void DoSomething(Int2Type<true>){

    }
public:
    //由于只会编译用到的模板，所以不会报错
    void DoSomething(){
        DoSomething(Int2Type<isPolymorphic>);
    }
};
 */

#endif //RESETCORE_INT2TYPE_H

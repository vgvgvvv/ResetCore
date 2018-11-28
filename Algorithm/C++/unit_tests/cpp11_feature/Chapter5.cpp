//
// Created by dieseldong on 2018/11/27.
//

#include "gtest/gtest.h"
#include <iostream>
#include <memory>

using namespace std;

namespace CPP_11_FEATURE_CHAPTER_5{

    /**
     * 枚举类型在C++98中是全局的容易发生冲突，
     * 并且不同的编译器产生的大小不一样，容易产生移植问题
     *
     * 如果不进行封装则会对命名空间造成污染，
     * 但是如果自己封装成类之后有以下问题：
     * * 代码量大
     * * 不是POD类型
     * * 并且无法存入寄存器，性能堪忧
     *
     * 所以在C++11中使用强类型enum代替
     * 并且在C++11中可以指定具体类型
     */

    enum class TestEnum : int {
        ENUM_1,
        ENUM_2,
        ENUM_3};

    TEST(CPP_11_FEATURE, CHAPTER_5_STATIC_ENUM){
        cout << (int)TestEnum::ENUM_1 << endl;
    }


    /**
     * 堆内存管理：智能指针与垃圾回收
     *
     * 我们在内存管理的时候经常遇到如下问题：
     * * 野指针
     * * 重复释放
     * * 内存泄漏
     */

    /**
     * 在C++98中有auto_ptr作为智能指针，但是有缺点：
     * * 拷贝是返回一个左值
     * * 不能调用delete[]
     *
     * C++11中则替换为：unique_ptr、shared_ptr以及weak_ptr
     */

    TEST(CPP_11_FEATURE, CHAPTER_t_SMART_POINTER){

        unique_ptr<int> up1(new int(11));

        //unique_ptr不允许赋值
        //unique_ptr<int> up2 = up1;

        cout << *up1 << endl;

        unique_ptr<int> up3 = move(up1);//现在up3是唯一的智能指针

        cout << *up3 << endl;
        //运行时错误
        //cout << *up1 << endl;

        up3.reset();    //显式释放内存
        up1.reset();    //显式释放内存

        //运行时错误
        //cout << *up3 << endl;

        shared_ptr<int> sp1(new int(22));
        shared_ptr<int> sp2 = sp1;

        cout << *sp1 << endl; //22
        cout << *sp2 << endl; //22

        sp1.reset();
        cout << *sp2 << endl; //22

    }

    /**
     * unique_ptr和shared_ptr与以前保持一致，但是在内存共享上还是有一定区别的
     * unique_ptr与内存直接绑定，不能共享内存。
     * 如果使用move则会将内存移动到另一个指针上。原本的指针会无法使用。
     *
     * unique_ptr删除了拷贝构造函数而保留了移动构造函数，所以构造之后右值对象即被窃取。
     *
     * shared_ptr则允许多个对象共享拥有同一堆分配对象的内存。内部使用引用计数。
     * 其中一个对象放弃所有权并不会有什么影响，只有在引用为0的时候才会释放内存。
     *
     */

    /**
     * 智能指针中还有一个weak_ptr的类，他可以指向shared_ptr指针指向的内存但不拥有该对象，
     * 如果指向的对象已经失效则返回空指针。
     *
     * 如果调用weak_ptr的lock函数则会返回一个shared_ptr，用法也就相同了。
     *
     * 使用方式如下。
     *
     *
     */

    void CheckWeakPtr(weak_ptr<int> &wp){
        shared_ptr<int> sp = wp.lock();
        if(sp != nullptr){
            cout << "still " << *sp << endl;
        }
        else{
            cout << "pointer is invalid." << endl;
        }
    }

    TEST(CPP_11_FEATURE, CHAPTER_5_WEAK_PTR){

        cout << endl;

        shared_ptr<int> sp1(new int(22));
        shared_ptr<int> sp2 = sp1;

        weak_ptr<int> wp = sp1;
        cout << *sp1 << endl;   //22
        cout << *sp2 << endl;   //22
        CheckWeakPtr(wp);

        sp1.reset();
        cout << *sp2 << endl;   //22
        CheckWeakPtr(wp);

        sp2.reset();
        CheckWeakPtr(wp);

    }

    /**
     * 智能指针能够比较好地进行内存管理，但是需要声明还是很恶心，
     * 所以还有垃圾回收
     *
     * （实际上还没有啥编译器实现了最小支持的垃圾回收
     */
}
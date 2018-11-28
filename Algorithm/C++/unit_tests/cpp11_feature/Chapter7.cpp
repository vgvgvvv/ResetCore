//
// Created by dieseldong on 2018/11/28.
//

#include <iostream>
#include "gtest/gtest.h"

using namespace std;

namespace CPP_11_FEATURE_CHAPTER_7{

    /**
     * 从0到NULL到nullptr
     * NULL指针会和int类型混淆起来所以并不好用，导致二义性
     */

    void f(char* p){
        cout << "invoke f(char*) " << endl;
    }
    void f(int){
        cout << "invoke f(int) " << endl;
    }
    TEST(CPP_11_FEATURE, CHAPTER_7_NULLPTR){
        //会采用char*
        f(nullptr);
        //采用int
        f(0);
    }

    /**
     * nullptr_t可以用于声明指针空值的变量
     * * nullptr_t类型的数据都是等价的并且行为一直
     * * 不能转换为非指针类型
     * * 不使用算数运算符
     * * 可以用于关系表达式，只能与nullptr_t或者指针类型比较
     *   当且仅当关系运算符为== <= >=为true
     *
     *   如果向函数中传nullptr需要显示转换类型以选择特定的重载函数
     *
     *   nullptr的大小与void*相同
     *   nullptr比(void*)0更加方便，例如可以隐式转换（不过在C语言中void*也可以）
     *   nullptr本身实际是一个右值常量
     *
     *   一般只需要记住不要对nullptr取地址就可以了
     */

    /**
     * 默认函数的控制
     *
     * 通过default关键字可以让类中的函数回归平凡
     */

    class TwoCstor{
    public:
        TwoCstor() = default;
        TwoCstor(int i) : data(i){}

    private:
        int data;
    };

    //TwoCstor是一个POD类型
    TEST(CPP_11_FEATURE, CHAPTER_7_DEFAULT_METHOD){
        cout << endl;
        cout << is_pod<TwoCstor>::value << endl;
    }

    class NoCopyCstor{
    public:
        NoCopyCstor() = default;
        NoCopyCstor(const NoCopyCstor &) = delete;
    };

    TEST(CPP_11_FEATURE, CHAPTER_7_DELETE_METHOD){
        NoCopyCstor a;
        //无法编译通过
        //NoCopyCstor b(a);
    }

    /**
     * 实际上类型外也可以使用default
     * 我们可以删除一些函数以避免一些不需要的隐式转换，
     * 不建议将delete和explicit合用，会产生混乱性
     */

    void Func(int i){}
    void Func(char c) = delete;

    TEST(CPP_11_FEATURE, CHAPTER_7_DELETE_CUSTOM_FUNC){
        Func(3);
        //该语句无法编译通过
        //Func('a');
    }
    /**
     * 你甚至可以禁用operator new来禁止在堆上分配该对象
     */
     class NonHeapAlloc{
     public:
         void *operator new(std::size_t) = delete;
     };

     TEST(CPP_11_FEATURE, CHAPTER_7_DELETE_HEAP_ALLOC){
         NonHeapAlloc nh;
         //无法编译通过
         //NonHeapAlloc * nhp = new NonHeapAlloc();
     }

     /**
      * 通过删除析构函数可以限制自定义类型在栈上的构造
      */
      class NonStackAlloc{
      public:
          ~NonStackAlloc() = delete;
      };

      TEST(CPP_FEATURE, CHAPTER_7_DELETE_STACK_ALLOC){
          //无法编译通过
          //NonStackAlloc ns;
      }

      /**
       * lambda表达式
       * 
       */

}
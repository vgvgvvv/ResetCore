//
// Created by 35207 on 2018/11/23 0023.
//

#include "gtest/gtest.h"
#include <iostream>
#include <utility>

using namespace std;

namespace CPP_11_FEATURE_CHAPTER_3 {


    /**
     *
     * 继承构造函数
     * 将使用基类类型函数的思想延展到构造函数上
     *
     * 需要注意默认值的情况，造成多个构造函数版本的情况，
     * 有默认参数的构造函数必须小心
     *
     */
     class ClassA{
     public:
         ClassA(){}
         ClassA(int i){}
         ClassA(double d, int i){};
     };

     class ClassB : ClassA{
     public:
         using ClassA::ClassA; //继承构造函数
     };

     class ClassC{
     public:
         ClassC(){}
         ClassC(int i){}
     };

     /**
      * 多继承的情况
      */
     class ClassD : ClassA, ClassC {
     public:
         using ClassA::ClassA;
         using ClassC::ClassC;
         ClassD(int i){}    //防止构造函数冲突
     };

     TEST(CPP_11_FEATURE, CHAPTER_3_DRIVER_CONSTRUCTOR){
        ClassB b(100);  //透传调用到A的构造函数
     }


     /**
      * 委派构造函数
      */
     class ClassE{
     public:
         ClassE() { Init(); }

         /**
          * C++98的做法
          * ClassE(int a) : a(a) { Init(); }
          * ClassE(char* b) : b(b) { Init(); }
          */

         /**
          * C++11的做法
          * 委派构造函数不能有初始化表
          */

         ClassE(int a) : ClassE() { this->a = a; }
         ClassE(char* b) : ClassE() { this->b = b; }

     private:
         int a { 100 };
         char* b { (char*)"aa" };
         void Init(){};
     };

     /**
      * 可以换一种写法
      * 我们无需通过初始化表初始化变量
      */
     class ClassF{
     public:
         //委派构造函数本身也可以是委派构造函数
         //不过需要注意循环引用的情况
         ClassF() : ClassF(1){}
         ClassF(int i) : ClassF(i, 'a'){}
         ClassF(char b) : ClassF(1, b){}

     private:
         ClassF(int i, char b) : i(i), b(b) {}
         int i;
         char b;
     };

     /**
      * 委派构造函数让模板构造函数变成一个可能
      */

     /**
      * 右值引用：移动语义和完美转发
      *
      * 如果一个类里面有指针成员的话要特别小心拷贝构造函数的编写，
      * 容易出现内存泄漏
      */

     class ClassG{
     public:
         ClassG() : d(new int(0)) {
             cout << "Construct: " << ++n_cstr << endl;
         }
         //需要自己实现拷贝构造函数，默认拷贝构造函数会造成泄漏
         ClassG(const ClassG & g) : d(new int(*g.d)) {
             cout << "Copy Construct: " << ++n_cptr << endl;
         }
         /**
          * 移动语义
          * 如果直接使用拷贝构造函数的话会造成多次的临时构造函数的开销
          * 如果使用移动语义的话则不会有反复开销
          */
         ClassG(ClassG && g) : d(g.d){
             g.d = nullptr;
             cout << "Move construct: " << ++n_mvtr << endl;
         }
         ~ClassG() {
             cout << "Destruct: " << ++n_dstr << endl;
             delete d;
         }
         int getd(){
             return *d;
         }

     private:
         int * d;
         static int n_cstr;
         static int n_dstr;
         static int n_cptr;
         static int n_mvtr;
     };

     int ClassG::n_cstr = 0;
     int ClassG::n_cptr = 0;
     int ClassG::n_dstr = 0;
     int ClassG::n_mvtr = 0;

     ClassG GetClassG(){
         ClassG result;
         cout << "Resource from " << __func__ << endl;
         return result;
     }
     TEST(CPP_11_FEATURE, CHAPTER_3_CONSTRUCT){
         ClassG g = GetClassG();
        cout << "Resource from " << __func__ << endl;
     }

     /**
      * 实际在C++98/03时移动语义已经存在了，例如：
      * 某些情况下的拷贝构造函数的省略
      * 智能指针的拷贝
      * 链表拼接
      * 容器内的置换
      */


     /**
      * 左值、右值以及右值引用
      * 例如：a = b + c
      * a为左值，b+c为右值
      *
      * 更广泛的说法是：可以取地址的是左值，不能取地址的是右值
      *
      * 在C++11中右值有两个概念：
      * 1.将亡值（xvalue, eXpiring Value）
      *     不与对象关联的值，例如零食变量值，字面量值，返回值，lambda表达式等
      *
      * 2.纯右值(prvalue, Pure Rvalue)
      *     和右值引用相关的表达式，通常是将要被移动的对象，比如右值引用T&&的函数返回值
      *     std::move的返回值或者转换为T&&的类型转换函数的返回值。
      *
      * 其余的可以标识函数、对象的都是左值。
      *
      * 实际上左右值很难归纳，即便归纳也需要大量解释。
      *
      * 右值引用就是对一个右值进行引用的类型。
      */
      ClassG ReturnRvalue(){
         return ClassG();
      }

      TEST(CPP_11_FEATURE_CHAPTER_3, CHAPTER_3_RVALUE){
          cout << endl;
          //使用右值对返回值续命
          ClassG && g1 = ReturnRvalue();
          cout << g1.getd() << endl;

          //未使用右值
          ClassG g2 = ReturnRvalue();
          cout << g2.getd() << endl;
      }


      TEST(CPP_11_FEATURE, CHAPTER_3_RVALUE_ERROR){
        // 右值引用无法绑定到左值上
        // int c;
        // int && b = c;

        //常量左值引用可以绑定右值
        const bool & judgement = true;
        //未使用左值
        const bool judgement2 = true;
        //实际上使用常量左值引用也可以减少临时对象的开销
      }


      struct Copyable{
          Copyable() = default;
          Copyable(const Copyable & o){
              cout << "copied" << endl;
          }
          Copyable(Copyable&&) noexcept { }
      };

      Copyable ReturnCopyableRvalue(){ return Copyable(); }
      void AcceptVal(Copyable){}
      void AcceptRef(const Copyable & ){}

      TEST(CPP_11_FEATURE, CHAPTER_3_LVALUE_REF){
          cout << endl;
          cout << "Pass by value" << endl;
          AcceptVal(ReturnCopyableRvalue());    //临时拷贝传值
          cout << "Pass by ref" << endl;
          AcceptRef(ReturnCopyableRvalue());    //临时值作为引用传递
      }

      void AcceptRvalueRef(Copyable&& s){
          Copyable news = std::move(s);
      }
      /**
       * 判断一个值是否是右值
       */
      TEST(CPP_11_FEATURE, CHAPTER_3_IS_REFRENCE){
          cout << is_reference<string &&>::value << endl;
          cout << is_rvalue_reference<string &&>::value << endl;
          cout << is_lvalue_reference<string &&>::value << endl;
      }

      /**
       * 使用std::move强制转换为右值
       *
       * std::move实际上不会移动任何东西，只是将左值强制转换为右值引用
       * 等于：
       * static_cast<T&&>(lvalue);
       * 在调用拷贝构造函数的时候我们期望将所有的值都拿过来，例如
       * class Moveable{
       * public:
       *    Moveable(Moveable && m) :
       *        i(m.i), h(std::move(m.h)){
       *        m.i = nullptr;
       *    }
       * private:
       *    int * i;
       *    int h;
       * }
       *
       * 其中i为指针成员，而h为普通的成员
       * 如果不使用move操作符，那么拷贝的成员对象h会因为左值生命周期结束而直接销毁，
       * 所以需要转换为右值以延长生命周期
       * 如果没有移动构造函数，也可以直接使用接收左值引用的拷贝构造函数，只是会有
       * 多余的临时变量开销，不会有太大问题。
       */

        /**
         * 移动语义的一些其他问题
         * 如果把移动构造函数定义为：
         * Moveable(const Moveable &&)
         * 或者把函数定义为
         * const Moveable ReturnRValue()
         * 都会导致临时变量常量化导致无法使用移动语义，所以在写的时候一定要
         * 排除不必要的const
         *
         *
         */

        /**
         * C++11中拷贝/移动构造函数有3种形式：
         * T Object(T &)
         * T Object(const T &)
         * T Object(T &&)
         * 其中常量左值是拷贝构造函数，右值引用版本则是移动构造版本，默认情况下
         * 编译器会隐式生成一个移动构造函数（没有用到则不会生成）默认移动构造函数
         * 实际上也只是按位拷贝，无法满足需求，如果一些简单的不包含资源的类而言没问题，
         * 拷贝就是移动，移动就是拷贝。
         *
         * C++11之前大多构造都是拷贝语义，如果只有移动语义的会比较有趣，
         * 表示只能被移动而不能被拷贝，一般都是资源类型：智能指针、文件流等，
         * 我们可以通过一些辅助的模板类来判断一个类型是否可以移动
         */
         TEST(CPP_11_FEATURE, CHAPTER_3_MOVE_TYPE_TRAITS){
             cout << is_move_constructible<int>::value << endl;
             cout << is_trivially_move_constructible<int>::value << endl;
             cout << is_nothrow_move_assignable<int>::value << endl;
         }

     /**
      * 有了移动语义之后还有一个应用就是高性能的置换
      */
      template <typename T>
      void swap(T& a, T& b){
          T tmp(move(a));
          a = move(b);
          b = move(tmp);
      }

      /**
       * 这个类型如果是可移动的则会使用移动语义，否则则会采用拷贝语义
       */
      TEST(CPP_11_FEATURE, CHAPTER_3_MOVE_SWAP){
          cout << endl;
          int a = 100;
          int b = 200;
          swap(a, b);
          cout << "a:" << a << " b:" << b << endl;
      }

      /**
       * 移动构造函数要尽可能防止异常的发生，如果移动语义未完成而抛出异常
       * 是非常危险的。
       * 所以要加上一个noexcept关键字，
       * 保证在抛出异常的时候直接终止应用而不是指针悬挂状态
       *
       * 除此之外：
       * 编译器的RVO(Return Value Optimization)/NRVO(Named Return Vlaue optimization)优化
       * 如果打开RVO/NRVO，
       * 那么从ReturnValue函数中变量拷贝/移动构造临时变量以及
       * 临时变量拷贝/移动构造b的二重奏通通没了
       *
       * A ReturnRvalue() { A a(); return a; }
       * A b = ReturnRvalue();
       *
       * b变量直接使用了a的地址，任何的拷贝以及移动都没了，不过这个并不是所有时候都有效，
       * 所以移动语义还是非常有用的。
       *
       */
}
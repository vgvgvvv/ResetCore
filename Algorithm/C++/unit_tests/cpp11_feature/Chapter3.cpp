//
// Created by 35207 on 2018/11/23 0023.
//

#include "gtest/gtest.h"
#include <iostream>

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
         char* b { "aa" };
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
         ClassG(ClassG & g) : d(new int(*g.d)) {
             cout << "Copy Construct: " << ++n_cptr << endl;
         }
         ~ClassG() {
             cout << "Destruct: " << ++n_dstr << endl;
             delete d;
         }

     private:
         int * d;
         static int n_cstr;
         static int n_dstr;
         static int n_cptr;
     };

     int ClassG::n_cstr = 0;
     int ClassG::n_cptr = 0;
     int ClassG::n_dstr = 0;

     /**
      * 移动语义
      * 如果直接使用拷贝构造函数的话会造成多次的临时构造函数的开销
      * 如果使用移动语义的话则不会有反复开销
      */
     class ClassG{
     public:
         ClassG() : d(new int(3)){
             cout << ""
         }

     private:
         int *d;
     };



}
//
// Created by 35207 on 2018/11/23 0023.
//

#include "gtest/gtest.h"
#include <iostream>
#include <cassert>
#include <climits>

using namespace std;

namespace CPP_11_FEATURE_CHAPTER_2{

/**
 * 判断是否为c++11
 */
#if __cplusplus < 201103L
#error "should use c++11 implementtion"
#endif

    /**
     * 预定义宏
     */
    TEST(CPP_11_FEATURE, CHAPTER_2_MACRO){
        cout << endl;
        //行数
        cout << __LINE__ << endl;
        //文件
        cout << __FILE__ << endl;
        //输出当前的函数名(不允许作为函数的默认参数出现)
        cout << __func__ << endl;
        //如果编译器目标系统环境有完整的标准C库则为1否则为0
        cout << __STDC_HOSTED__ << endl;
        //编译器实现是否与C标准一致
        cout << __STDC__ << endl;
        //是否是cpp
        //在C++03中该值为199711L，在C++11中则为201103L

        cout << __cplusplus << endl;
    }

    /**
     * _Pragma的使用
     */
    TEST(CPP_11_FEATURE, CHAPTER_2_PRAGMA){
        //_Pragma("once")效果与#pragma once效果一致
    }


    /**
     * 变长参数
     */
    TEST(CPP_11_FEATURE, CHAPTER_2_VA_ARGS){
#define TEST_VA_ARGS(...) cout << __VA_ARGS__ << endl

        cout << endl;
        TEST_VA_ARGS("Hello Args!");

#undef TEST_VA_ARGS
    }

    /**
     * 宽窄字符串连接
     */

    /**
     * long long 的支持
     */
    TEST(CPP_11_FEATURE, CHAPTER_2_LONG_LONG){
        cout << endl;
        cout << LLONG_MIN << endl;
        cout << LLONG_MAX << endl;
        cout << ULLONG_MAX << endl;
    }


    /**
     * assert的使用
     */
    TEST(CPP_11_FEATURE, CHAPTER_2_ASSERT){
        //如果有NDEBUG的宏 assert则会被展开为无意义的语句。
        int n = 100;
        assert(n > 0);
    }

    const int asset_test = 100;
    /**
     * static_assert的使用
     */
    TEST(CPP_11_FEATURE, CHAPTER_2_STATIC_ASSERT){
        //编译时断言
        static_assert(asset_test == 100, "static assert!!");
    }

    /**
     * noexcept修饰符
     * 表示不会抛出异常，如果有异常则直接终止进程
     */
    TEST(CPP_11_FEATURE, CHAPTER_2_NOEXCEPT){}

    /**
     * 表示不会抛出异常
     */
    void noexcpt_func() noexcept{

    }

    /**
     * 表示有可能发生异常
     */
    void hasexcept_func() noexcept(false){

    }

    /**
     * 快速初始化成员变量
     * 初始化列表优先于就地初始化
     * 注意：非常量的静态成员变量还是需要到头文件以外去定义，
     *      保持编译时类静态成员的定义最后只存在于一个目标文件中
     */
    class InitTest{
    public:
        const int constVar = 100;//常量可以通过‘=’初始化
        int nonConstVar {100};//非常量可以通过花括号初始化
        const char * strVar {"111"};
    };

    /**
     * 非静态成员的sizeof
     */
    TEST(CPP_11_FEATURE, CHAPTER_2_SIZEOf){
        InitTest test;
        cout << endl;
        cout << sizeof(test.nonConstVar) << endl;

        //C++98的做法
        //sizeof(((InitTest*)0)->nonConstVar)
    }

    /**
     * 扩展的friend语法
     * C++98:friend class TYPE
     * C++11:friend TYPE
     * 模板中也可以支持friend了
     *
     * example：
     *
     * template <typename T> class People {
     *      friend T;
     * };
     *
     * People<P> PP;//此处P为People的friend
     *
     * 进行测试的时候可以将测试代码作为friend
     *
     */

    template <typename T>
    class TestFriend {
        friend T;
    private:
        int privateVar {10};
    };
    TEST(CPP_11_FEATURE, CHAPTER_2_FRIEND_EX){

    }

    /**
     * final/override控制
     * 通过final禁止重写
     */

    /**
     * 模板函数的默认模板参数
     */
    template <typename T = int>
    class DefTempClass{};//C++98通过 C++11通过

    template <typename T = int>
    void DefTempMethod(){};//C++98失败 C++11通过

    /**
     * 外部模板
     * (强制实例化，只会实例化一份模板)
     * extern template void func<int>(int);
     */

    /**
     * 局部和匿名类型作为模板实参
     */
    template <typename T>
    class X{};
    template <typename T>
    void TempFunc(T t){};
    struct A {} a;
    struct { int i; }b; //匿名类
    typedef struct {int i;} B;//匿名类

    TEST(CPP_11_FEATURE, CHAPTER_2_LOCAL_TYPE_TEMPLATE){

        struct C{} c;//局部类

        X<A> x1;    //C++98通过 C++11通过
        X<B> x2;    //C++98错误 C++11通过
        X<C> x3;    //C++98错误 C++11通过

        TempFunc(a);//C++98通过 C++11通过
        TempFunc(b);//C++98错误 C++11通过
        TempFunc(c);//C++98错误 C++11通过
    }

    /**
     * 匿名类型声明不能再模板实参位置
     * template <typename T> struct MyTemplate {};
     *
     * int main(){
     *      MyTemplate<struct { int a; }> t;//非法
     * }
     */

}


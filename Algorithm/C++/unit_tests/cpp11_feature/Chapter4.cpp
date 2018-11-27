//
// Created by 董宸 on 2018/11/26.
//

#include "gtest/gtest.h"
#include <type_traits>
#include <algorithm>

using namespace std;

namespace CPP_11_FEATURE_CHAPTER_4 {

    /**
     * 模板中的两个右移符号不会再发生歧义了
     */

    /**
     * auto类型推导
     * 实际上和c#的var差不多
     * 使用stl的时候更加容易了
     *
     * auto同时也能帮助我们判断类型以避免精度丢失的问题
     * 虽然也不是所有情况都可以。
     * 该数值最大值+1之后类型还是原来的类型，无法避免溢出
     *
     * auto关键字还可以帮助我们跨平台，在不同平台编译的时候看清楚直接使用auto
     *
     */

    /**
     * auto使用细则
     * 对于指针 auto* 与 auto是一样的
     * 但是对于引用而言必须使用auto&
     *
     * const与volatile 可以和auto一起用，
     * 但是通过auto声明的变量无法带走长良性以及易失性，
     * 所以在声明的时候需要我们自己写上
     */

    TEST(CPP_11_FEATURE, CHAPTER_4_AUTO_USAGE) {
        int x;
        int *y = &x;
        //double foo();
        //int& bar();

        auto *a = &x;   //int*
        auto &b = x;    //int&
        auto c = y;     //int*
        auto *d = y;    //int*
        //auto* e = &foo();   //编译失败 指针无法指向临时变量
        //auto& f = foo();    //编译失败 非常量左值无法绑定临时变量
        //auto g = bar(); //int
        //auto& h = bar();//int&


        const auto constVar = 1;    // 拥有常量性
        auto nonconstVar = constVar;// 没有常量性

    }

    /**
     * auto 的限制
     * * auto不能是形参，如果需要泛型参数还是需要求助于模板
     * * 结构体的非静态成员不能是auto的
     * * auto不能以数组形式存在 例如：auto z[3]
     * * 实例化模板的时候用auto作为模板参数是禁止的
     *
     * 其实这些规则在C#中也是一样不允许的
     */

    /**
     * typeid与decltype
     * RTTI的机制是为每个类型产生一个type_info类型的数据，
     * 程序员可以在程序中使用typeid随时查询一个返回类型的名字。
     * C++11中又添加了hash_code这个成员函数，
     * 返回该类型的唯一的哈希值来进行类型比较
     */

    class White {
    };

    class Black {
    };

    TEST(CPP_11_FEATURE, CHAPTER_4_TYPEID) {
        White a;
        Black b;
        cout << typeid(a).name() << endl;
        cout << typeid(b).name() << endl;

        White c;

        bool a_b_sametype = (typeid(a).hash_code() == typeid(b).hash_code());
        bool a_c_sametype = (typeid(a).hash_code() == typeid(c).hash_code());

        cout << "A == B ? " << a_b_sametype << endl;
        cout << "A == C ? " << a_c_sametype << endl;
    }

    /**
     * RTTI实际上会有一些运行时开销
     * 可以通过选项关闭，通常我们可以在编译期确定类型会更好
     *
     */

    /**
     * decltype也可以进行推导
     */
    TEST(CPP_11_FEATURE, CHAPTER_4_DECLTYPE) {
        int i = 0;
        decltype(i) j = 0;
        cout << typeid(j).name() << endl;

        float a;
        double b;
        decltype(a + b) c;
        cout << typeid(c).name() << endl;
    }

    /**
     * decltype可以对匿名类进行操作
     */

    enum {
        K1, K2, K3
    } anon_e;
    union {
        decltype(anon_e) key;
        char *name;
    } anon_u;

    struct {
        int d;
        decltype(anon_u) id;
    } anon_s[100];

    TEST(CPP_11_FEATURE, CHAPTER_4_DELTYPE_FOR_NO_NAME_CLASS) {
        decltype(anon_s) as;
        as[0].id.key = decltype(anon_e)::K1;
    }

    /**
     * decltype可以改进模板的部分能力
     * 动态改变模板类型
     */
    template<typename T1, typename T2>
    void Sum(T1 &t1, T2 &t2, decltype(t1 + t2) &s) {
        s = t1 + t2;
    }

    /**
     * decltype 4 准则
     * * 如果e是一个没带括号的标记表达式或者类成员访问表达式。
     *   那么decltype(e)就是所命名的实体的类型。
     *   此外，如果e是一个被重载的函数，编译时会报错。
     * * 否则，e的类型是T，若果e是一个将亡值，则decltype(e)为T&&
     * * 否则，e的类型是T，如果e是左值则decltype(e)为T&
     * * 否则，e的类型是T，则decltype(e)为T
     *
     * 最好的方法是先在其他语句进行初始化，因为左值引用总是需要初始化的。
     * 否则容易出现 decltype((i)) = d 类似的无法编译通过的情况
     */

    /**
     * decltype是能够带走cv限制符(const以及volatile)的
     * 不过对象的定义中有const或者volatile限制符的时候，
     * 使用decltype进行推导其*成员*不会继承
     */

    /**
     * 追踪返回类型
     */

    /**
     * 以下写法无法编译通过，需要通过auto来进行
     * template <typename T1, typename T2>
     * decltype(t1 + t2) Sum(T1& t1, T2& t2){
     *    return t1+t2;
     * }
     * 下面的写法自动推导返回类型
     */
    template<typename T1, typename T2>
    auto Sum(T1 &t1, T2 &t2) -> decltype(t1 + t2) {
        return t1 + t2;
    }

    /**
     * 类型后置为模板推导提供了不少可能性
     * 定义返回函数指针的函数也会更简单
     * 以下两种写法是相同的，明显新的写法更加清晰
     */
    int (*(*pf()))() {
        return nullptr;
    }

    auto pf1() -> auto (*)() -> int (*)() {
        return nullptr;
    }

    /**
     * 在转发函数中也有大量的应用
     * 可以在不同类型下返回不同的值
     */
    double TestDeclForward(int a) {
        return (double) a + 0.1;
    }

    int TestDeclForward(double b) {
        return (int)b;
    }

    template<typename T>
    auto DoDeclForward(T t) -> decltype(TestDeclForward(t)) {
        return TestDeclForward(t);
    }

    TEST(CPP_11_FEATURE, CHAPTER_4_DECLTYPE_AND_FORWARD) {
        cout << DoDeclForward(2) << endl;
        cout << DoDeclForward(0.5) << endl;
    }

    int ForEachAction(int &e) { cout << e << '\t'; }
    /**
     * 基于范围的for循环
     */
    TEST(CPP_11_FEATURE, CHAPTER_4_RANGE_FOR) {

        //C++98时需要这样
        int arr[5] = {1, 2, 3, 4, 5};
        int *p;
        for (p = arr; p < arr + sizeof(arr) / sizeof(arr[0]); ++p) {
            cout << *p << '\t';
        }

        //我们还可以使用for_each来遍历
        for_each(arr, arr + sizeof(arr) / sizeof(arr[0]), ForEachAction);

        //C++11中引入了基于范围的for
        for (int &e : arr) {
            cout << e << '\t';
        }

        //也可以使用auto
        for (auto e : arr) {
            cout << e << '\t';
        }
    }
    /**
     * 需要注意地是范围for循环需要这个类有begin以及end函数，
     * 而数组则是第一个和最后一个元素的范围
     * 要求对象实现++和==等操作符
     * 而且数组大小无法确定的话也是无法使用基于范围的for的
     *
     * 用惯了迭代器的程序员应该注意，基于范围的for是已经解引用了的
     */

}
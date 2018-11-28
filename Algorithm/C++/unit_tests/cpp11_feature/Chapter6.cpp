//
// Created by 董宸 on 2018/11/28.
//

#include "gtest/gtest.h"
#include <iostream>
#include <cstdarg>

#ifndef WIN32
#include <pthread.h>
#endif

using namespace std;

/**
 * 提高性能以及操作硬件的能力
 */
namespace CPP_11_FEATURE_CHAPTER_6 {

    /**
     * 常量表达式
     * 运行时常量和编译时常量
     *
     * const大多描述的是运行时常量，而不是编译时常量
     */
    enum BitSet {
        V0 = 1 << 0,
        V1 = 1 << 1,
        V2 = 1 << 2,
        VMAX = 1 << 3,
    };

    /**
     * 通过constexpr可以帮助我们使用表达式表示常量
     * @return
     */
    constexpr int GetConst() { return 1; }

    template<int i = GetConst()>
    void LikeConst() {

    }

    /**
     * constexpr对函数有以下要求
     * * 函数体只有单一的return语句
     * * 函数必须有返回值
     * * 在使用前必须已有定义
     * * return返回表达式不能使用非常量表达式的函数，全局数据，必须是个常量表达式
     *
     * C++11中constexpr是不能用于修饰自定义类型的定义的
     * 正确的方式是定义自定义常量构造函数
     */

    struct MyConstType {
        constexpr MyConstType(int x) : i(x) {}

        int i;
    };

    constexpr MyConstType mt = {0};

    /**
     * 常量构造函数中，函数体必须为空
     * 初始化列表只能有常量表达式来赋值
     * 常量表达式不能作用域虚函数
     */

    /**
     * 常量表达式的其他应用
     * 模板的不确定性，模板函数的实例化结果不满足常量表达式需求，
     * 则constexpr会自动忽略而成为普通函数
     */
    struct NonLiteral {
        NonLiteral() { i = 5; }

        int i;
    };

    template<typename T>
    constexpr T ConstExp(T t) {
        return t;
    }

    TEST(CPP_11_FEATURE, CHAPTER6_CONSTEXPR_TEMPLATE) {
        NonLiteral nl;
        NonLiteral nl1 = ConstExp(nl);
        //无法通过编译
        //constexpr NonLiteral nl2 = ConstExp(nl);
        constexpr int a = ConstExp(1);
    }

    /**
     * 通过递归的常量表达式，可以实现类似于模板元编程的效果
     */
    constexpr int Fibonacci(int n) {
        return (n == 1) ? 1 : ((n == 2) ? 1 : Fibonacci(n - 1) + Fibonacci(n - 2));
    }

    TEST(CPP_11_FEATURE, CHAPTER_6_CONSTEXPR_LOOP) {
        int fib[] = {
                Fibonacci(11),
                Fibonacci(12),
                Fibonacci(13),
                Fibonacci(14),
                Fibonacci(15),
                Fibonacci(16),
        };
        for (int i : fib) {
            cout << i << endl;
        }
    }

    /**
     * 不过实际上constexpr并不一定会在编译期做运算，
     * 所以实际上模板元编程还是更强一些
     */

    /**
     * 变长模板
     * 以前已经有了变长宏和变长函数参数
     */

    /**
     * 变长函数参数
     * 实际上变长参数的原理就是指针移动，
     * vastart拿到句柄
     * vaarg移动指针并且拿到参数
     * 函数本身是无法拿到任何参数数量以及类型的
     */
    double SumOfFloat(int count, ...) {
        va_list ap;
        double sum = 0;
        va_start(ap, count);
        for (int i = 0; i < count; i++) {
            sum += va_arg(ap, double);
        }
        va_end(ap);
        return sum;
    }

    /**
     * C++引入了更加现代化的变长参数的实现方式
     * 例如tuple可以任意模板长度
     * std::make_tuple(9.8, 'g', "asdasd")
     */
    template<int ... Elements>
    class TypeA {
    };

    TypeA<1, 2, 3> ntvt;

    template<typename T1, typename T2>
    class TypeB {
    };

    template<typename ... A>
    class TypeC : private TypeB<A...> {
    };

    /**
     * 变长参数递归，任意参数都可接受
     * 这让我想起了prolog里面的写法……
     */
    template<typename ...Elements>
    class tuple;// 定义变长模板

    //偏特化定义
    template<typename Head, typename... Tail>
    class tuple<Head, Tail...> : private tuple<Tail...> {
        Head head;
    };

    template<>
    class tuple<> {
    };

    /**
     * 非类型的写法
     * 实际上也属于模板元编程的范畴了
     */
    template<long... Nums>
    struct Multiply;

    template<long Head, long... Tail>
    struct Multiply<Head, Tail...> {
        static const long val = Head * Multiply<Tail...>::val;
    };
    template<>
    struct Multiply<> {
        static const long val = 1;
    };

    TEST(CPP_11_FEATURE, CHAPTER_6_VARIABLE_LENGTH_TEMPLATE) {
        cout << Multiply<1, 2, 3, 4, 5>::val << endl;
    }

    /**
     * 变长模板进阶，有以下地方可以进行参数包展开：
     * 表达式
     * 初始化表达式
     * 基类描述列表
     * 类成员初始化列表
     * 模板参数列表
     * 通用属性列表
     * lambda函数捕捉列表
     */

    template<typename... T>
    void DummyWrapper(T... t) {}

    template<typename T>
    T pr(T t) {
        cout << t << endl;
        return t;
    }

    template<typename... A>
    void VTPrint(A... a) {
        DummyWrapper(pr(a)...);//解包为pr(1), pr(",")...
    }

    TEST(CPP_11_FEATURE, CHAPTER_6_FUNCTION_VARIABLE_ARG) {
        VTPrint(1, "asdas", 1.2);
    }

    /**
     * 'sizeof...' 也是一个新操作
     * 计算包中的参数个数
     */

    template<class... A>
    void PrintPackSize(A... a) {
        int size = sizeof...(A);
        cout << size << endl;
    }

    TEST(CPP_11_FEATURE, CHAPTER_6_SIZE_OF_PACK) {
        PrintPackSize(1, 2, "asd", 1.22);
    }

    /**
     * 模板中的变长模板是无法做递归偏特化和定义边界条件的特化声明的
     */

    /**
     * 变长模板和完美转发结合使用
     */

    struct A{
        A(){}
        A(const A& a){
            cout << "Copy Constructed " << __func__ << endl;
        }
        A(A&& a){
            cout << "Move Constructed " << __func__ << endl;
        }
    };

    struct B{
        B(){}
        B(const B& b){
            cout << "Copy Constructed " << __func__ << endl;
        }
        B(B&& b){
            cout << "Move Constructed " << __func__ << endl;
        }
    };

    /**
     * 变长模板的定义
     */
    template<typename... T>
    struct MultiTypes;

    template<typename T1, typename... T>
    struct MultiTypes<T1, T...> : public MultiTypes<T...> {
        T1 t1;
        MultiTypes<T1, T...>(T1 a, T... b) :
                t1(a), MultiTypes<T...>(b...){
            cout << "MultiTypes<T1, T...>(T1 a, T... b)" << endl;
        }
    };
    template <>
    struct MultiTypes<>{
        MultiTypes<>(){
            cout << "MultiTypes<>()" << endl;
        }
    };

    /**
     * 完美转发的变长模板
     * @tparam VariadicType
     * @tparam Args
     * @param args
     * @return
     */
    template <template <typename ...> class VariadicType, typename... Args>
    VariadicType<Args...> Build(Args&& ... args){
        return VariadicType<Args...>(std::forward<Args>(args)...);
    }

    /**
     * 我们传递两个参数给Build作为变长函数包，
     * forward之后没有拷贝消耗，递归构造MultTypes
     */
    TEST(CPP_11_FEATURE, CHAPTER_6_VARIABLE_ARGS_AND_FORWARD){
        cout << endl;
        A a;
        B b;
        Build<MultiTypes>(a, b);
    }

    /**
     * 所以在编写库的时候如果有变长模板的话不妨用递归试试
     */

    /**
     * 原子类型与原子操作
     * Todo Windows上并不能用
     */
    #ifndef WIN32
    //Todo
    #endif



}

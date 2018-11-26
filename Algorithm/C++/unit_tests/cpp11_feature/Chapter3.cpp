//
// Created by 35207 on 2018/11/23 0023.
//

#include "gtest/gtest.h"
#include <iostream>
#include <utility>
#include <map>
#include <initializer_list>

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

      /**
       * 完美转发是指在函数模板中，完全依照模板的参数的类型，将参数传递给函数模板中调用的另外一个函数
       *
       * template<typename T>
       * void IamForwarding(T t) { IrunCodeActually(t); }
       *
       * 上述代码使用最基本的类型转发，但是会导致临时对象拷贝，所以其实并不完美
       *
       */

      /**
       *
       * //目标函数需要接受非常量的左值引用
       * void IrunCodeActually(int& t){}
       *
       * template <typename T>
       * //常量左值引用，可以接受所有的值
       * void IamForwarding(const T& t)
       * {
       *    //这句代码无法编译通过因为目标函数无法接受常量左值引用
       *    IrunCodeActually(t);
       * }
       *
       */


      TEST(CPP_11_FEATURE, CHAPTER_3_FORWARDING){

          typedef const int T;
          typedef T& TR;
          //在C++98中无法编译通过
          TR& v = 1;

        /**
         * 引用折叠
         * 类型定义   声明的类型   实际类型
         * T&        TR          A&
         * T&        TR&         A&
         * T&        TR&&        A&
         * T&&       TR          TR&&
         * T&&       TR&         TR&
         * T&&       TR&&        TR&&
         *
         * 只要出现了左值引用例如T&或者TR&最终就会推导为左值引用
         */

      }

      /**
       * 通过引用折叠以及forward可以帮助我们完美转发所有类型。
       * 无需产生任何的临时构造函数
       * @param t
       */
    void IrunCodeActually(int && t){}
    void IrunCodeActually(int& t){}
    void IrunCodeActually(const int && t){}
    void IrunCodeActually(const int & t){}


    template <typename T>
    void IamForwarding(T && t)
    {
        /**
         * 实际上forawrd<T>与static_cast<T&&>是等价的，通过折叠可以
         * 通过引用折叠即可帮助我们将所有类型变为右值引用
         */

        IrunCodeActually(forward<T>(t));
    }

    TEST(CPP_11_FEATURE, CHAPTER3_PERFECT_FORWARD){
        int a;
        int b;
        const int c = 1;
        const int d = 0;

        IamForwarding(a);//非常量左值
        IamForwarding(move(b));//非常量右值
        IamForwarding(c);//常量左值
        IamForwarding(move(d));//常量右值
    }

    void RunCode(double && m){};
    void RunHome(double && h){};
    void RunComp(double && c){};

    /**
     * 完美转发可以用于包装函数，有点像函数式编程的概念
     */
     template<typename T, typename U>
     void PerfectForwardTest(T && t, U& func){
         cout << t << "\tforwarded..." << endl;
         func(forward<T>(t));
     }



     TEST(CPP_11_FEATURE, TEST_PERFECT_FORWARD){
         PerfectForwardTest(1.5, RunCode);
         PerfectForwardTest(3, RunHome);
         PerfectForwardTest(9, RunComp);
     }

     /**
      * C++11中make_pair以及make_unique都通过完美转发进行实现，
      * 无论性能还是代码简化上都完美
      */

     /**
      * 显示转换操作符 explicit
      * 以前只能在构造函数里面用，现在可以推广到转换操作符
      * C++中的隐式转换非常容易出现问题
      */

     class ConvertTo{};
     class Convertable{
     public:
         explicit operator ConvertTo() const { return ConvertTo(); }
     };
    void Func(ConvertTo ct){}
    /**
     * 测试隐式转换限制
     */
    TEST(CPP_11_FEATURE, CHAPTER_3_EXPLICIT){
        Convertable c;
        ConvertTo ct(c);
        //下面这句话无法通过编译因为隐式变换
        //ConvertTo ct2 = c;
        ConvertTo ct3 = static_cast<ConvertTo>(c);
        //下面这句话无法通过编译因为隐式变换
        //Func(c);
    }

    /**
     * 初始化列表
     * 无论是数组还是std库都已经支持的花括号初始化
     */
     TEST(CPP_11_FEATURE, CHAPTER_3_INITIALIZER_LIST){
         int arr1[5] = {0};
         int arr2[] = {1, 2, 3, 4};
         int arr3[] {1, 3, 5};
         vector<int> vec1 {1, 3, 5};
         map<int, float> map1 {{1, 2}, {1, 3}};
     }

     /**
      * C++11的赋值方式
      *
      * 等号加赋值表达式
      * int a = 3 + 4;
      *
      * 等号加花括号
      * int a = {3 + 4};
      *
      * 圆括号
      * int a(3+4);
      *
      * 花括号的初始化列表
      * int a {3 + 4};
      */

     /**
      * 我们可以自定义自己的初始化列表的构造
      * 需要包含头文件：
      * #include <initializer_list>
      */

     /**
      * 类中使用初始化列表
      */
     class TestInit{
     public:
         TestInit(initializer_list<pair<string, int>> l){
             auto i = l.begin();
             for(;i != l.end(); ++ i){
                 data.push_back(*i);
             }
         }

     private:
         vector<pair<string, int>> data;
     };

     /**
      * 函数中使用初始化列表
      * @param v
      */
     void InitListFunc(initializer_list<int> v){
         for(auto i = v.begin(); i != v.end(); i ++){
             cout << *i << endl;
         }
     }

     TEST(CPP_11_FEATURE, CHAPTER_3_TEST_INITIALIZER_LIST){
         TestInit init({
             {"hoho", 1},
             {"asd", 2}
         });
         InitListFunc({1, 2, 3});
     }

     /**
      * 我们也可以使用[]操作符
      */
      class TestInitInOperator{
          TestInitInOperator &operator [] (initializer_list<int> l){

          }
          TestInitInOperator &operator =(int v){

          }
      };

      /**
       * 返回初始化列表的情况
       */
    /**
     * 这样会造成临时变量
     *
     * vector<int> ReturnInitList(){
     * return {1, 2};
     * }
     */

    /**
     * 正确做法，const是因为等同于返回字面量，否则会报错
     * @return
     */
    const vector<int>& ReturnInitList(){

        return {1, 2};
    }

    /**
     * 花括号的初始化方式还可以防止类型变窄
     * 其实就是丢精度，列表初始化是唯一一种可以防止类型收窄的初始化方式
     */
    TEST(CPP_11_FEATURE, CHAPTER_3_NARROWING){
        const int x = 1024;
        const int y = 10;

        char a =  x;
        //以下语句会编译报错，因为类型变窄（丢精度）
        //char c = {x};
        //unsigned char e = {-1};
    }

    /**
     * POD类型(Plain Old Data)
     * 表示一个类型的属性
     * Old体现在与C语言的兼容性，可以使用memcpy以及memset进行初始化等
     * 具体的C++将POD分为两个概念的合集：
     * 平凡的(trivial)和标准布局(standard layout)
     */

    /**
     * 什么是平凡的？
     * * 拥有平凡的构造函数以及析构函数（默认构造析构函数）
     * * 拥有平凡的拷贝构造以及移动构造函数（默认的）
     * * 拥有平凡的拷贝赋值运算符以及移动赋值运算符（默认的）
     * * 不包含虚函数以及虚基类
     */
     TEST(CPP_11_FEATURE, CHAPTER_3_TRIVIAL){
         //判断是否为平凡的
         cout << std::is_trivial<int>::value << endl;
     }

     /**
      * 什么是标准布局？
      * * 所有的费静态成员有相同的访问权限(public, priveate, protected)
      * * 在类活结构体继承时满足以下两种情况之一：
      *     1.派生类中有非静态成员，且只有一个仅包含静态成员的基类
      *     2.基类有非静态成员，而派生类没有非静态成员
      * * 类中的第一个非静态成员的类型与其基类不同
      * * 没有虚函数和虚基类
      * * 所有非静态数据成员都符合标准布局类型，其基类也符合标准布局，
      *   这是一个递归的定义
      */

    TEST(CPP_11_FEATURE, CHAPTER_3_IS_POD){
        //判断是否为平凡的
        cout << std::is_pod<int>::value << endl;
    }

    /**
     * 那么POD有啥好处呢
     * * 字节赋值可以安全地使用memset和memcpy对POD类型进行初始化和拷贝等操作
     * * 提供对C内存布局兼容。C++程序可以与C函数进行交互因为POD类型的数据在C与
     *   C++直接操作总是安全的
     * * 保证了静态初始化的安全有效，静态初始化很多时候能够提高程序的性能，
     *   而POD类型的对象初始化往往更加简单（比如放入.bbs段）在初始化中直接被赋值0
     */

    /**
     * 非受限联合体
     * 在C++98中并不是所有成员都能成为联合体数据成员
     * 这导致联合体和C的兼容性形同虚设
     * //Todo
     */

    /**
     * 用户自定义字面量
     * 比如我们需要表示颜色的时候我们希望有一个自己的字面量来简化初始化
     */
     struct RGBA{
         uint8_t r;
         uint8_t g;
         uint8_t b;
         uint8_t a;
         RGBA(uint8_t R, uint8_t G, uint8_t B, uint8_t A=0) :
            r(R), g(G), b(B), a(A) {}
     };

     /**
      * 定义自定义字面量
      * @param col
      * @param n
      * @return
      */
     RGBA operator "" _COLOR(const char* col, size_t n){
         const char* p = col;
         const char* end = col + n;
         const char *r, *g, *b, *a;
         r = g = b = a = nullptr;
         for(;p != end; ++p){
             if(*p == 'r'){
                 r = p;
             }else if(*p == 'g'){
                 g = p;
             }else if(*p == 'b'){
                 b = p;
             }else if(*p == 'a'){
                 a = p;
             }
         }
         if((r == nullptr) || (g == nullptr) || (b == nullptr))
             throw ;
         else if(a == nullptr){
             return RGBA(atoi(r+1), atoi(g+1), atoi(b+1));
         }else{
             return RGBA(atoi(r+1), atoi(g+1), atoi(b+1), atoi(a+1));
         }
     }

     std::ostream & operator << (std::ostream &out, RGBA& col){
         return out << "r: " << (int)col.r
            << ", g: " << (int)col.g
            << ", b: " << (int)col.b
            << ", a: " << (int)col.a << endl;
     }

     void blend(RGBA&& col1, RGBA&& col2){
         cout << "blend " << endl << col1 << col2 << endl;
     }

     //使用自定义字面量
     TEST(CPP_11_FEATURE, CHAPTER_3_LITERAL_OPERATOR){
         blend("r255 g240 b155"_COLOR, "r15 g255 b10 a7"_COLOR);
     }

     struct Watt{ unsigned int v; };
     Watt operator "" _watt(unsigned long long v){
         return { (unsigned int)v };
     }

     TEST(CP_11_FEATURE, CHAPTER_3_LITERAL_OPERATOR_WITH_NUM){
         Watt cap = 1024_watt;
     }

     /**
      * 不过事实上其是有一定限制的
      * * 如果字面量为整数型，字面量操作符函数止咳接受unsigned long long 或者 const char*
      *   如果unsigned longlong无法容纳字面量时，自动将字面量转换为\0结尾的字符串，并以char*
      *   版本进行处理
      * * 如果字面量为浮点数，则字面量操作符只可接受long double或者 const char*为参数。
      *   const char*版本调度规则同整型
      * * 如果字面量为字符串，则字面量操作符只可接受 const char*，size_t为参数
      * * 如果字面量为字符，则字面量操作符只可接受一个char为参数
      *
      * 几点需要注意的：
      * * 在声明时 operator "" 与后面用户自定义后缀必须有空格
      * * 后缀建议以下划线开始，不宜使用非下划线后缀的用户自定义字符串常量，否则会有编译警告，
      *   主要就是避免混乱
      */

     /**
      * 内联名字空间
      * //Todo
      *
      */

     /**
      * 模板的别名
      * 除了typedef，using在C++11中是更好的选择
      */
     TEST(CPP_11_FREATURE, CHAPTER_3_USING_NAME){
         using uint = unsigned int;
         typedef unsigned int UINT;
         cout << is_same<uint, UINT>::value << endl;
     }

     /**
      * using 可以针对模板进行别名更加灵活了
      */
     template<typename T> using MapString = std::map<T, char*>;
     TEST(CPP_11_FREATURE, CHAPTER_3_TEMPLATE_USING_NAME){
        MapString<int> numberedString;
     }

     /**
      * 一般化的SFINEA规则
      * 模板在推导时无法进行匹配的时候不会产生编译错误
      * //Todo
      */


}
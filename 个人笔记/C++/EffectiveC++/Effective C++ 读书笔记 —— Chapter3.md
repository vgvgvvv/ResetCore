# Effective C++ 读书笔记 —— Chapter3
#读书笔记/C++/EffectiveC++

### 条款13：以对象管理资源

auto_ptr和shared_ptr都可以自动调用delete从而释放资源。
1. auto_ptr无法重复指向资源，否则当被其中一个智能指针销毁之后另外的指针就访问到的都是null了
2. shared_ptr则是需要使用引用计数的方法来进行资源管理的，但是它并没有办法对循环引用进行甄别，但是其copy操作比auto_ptr好多了。

如果需要使用到数组，则考虑使用STL或者Boost中的容器。智能指针并不支持数组等。

### 条款14：小心对象的Copy行为

1. 拷贝资源的时候需要将底层的资源一并拷贝（深度赋值）
2. 普遍的拷贝行为有：
防止拷贝行为、引用计数法

类似于auto_ptr的转移资源控制权的方式其实是非常少见的。

### 条款15：资源管理类中提供对原始资源的访问

因为大多API还是需要直接访问内部资源的所以需要开放接口来提供对原始资源的访问。
普遍有两种方法：get()函数或者是隐式转换。
get函数虽然麻烦但是比较准确，隐式转换虽然比较易用，但是容易被误用。

### 条款16：成对使用 new 和 delete 需要使用相同形式

很简单：
使用 **new[]** 就使用 **delete[]**
使用 **new** 就使用 **delete**

### 条款17：独立语句将已经申请的对象放入智能指针

`processWidget(std::tr1::shared_ptr<Widget>(new Widget), priority())`

注意在此处，如果priority发生了异常有可能会中断new Widget放入智能指针的过程，从而产生内存泄漏——因为调用顺序是不确定的。

所以拟用另一种方式进行调用：
```
std::tr1::shared_ptr<Widget> pw(new Widget)
processWidget(pw, priority())
```

在上述代码中不可能出现内存泄漏的情况


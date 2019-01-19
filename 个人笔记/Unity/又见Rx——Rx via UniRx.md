# 又见Rx——Rx via UniRx
#日常学习/源码阅读 #日常学习/Unity

## 缘起
在最早我大三的时候，我就看见了Rx这么一个函数响应式编程的东西，并且激发了浓厚的兴趣，不过那时候的源码阅读水平也不足，随便翻了一翻UniRx的源码就草草了事。
自己写的基于池的事件系统其实是对自己的事件系统的扩展，对Rx的仿照，但是事实上一点也不好用，是我对Rx本身原理还了解得非常浅，以至于非常难以扩展。
[基于池的事件系统扩展 | Unity之路](http://www.resetoter.cn/?p=56)
就在最近，我又重新打开了UniRx 的源码，就在现在，无论是JavaRx还是JsRx都受到了前端的追捧，一度成为被瞩目的技术。
不过在游戏开发当中，Rx就像守望先锋那种数据与行为完全分离的ECS框架一样，叫好不叫座（能这么用吗）实际工程开发当中使用到Rx的场景并不多（虽然uFrame中嵌入了UniRx），大多数人还是停留在“卧槽牛逼！还能这么搞呐！”这样的阶段，但是我觉得总有一天，这个技术能在游戏开发领域也大放异彩（Maybe）。

## 为什么要Rx
我们先用一个简单的例子，来引出Rx的必要性：

例如：我们的功能现在在使用Linq（打个比方）访问一个数据库，访问的数据就直接显示出来，这个时候数据库突然出现了问题，我们无法拿到下一个数据，这个时候我们的程序就完全卡住了！

那这个时候我们就想到了，使用异步的方式来做，所以我们现在大多数的网络数据访问、加载等等操作都是使用了异步的操作来进行的——为了不让我们的程序卡顿。

但是我们都知道，异步编程是非常复杂的，我们需要考虑到各种情况，特别是我们需要对这个一步操作进行处理的时候非常地麻烦，我们会写大量的重复代码，写出来的代码难以维护以及扩展。

就举个最简单的例子吧，我们需要编写一个双击的功能，我们会如何写？我们能够拿到最简单的接口就是onClick。

首先我们第一次click，起一个定时器，当下一次点击的时候这个定时器还没有到时间那么就进行双击响应。

看起来很简单？

那扩展到多次点击响应？要起很多定时器吗？
然后扩展到事件本身？多次同一个事件触发之后才进行响应？

再考虑到效率本身，是否复杂度就成倍扩展了？

**Rx本身就是为了解决这些问题而存在的。**

顺便，在UniRx中的双击是这么实现的：
```
//创建一个点击事件流
var clickStream = Observable.EveryUpdate()
                .Where(_ => Input.GetMouseButtonDown(0));

//将点击事件流中的事件以250毫秒缓存起来，
//如果缓存池的数量大于等于2就触发事件
clickStream.Buffer(clickStream.Throttle(TimeSpan.FromMilliseconds(250)))
.Where(xs => xs.Count >= 2)
.Subscribe(xs => Debug.Log("DoubleClick Detected! Count:" + xs.Count));
```
写法是不是和Linq很像？

## 什么是Rx
Rx是Reactive Extension的简单写法，翻译过来就是响应扩展。

不过在讲Rx之前我们要先讲一讲Linq。
我们都知道Linq，人人都爱Linq，因为Linq通过函数式编程的形式（甚至可以用类似Query查询的语法糖来将我们想要的数据拿到手）
但是在上一节中我们已经知道，Linq会被阻塞（数据库的话可能是Linq to SQL），导致程序卡死。那如何解决这个问题？

二元性，这是Rx与Linq很关键的联系。Linq是通过从数据源**拉**数据来完成读取的，而Rx则是告诉数据源，“有数据了就告诉我哦~”，等待数据源自己**推**数据来读取的，说得再笼统一点，就是观察者模式。

我们看看迭代器的接口：
```
public interface IEnumerator
{
   bool MoveNext();
   object Current { get; }
   void Reset();//这个接口函数我们会忽略掉
}
```

我们再来看看IObserver的接口
```
public interface IObserver<T>
{
	void OnCompleted();
	void OnError(Exception error);
 	void OnNext(T value);
}
```

这两个接口看似毫无联系，但是却有着密不可分的联系。

### Current -> void OnNext(T value)
Current是用于获取当前数据的，直接从远端拉，然后返回的一个数据。
而OnNext则是远端推一个数据过来的时候进行的相应的处理。

### MoveNext() ->OnCompleted()与OnError()
MoveNext是用于位移到下一个数据的，有可能有两种结果，有下一个或者没下一个。
如果没有下一个就结束遍历，对应到Rx则是OnComplete()

事实上还有第三种结果，抛出异常。
对应了Rx中的OnError。

仔细思考一下，这两个过程是不是互相对应？

如果是的话，那已经离答案非常接近了。
![](%E5%8F%88%E8%A7%81Rx%E2%80%94%E2%80%94Rx%20via%20UniRx/4B9C9C9F-12EA-46E8-85DB-5A0AF7982B49.png)
以下两份代码的运行结果是完全一致的，唯一的区别就是，Rx是非阻塞的，迭代器是阻塞的。

## Rx的源码阅读路线
微软官方在描述Rx的时候也有说道，Rx有三个部分组成：

* Observable
* Linq
* Scheduler

所以我们接下来也会从这三个方面入手。
刚开始阅读UniRx的时候会发现，进入了IObservable之后里面全是接口！实现到底在什么地方！我刚开始阅读的时候也是这种感觉，但是后来才发现，这就是实现类似于Linq方式编程的关键，具体是什么我们在后面会讲到。

如果觉得UniRx还是过于复杂的话，推荐去阅读RxLua试试，会好理解很多，当然功能也没有UniRx强大，只是能更快看到本质的原理。
[RxLua的Github地址](https://github.com/bjornbytes/RxLua)

在看该文章的时候我希望读者也能将UniRx下到自己的工程当中一边看一遍理解。
[Unity商店页面](https://assetstore.unity.com/packages/tools/unirx-reactive-extensions-for-unity-17276) 

我们从最简单的代码开始：
```
Observable
.Range(1, 10)	//创建1~10的数据源
.Where((x) => x % 2 == 0)//筛选其中能够被2整除的部分
//监听数据并进行处理
.Subscribe((arg) =>
{
	Debug.Log(arg);
}); 
```

大家或许会觉得这段代码过于简单，但是相信我，学会了简单的，复杂的代码也不过是举一反三而已。

## Observable观察者模式
事实上并不存在一个实际的，名字叫做Observable或者Observer的类，存在的只有IObservable以IObserver两个接口，一切实现了这两个接口的东西都能被用上。不过恰恰是因为这个原因，导致代码读起来着不了力，因为我们太想去看基类的实现了（继承的思想），但是不存在基类。
我们后面会讲到，Rx的主要思想是包装模式。

下面是这两个接口：
```
public interface IObservable<T>
{
   IDisposable Subscribe(IObserver<T> observer);
}

public interface IObserver<T>
{
	void OnCompleted();
	void OnError(Exception error);
  void OnNext(T value);
}
```

需要引起注意的一点是，Subscribe中的返回值是IDisposable。
为什么是IDisposable？**因为观察者模式中需要取消订阅**，当我们知道数据源不需要再向我们推送消息的时候我们需要主动销毁事件，就例如按钮销毁之后我们要销毁按钮点击事件一样。

我们继续看我们刚刚提到过我们要看的简单源码
首先通过Range来创建一个数据源（Observable）

实际上UniRx创建了一个名叫RangeObservable类的实例，该类继承自OperatorObservableBase<int>，我们现在先不管这个基类，因为其中主要是对多线程的处理，我们现在只需要知道它实现了IObservable即可。

Range中有一个关键的函数：**SubscribeCore**，告诉我们该Observable以什么方式被不同的监听者监听。我们在这里可以直接把它当做是IObservable接口中的Subscribe函数。

```
protected override IDisposable SubscribeCore(IObserver<int> observer, IDisposable cancel)
{
    observer = new Range(observer, cancel);

    if (scheduler == Scheduler.Immediate)
    {
        for (int i = 0; i < count; i++)
        {
            int v = start + i;
            observer.OnNext(v);
        }
        observer.OnCompleted();

        return Disposable.Empty;
    }
    else
    {
        var i = 0;
        return scheduler.Schedule((Action self) =>
        {
            if (i < count)
            {
                int v = start + i;
                observer.OnNext(v);
                i++;
                self();
            }
            else
            {
                observer.OnCompleted();
            }
        });
    }
}
```

剔除调度的成分，我们直接将其作为Immediate模式的调度器
也就是

```
for (int i = 0; i < count; i++)
{
    int v = start + i;
    observer.OnNext(v);
}
observer.OnCompleted();

return Disposable.Empty;
```

Observable直接会向Observer发送OnNext()以及OnComplete的结果。
当然读过源码都明白这个Observer是经过包装的（RangeObserver的内部类Range），当发生错误，会直接调用Observer的OnError


事实上，这就是一个简单的观察者模式。

举一反三，我们会发现Observable的类中大多数的方法都是通过这个方式来创建经过包装的数据源并且将传入的最基础的Observer包装成相应的功能更复杂的Observer。

### 事件的卸载
在Rx中IDisposable占到了非常重要的一个角色，那就是卸载事件。与迭代器不同，没下一个就完事儿了，观察者模式永远也不知道有没有写一个，所以主动地卸载事件是非常重要的一个事情。我在之前讲到行为队列的时候也有讲到事件泄漏的情形（[DSL在项目中的应用：用DSL高效组织游戏情节](https://zhuanlan.zhihu.com/p/34974072)）。

以至于，Rx的源码当中存在大量关于Dispose的代码。

![](%E5%8F%88%E8%A7%81Rx%E2%80%94%E2%80%94Rx%20via%20UniRx/7AF8319A-96A4-4938-BF88-4835CBF26D87.png)


在调用SubscribeCore中需要传入一个ICancelable，并且需要返回一个IDisopsable。

SubscribeCore的函数原型：
`protected override IDisposable SubscribeCore(IObserver<T> observer, IDisposable cancel)`

ICancelable的接口：
```
public interface ICancelable : IDisposable
{
    bool IsDisposed { get; }
}
```

当然，并不是所有东西都需要实现，所以Rx通常会返回一个代表不需要Dispose的常量来代表不需要销毁。


## Linq操作
事实上，我们刚刚已经接触了一部分包装模式，只不过我们的数据源是直接创建出来的，那么Linq操作又是如何实现的呢？
类似于.Where、.Amb等等操作是如何连起来让整个数据流通过短短的几个函数就可以被我们自由操作？

答案还是包装模式。

我们进入我们找到Where的实现，实际上是创建了一个名叫WhereObservable类的实例，我们到里面去看，与Range的实现大同小异。
它的SubscribeCore非常简单：
```
protected override IDisposable SubscribeCore(IObserver<T> observer, IDisposable cancel)
{
    if (predicate != null)
    {
        return source.Subscribe(new Where(this, observer, cancel));
    }
    else
    {
        return source.Subscribe(new Where_(this, observer, cancel));
    }
}
```
只是对Observer进行简单包装之后直接将其监听到传进来的数据源中。

所以我们只需要看Where这个Observer即可：

```
class Where : OperatorObserverBase<T, T>
 {
     readonly WhereObservable<T> parent;

     public Where(WhereObservable<T> parent, IObserver<T> observer, IDisposable cancel)
         : base(observer, cancel)
     {
         this.parent = parent;
     }

     public override void OnNext(T value)
     {
         var isPassed = false;
         try
         {
             isPassed = parent.predicate(value);
         }
         catch (Exception ex)
         {
             try { observer.OnError(ex); } finally { Dispose(); }
             return;
         }

         if (isPassed)
         {
             observer.OnNext(value);
         }
     }
 }
```

我们看到它的代码非常简单，OnNext的时候判断一下条件，如果条件通过则调用其包装好的Observer，否则就直接跳过，出错就OnError。

举一反三，我们可以通过这种方法来做其他各类的操作符，类似于Select、Amb等等等，方式完全一样！只不过是包装了传入的Observer而已！

## Schedule调度器
事实上，我们大多数的时候都不会注意到调度器的存在，因为它大多数情况下以默认值的形式存在，当我们想要更加灵活地使用Rx的时候也许就会用到它。

调度器的接口如下：
```
public interface IScheduler
{
    DateTimeOffset Now { get; }

    // Interface is changed from official Rx for avoid iOS AOT problem (state is dangerous).

    IDisposable Schedule(Action action);

    IDisposable Schedule(TimeSpan dueTime, Action action);
}
```

其中Now代表的是当前时间，下面两个方法一个是对传入的方法进行调度，而另一个是传入时间对行为进行调度。

查看引用之后我们会发现很多与时间相关的Observer与Observable都会采用调度器来进行实现，例如Range可以使用自定义的调度器来创建，我们可以隔几秒钟之后将一个值出队，其默认调度器是ImmediateScheduler也就是直接调用。

代码如下
```
//直接进行调度
if (scheduler == Scheduler.Immediate)
{
    for (int i = 0; i < count; i++)
    {
        int v = start + i;
        observer.OnNext(v);
    }
    observer.OnCompleted();

    return Disposable.Empty;
}
//其他的调度方式，传入的Action类似于行为队列的实现
else
{
    var i = 0;
    return scheduler.Schedule((Action self) =>
    {
        if (i < count)
        {
            int v = start + i;
            observer.OnNext(v);
            i++;
            self();
        }
        else
        {
            observer.OnCompleted();
        }
    });
}
```

一些操作符也用到了调度器，例如Timer、Delay等。

以下为Delay中的代码片段：
```
public override void OnNext(T value)
{
    var next = parent.scheduler.Now.Add(delay);
    var shouldRun = false;

    lock (gate)
    {
        queue.Enqueue(new Timestamped<T>(value, next));

        shouldRun = ready && !active;
        active = true;
    }

    if (shouldRun)
    {
        cancelable.Disposable = parent.scheduler.Schedule(delay, DrainQueue);
    }
}
```

当有新值到达的时候先放入队列，通过调度器来调用出队函数以达到Delay的目的。

调度器实际上也只是将行为放入队列，然后在Thread中进行wait，并没有使用协程，毕竟UniRx大部分代码是从Rx.Net改过来的吧。

## 对接Unity功能
讲到这里，事实上或许连Rx.Net与UniRx的区别都没有提到，但是最核心的内容已经讲完了，UniRx中提供了大量的方法来帮助开发者在Unity中使用Rx，实际上我们只需要将Unity中的各个回调包装成事件流便可以使用Rx了。

类似我们刚刚提及的双击功能：
```
//创建一个点击事件流
var clickStream = Observable.EveryUpdate()
                .Where(_ => Input.GetMouseButtonDown(0));
```

每一次Update被看做是一次事件，从中选出有点击事件的帧，即可获得点击事件流。

接下来的使用方法便和其他任何一种平台的Rx都一样了。
这部分的源码全部都在UnityEngineBridge文件夹下，都非常好理解，有需要的同学也可以扩展出自己的事件流，非常容易。


## 未提及的部分
既然我们已经知道了Observable、Linq以及Scheduler是怎么一回事，事实上我们自己也能去实现一个简单的Rx了，但是事实上还有一些我们没有阅读的部分，大家可以继续深入阅读。

* UniRx在多线程中的情形
* UniRx在IOS上避免JIT发生而进行的重写
* 在MVVM中使用Rx

## 推荐资料
Rx官网：[ReactiveX](http://reactivex.io/)
Channel9上关于Rx介绍的讲座（强烈推荐！）:[Reactive Extension—在线播放—优酷网，视频高清在线观看](http://v.youku.com/v_show/id_XNDcwMjQ0MTY4.html)


最后，还是一样，我觉得我讲的东西肯定会有疏漏，有大神看到的话还请多多指正了！
如果有在团队中大规模使用Rx的团队也希望能够好好请教请教如何在团队中推广使用Rx，并且有项目中的最佳实践能分享的话就更好了！





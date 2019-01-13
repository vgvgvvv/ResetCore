# Understanding Android memory usage (Google I/O '18)
#读书笔记/资料文献#
#原生开发/android/优化#

安卓上的物理内存被分成很多页，而每个页基本在4kb左右。不同的页有可能被用作不同的地方：

![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/558584BB-E96B-402B-8D68-794365E0EB7F.png)

* 已用页：正在被线程使用的
* 缓存页：被线程使用，在后台保存的，可以随时再利用的
* 空页：还没被使用的

## 为什么要降低内存
打开很多应用，之后我们会发现如下图
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/E6973542-D2E8-4EB2-8DEF-62672E089802.png)

一开始我们有很多内存，申请也不会有任何毛病，但是随着用的越来越多，FreePage用完了，这个时候我们就需要交换内存了，重复利用，但是用户可能没有察觉到这个问题。
但是现在内存越来越低，安卓就会调用low memory killer去杀死进程，然后获得内存，这并不是好事。
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/A1A26946-3B86-4A0B-88B4-D4DAE1A0AAC0.png)
接下来我们就看是什么东西决定low memory killder杀死进程。

杀死是有优先级的。
首先是Native层的，然后是系统层的，然后是持久层的，然后是前台的，然后是可察觉的，会被用户直接使用的，类似音乐之类的，在后面就是各种Service，接下来是Home，接下来就是用户上一个使用的App，最后就是各种缓存的应用
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/9610F4C1-A3F9-4C1C-B174-CF8BC3CB447E.png)
缓存的应用用户可以很快切换过去。

当设备内存低的时候会发生什么？
我们可以想象，当应用数变多的时候内存就会减少，然后就会从最后一个列表开始杀死进程。当我又要打开这个进程的时候会需要更多的时间。
 如果内存不断下降，缓存的程序会被全部杀光，
然后接下去就是Home，如果被杀掉的话，用户会看到我的壁纸怎么黑了一段时间，这是因为你的内存太低，Home里面使用的内存也被释放了。再坏的情况就会把后台的音乐关闭。更坏的情况，前台的程序也会挂掉。
依次类推，直到你的系统也重启。
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/6189CBA2-35C1-4D5A-940A-1A689267453B.png)
这就是内存下降时你系统发生的事情。
如果换成内存更低的设备则影响会更加严重：
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/DFD7FAB7-0C1E-4B31-B996-A2651A7359F6.png)
你可以预见的是，如果内存不够时对用户的影响是非常大的，这就是为啥我们关心内存。

那我们如何解决内存问题。
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/6C7386BC-B147-41E6-8D76-A62B4286105D.png)
我们已经知道系统有各种page，每个应用都会用内存，但是实际情况会更加复杂，因为不同的应用之间会共享内存。之前在垃圾回收章节中我们可以看到其中一个堆是用于共享库加载的，也就是安卓的根进程进行使用的那个。
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/BF440333-0F9D-4E3D-9A83-CA37CD9E3747.png)
我们如何算出我们具体用了多少内存，接下去会举出举个概念：
* VSS - Virtual Set Size 虚拟耗用内存（包含共享库占用的内存）
* RSS - Resident Set Size 实际使用物理内存（包含共享库占用的内存）
* PSS - Proportional Set Size 实际使用的物理内存（比例分配共享库占用的内存）
* USS - Unique Set Size 进程独自占用的物理内存（不包含共享库占用的内存）
一般来说内存占用大小有如下规律：VSS >= RSS >= PSS >= USS

我们可以用一些adb命令去查看其内存的占用：
 
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/808B3F78-E191-4815-A4DC-AB55A47F8877.png)

那么我们的应用应该用多少的内存呢？
 
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/0CEF0AFE-93C1-4463-8823-1BDF1D1327C8.png)
不同的设备会支持不同的app
当你期望往你的设备中加新功能，那对中高端机用户是好的，但是对低端机用户则是不好的，因为它们的机子可能就用不了了。

![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/E4784159-DFF0-4E81-BD59-C396CF5B3E9D.png)

用户不同的行为会造成内存的升降：
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/A821E014-631D-4216-B047-974A5CAF1B97.png)
不同的平台也会造成内存使用的不同：
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/259E90D9-440D-4A17-90C7-0D8BB3C454AD.png)
因为有不同的屏幕大小，不同平台优化，主进程的设置等等，都会影响到你的内存。
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/342C6A14-76B9-4997-8D18-CE07C70A5C17.png)
内存压力变大的时候Chrome的PSS，
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/D1AA4080-D180-4C10-A3B4-742DABD2C70E.png)
优化之后的Chrome

## 降低你应用的内存影响
首先可以去看Android Studio的Memory Profiler会给与非常多的信息关于Java Heap。我们会看到三个堆：zygote heap、image heap、app heap。前两者是在你的应用启动的时候就有的，你基本没法做什么，主要还是处理app heap。

![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/33B032D8-CFFD-43F9-A6A8-9CFDE8261F91.png)
实际上我们发现我们的java堆并没有使用这么多的内存，那么剩下的是什么东西?我们怎么办？
这关于整个安卓执行的过程。
我们可以通过-a的方式尽可能多的看到我们的内存信息：
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/704741A8-47C8-45F2-81E9-EA895630EEC1.png)
Private dirty内存就是正在使用的内存。
Private clean内存就是cache内存。
 我们看到各种参数

![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/A03CBE1C-359A-4844-B1CD-C8F8EF67ED7E.png)

![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/8C4E0772-DA0C-42F3-8958-1E2996492641.png)
我们还可以看到heap dump

![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/301BD2CD-0481-4EFC-B33E-9C92C55929E6.png)
内存申请，类似于instrament
查泄漏之类的

这些东西在AS的Profiler里面都有。
不过这些工具有各种问题：
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/2C41DED6-345A-4208-881B-BC0079C3ABED.png)
* 支持不到位
* 用户接口不友好
* 需要深度掌握平台
* 也许需要root的机器
* 显示的数字有很多噪声不太准确
* 很多内存不在你的掌控之中

降低内存时我们最需要做的两件事情就是：
* Proiler你的Java堆
* 降低你的包大小

经常有一些非Java申请的对象与Java申请联系起来：
Native层的比如数据库，比如线程又不如图像。
（实际上这个现象在Unity的C#中也是一样的，也就是Non-Managed Object，这些东西度需要我们自己手动销毁，否则会内存泄漏
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/8DFA2875-62FF-4126-A4F6-990F499485D2.png)
如果你发现代码中有这些东西记得自己去释放。

接下来就是包大小，包大小也会影响内存使用。
 
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/A0E63A98-166C-44E1-AE1A-846A239523F3.png)

包内的这些东西都会影响内存。
而且减少包大小往往比减少内存是更有效的优化手段！

## 快速回顾
 内存如何影响一个设备
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/033E1BB7-017F-441F-A170-5B4AB609401E.png)
如何计算你的应用内存用量是比较合理
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/1FD65C21-7E8A-42C7-AF92-E87227E5834B.png)

如何优化你的应用
![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/9893072B-E9C0-4336-9901-1DF9AA004CD9.png)

![](Understanding%20Android%20memory%20usage%20(Google%20IO%20'18)/7E7C1203-D679-4ADA-A5F9-8D8386AFBFBE.png)



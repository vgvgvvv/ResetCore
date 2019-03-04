# Unity图文混排——读读EmojiText顺便吐槽下
#日常学习/Unity/UI

既然我们目前已经知道如何为UGUI重置绘制，那么现在我们就来尝试一下——如何进行图文混排！
网上查找过一些资料，发现UWA中已经有文章对图文混排进行研究了，所以我也就直接拿别人的源码来看了。

而且UWA中对其说明也不是很详细，导致我感觉它好像什么都讲，只是讲了个流程而已……
而且还发现其实现并不是很完美，在Perfect Pixel模式下会被打回原形（做处理的分支压根就没做处理，不知道是不是作者偷懒……
![](Unity%E5%9B%BE%E6%96%87%E6%B7%B7%E6%8E%92%E2%80%94%E2%80%94%E8%AF%BB%E8%AF%BBEmojiText%E9%A1%BA%E4%BE%BF%E5%90%90%E6%A7%BD%E4%B8%8B/9674BFDC-6E8C-47F5-B2BE-EB3A1F9FCF9F.png)

这里就稍微详细一些来讲一讲这个图文混排吧。

[原文链接](https://blog.uwa4d.com/archives/Sparkle_UGUI.html#comment-94)

## 如何使用
其实也就只是简单地对Text里面的OnPopulateMesh进行重写而已如何对Graphic进行重新绘制我在之前的文章中已经讲过，所以对重绘过程不太了解的同学也可以看看之前写的文章。

可以直接看原文链接，也可以直接下原作者[Github的工程](https://github.com/zouchunyi/EmojiText)来看看样例。

## 源码解读
事实上，这个重写的Text需要看的就只有一个Text Component以及一个Shader。


我们可以直接拿Unity原生的Text源码和其进行比较：

EmojiText中与Text唯一不同的地方就是OnPopulateMesh。

这是原生的Text带有的PopulateMesh
![](Unity%E5%9B%BE%E6%96%87%E6%B7%B7%E6%8E%92%E2%80%94%E2%80%94%E8%AF%BB%E8%AF%BBEmojiText%E9%A1%BA%E4%BE%BF%E5%90%90%E6%A7%BD%E4%B8%8B/021BACFA-01D1-454F-9C92-EFFE5CE54996.png)

而下面则是我们要看的新Text中的PopulateMesh

![](Unity%E5%9B%BE%E6%96%87%E6%B7%B7%E6%8E%92%E2%80%94%E2%80%94%E8%AF%BB%E8%AF%BBEmojiText%E9%A1%BA%E4%BE%BF%E5%90%90%E6%A7%BD%E4%B8%8B/81071071-1A50-4E5F-BAA0-95A63BBDE048.png)

我们会发现，除了计算字体之间偏移的不同以外其他基本完全一样！

### 读取配置文件
在绘制之前，EmojiText会尝试预读表情数据，并且放入列表当中。
然后利用正则表达式查找字符串中符合条件的表情占位符，记下index放入dictionary中。
![](Unity%E5%9B%BE%E6%96%87%E6%B7%B7%E6%8E%92%E2%80%94%E2%80%94%E8%AF%BB%E8%AF%BBEmojiText%E9%A1%BA%E4%BE%BF%E5%90%90%E6%A7%BD%E4%B8%8B/DB09D0B7-F6F8-4FB9-B431-3D74348E9AF0.png)
这一部分实际上非常简单，以至于我都不想更多进行讲解。

### 生成Mesh
而剩下我们要做的就是将Mesh生成出来，除了将原生Text中的OnPopulateMesh照抄下来以外，我们还要插入我们自己的代码：
![](Unity%E5%9B%BE%E6%96%87%E6%B7%B7%E6%8E%92%E2%80%94%E2%80%94%E8%AF%BB%E8%AF%BBEmojiText%E9%A1%BA%E4%BE%BF%E5%90%90%E6%A7%BD%E4%B8%8B/923381B0-AAFE-49F6-ABEB-0B02D0395570.png)
这里依旧还是原生的代码，需要注意的是，最外层的if是对Text的Perfect Pixel进行操作，也就是说上下两个分支是打开与关闭Perfect Pixel的两种绘制方式，所以上下两个分支都需要对我们的EmojiText进行处理，但是在源码当中并没有对Perfect Pixel进行处理，导致在Perfect Pixel打开的情况下表情无法正常显示。

不过上面与下面的实现事实上差不多，但是我们平常也不怎么开Perfect Pixel所以我也就不太多在意了。总之看明白其中一条分支那就明白整个绘制过程了，想要自己改也是非常容易的。

![](Unity%E5%9B%BE%E6%96%87%E6%B7%B7%E6%8E%92%E2%80%94%E2%80%94%E8%AF%BB%E8%AF%BBEmojiText%E9%A1%BA%E4%BE%BF%E5%90%90%E6%A7%BD%E4%B8%8B/95FEC0BD-56C7-43E0-AE80-F1A24DBC78ED.png)

我们知道在上面我们已经记录了所有表情的index，我们尝试从Dictionary中进行获取。
接下来就是完全是顶点位置的计算以及拼接了，并且写入第二张uv，一会儿shader会用到……

![](Unity%E5%9B%BE%E6%96%87%E6%B7%B7%E6%8E%92%E2%80%94%E2%80%94%E8%AF%BB%E8%AF%BBEmojiText%E9%A1%BA%E4%BE%BF%E5%90%90%E6%A7%BD%E4%B8%8B/F6EC204C-9DD1-4C04-BF91-B57E142EC937.png)

还要注意的是在换行时如果表情比较大的时候对行距进行修正……
看到github上有好多对这个换行提出的issue，我也懒得细看了说实话（溜了

### 重写Shader
其实Shader涉及到的很非常简单，动画无非就是逐帧动画而已。
![](Unity%E5%9B%BE%E6%96%87%E6%B7%B7%E6%8E%92%E2%80%94%E2%80%94%E8%AF%BB%E8%AF%BBEmojiText%E9%A1%BA%E4%BE%BF%E5%90%90%E6%A7%BD%E4%B8%8B/1C4C4AD3-C1C1-4FCB-B070-18F57E3C7236.png)
首先判断一下第二张UV的xy都不为0
然后计算一下每帧的offset直接平的显示出来就ok了！

## 总结
发现我也只是简单写了个流程+找到个issue而已！（仿佛睿智




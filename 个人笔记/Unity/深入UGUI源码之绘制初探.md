# 深入UGUI源码之绘制初探
#日常学习/Unity/UI

在日常开发过程当中，类似于HUD或者是弹幕之类大量的体积小但是不断在移动的UI，在不断的重建的过程当中会产生大量的GC，导致游戏卡顿到不能玩。
今天就研究一下Unity中UGUI的绘制方法以及规则：
UGUI的源码地址（C#部分）：
[下载地址](https://bitbucket.org/Unity-Technologies/ui)

## 利用底层API进行绘制
- - - -
首先我们要搞清楚，到底UGUI是如何对UI进行绘制的。我们首先从最稀疏平常的Image开始。

### Image源码尝试阅读
我们首先盯上的是Image，我们发现首先，其基类为**MaskableGraphic**,其他的接口我们先暂时不管，并且最终我们看到其最终的基类依旧是Monobehavior。

![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/7047BE90-C80C-43E6-8894-ADD5EF9E8B25.png)

经过阅读，其实最主要的绘制部分其实不多，主要是对Graphic基类中几个成员对象的重写。

* **mainTexture属性** 主要负责提供贴图
* **material属性** 主要提供材质
* **OnPopulateMesh函数** 主要提供顶点以及UV信息（也就是Mesh信息）

首先是**mainTexture**，我们发现就是简单从sprite中取出Texture而已，如果没有sprite则提供默认的白色贴图
![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/BCFDE1DA-2F59-4CB6-B402-42FF17C71DEB.png)

然后是**material属性**，我们可以看到，也是非常简单，首先判断有没有材质，如果没有就提供默认材质。
![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/22E1DF72-9532-49B5-A608-5D1DB4B74012.png)

最后就是比较重头的**OnPopulateMesh**，这个函数的主要作用就是用于构建Mesh。
![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/158A7D96-FE89-4B28-8185-524503B331C0.png)
在这之前我们可以先看看VertexHelper这一个类的作用。

打开VertexHelper的源码，我们可以很清楚看到，其实VertexHelper只是一个临时的容器，用于存放需要渲染的Mesh信息。

值得注意地是其中所用到的ListPool，性能高度敏感的代码块，需要大量的使用对象池，这是优化代码效率减少GC的好技巧，无论是网络模块还是底层的组件重用，对象池都是必不可少的！

那这个时候我们其实自己可以想到，我们的合批需要怎么做了，在我们使用UGUI的时候其实每一个Image都是使用了PopulateMesh来进行Mesh生成之后进行合批，我们其实可以将这一步直接放在这一个函数里面，由我们自己来重写就可以了，这样我们就不需要调用**N次**PopulateMesh之后进行合并，而只需要调用**一次**自己编写的PopulateMesh函数就可以了。
![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/AF1C8EC8-D3BE-4723-AF2F-E88E59D3A6AE.png)

我们再看一看Graphic基类当中对该函数的编写：
![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/4C339D55-835A-42E2-BA09-0ECFD862B0FD.png)

上面图中我们可以看到，其仅仅是画了一个正方形的Mesh而已。
具体如何通过Vertex画Mesh其实和OpenGL中VAO EBO是一样的，就不再赘述了。

我们回过头来看Image当中的OnPopulateMesh：
![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/9EBF9456-E0D2-4936-AE5A-92AE6C243353.png)
我们会发现，当Sprite为空的时候画Image的方式就是Graphic，与我们平时的使用方式相同。
当Sprite不为空则会通过不同的sprite类型来进行绘画，实际上就是获取不同的UV，通过何种方式来将贴图绘制到Mesh上面。

我们这里就只看一下最简单的sprite绘制方法，我们会看到
![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/E8C5A596-4A25-419B-A2AD-FDEB1F854FC7.png)
其实也是画个矩形……
GetDrawingDimensions在获取Rect，而通过GetOuterUV来获得UV。

获得UV的时候主要是为了将Sprite中空白的地方进行裁切，主要是通过从Native层得到的padding来计算的，具体内部如何计算我们不得而知，但是我们拿到padding值就已经可以完全算出Sprite的实际大小了。

由于我们只是绘制Simple类型的sprite，所以对UV不需要进行比较复杂的计算，实际上在其他几种绘制方式之中，除了OuterUV还会用到InnerUV，详细算法也就得自己慢慢看了……

至此，我们已经大致明白了如何通过顶点来对UI进行绘制。是时候自己实践了。

### 自定义Graphic
我们建立一个新的类：MyGraphic
其中我们重写三个对象，也就是上面提到的：

* **mainTexture属性** 
* **material属性** 
* **OnPopulateMesh函数** 

![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/C4BC606D-6A6B-4C7B-8D15-C7AEBE7329EB.png)

因为自己很懒惰，所以直接将DrawingDimension函数拿过来用了。
获得了Rect以及UV之后我们就可以像普通的Image一样绘制出图片了，
当然我们也可以把其中一句AddTriangle删除，我们就可以发现，我们只绘制了一个三角面

![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/4F11FA18-04FF-40BF-BB6C-E17E1360AFEF.png)
![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/48B4689E-52B2-4547-827A-EEAEFEF1FB9F.png)

## UGUI底层绘制规则
- - - -
接下去，我比较好奇的是，**OnPopulateMesh到底是什么时候进行调用的**，因为我们都知道，在UI当中消耗比较大的用于都是UI的重建操作，如果我们能够搞明白UI重建规律的话那我们应该可以大大避免UI重建的情况。

Graphic当汇总的DoMeshGeneration调用了OnPopulateMesh
![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/BB78F933-CFF1-4077-8CC9-BDB360616081.png)
我们可以看到，当RectTransform的尺寸为0的时候就不再调用OnPopulateMesh了，与以前总结出来的优化方案相同。
下面则是对Mesh进行调整并且通过VertexHelper将数据实装到Mesh当中最后将Mesh赋值给CanvasRenderer进行渲染，这下能理解为何所有显示组件都要带一个CanvasRenderer了吧。

UpdateGeometry调用到了DoMeshGeneration
![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/5B1DAC54-A031-4132-8329-D395D9367DAA.png)
除了Text在字体变换的时候会进行更新以外，入口都是统一的：Graphic中的Rebuild。

Rebuild实际上是实现了ICanvasElement的接口。
![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/B2A6C9CC-C90D-49B3-B9B4-686851A839C6.png)
当顶点数据遭到修改，则立刻会进行重建，实际上我们只需要关心何时VertsDirty会被修改就可以明白到底什么时候UI会进行重建。

不过在此之前，我们先看看Rebuild在何时会被调用。

查找引用，发现是在CanvasUpdateRegistry中的PerformUpdate中进行调用，这个PerformUpdate是挂载在Canvas.willRenderCanvases这个事件上的，也就是Canvas进行渲染之前便会进行调用。

### SetVerticesDirty被调用的情况
我们回过头来看一看何处会修改SetVerticesDirty：
![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/399DC385-32D0-43F3-AF80-E861006D80E1.png)
当调用了SetVerticesDirty则会修改该值并且添加到Canvas的重建列表当中。

调用该函数的地方就不少了，查找之后总共34处调用。
![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/A9409719-0D28-47DB-9ABF-C53FD50BDE9A.png)

文字的以下属性进行变化都会进行Mesh重建，所以如果有大量需要经常变动的Text就要小心了。
![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/B269DC38-A71D-4FB6-9406-BE46FDE72426.png)

同样的图片也有属性一旦改变就会引发改变，其中我们比较常用的可能就是尺寸了，例如CD、位置修改等等，都会造成重建。

![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/76C95939-6014-48B7-BB05-979CC318F946.png)

RawImage中进行修改的时候也会产生重建

![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/A27B2279-B6E5-4B80-90E4-60F7DA825EC2.png)

当蒙版进行修改的时候也会进行重建

![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/7428BCDB-5366-4FE0-813C-53CCFE0EC6BE.png)

采用Shadow的时候也会产生重建

![](%E6%B7%B1%E5%85%A5UGUI%E6%BA%90%E7%A0%81%E4%B9%8B%E7%BB%98%E5%88%B6%E5%88%9D%E6%8E%A2/A0C5CC04-47EC-47B6-9075-73E265E4F4DE.png)

了解到何处会对Mesh重建进行调用我们就可以尽可能减少重建消耗。

## 总结
- - - -
例如弹幕或者是MMO中飘血文字都可以通过自己重写OnPopulateMesh来自己对Mesh进行合批而不需要通过Canvas对所有元素进行逐一的合批，提高效率。















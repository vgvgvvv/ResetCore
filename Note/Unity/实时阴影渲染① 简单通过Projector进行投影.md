# 实时阴影渲染①	简单通过Projector进行投影
#日常学习/Unity/技巧/阴影渲染

[原文地址](http://blog.csdn.net/aceyan0718/article/details/52279594)

虽然应该不是最好的方案，但是也要看一看对吧。

## 原理解释
添加一个光照相机，光照相机的位置即是光照位置，然后把照出来的东西的罗阔作为阴影作为Projector的贴图投射到地上。

**需要注意的是：如果边缘没有做任何处理的话则会无限延长边缘至Mesh**
![](%E5%AE%9E%E6%97%B6%E9%98%B4%E5%BD%B1%E6%B8%B2%E6%9F%93%E2%91%A0%20%E7%AE%80%E5%8D%95%E9%80%9A%E8%BF%87Projector%E8%BF%9B%E8%A1%8C%E6%8A%95%E5%BD%B1/6FBD805A-1FB3-42A2-B935-2CE0CF049279.png)
这个时候就需要一个遮罩把周围全部都遮起来。

![](%E5%AE%9E%E6%97%B6%E9%98%B4%E5%BD%B1%E6%B8%B2%E6%9F%93%E2%91%A0%20%E7%AE%80%E5%8D%95%E9%80%9A%E8%BF%87Projector%E8%BF%9B%E8%A1%8C%E6%8A%95%E5%BD%B1/DC955241-EBA2-44B1-872E-FC0F4C70322B.png)
这个相机事实上是代码进行生成的，然后将其照射到的对象作为projector的遮罩：
![](%E5%AE%9E%E6%97%B6%E9%98%B4%E5%BD%B1%E6%B8%B2%E6%9F%93%E2%91%A0%20%E7%AE%80%E5%8D%95%E9%80%9A%E8%BF%87Projector%E8%BF%9B%E8%A1%8C%E6%8A%95%E5%BD%B1/002F03F2-568A-4841-91E2-5DC34F1827F9.png)
然后就可以直接进行阴影的渲染了。
![](%E5%AE%9E%E6%97%B6%E9%98%B4%E5%BD%B1%E6%B8%B2%E6%9F%93%E2%91%A0%20%E7%AE%80%E5%8D%95%E9%80%9A%E8%BF%87Projector%E8%BF%9B%E8%A1%8C%E6%8A%95%E5%BD%B1/A96E4AB5-5061-4755-AB0B-FC4F18908E8E.png)

## 源码阅读
![](%E5%AE%9E%E6%97%B6%E9%98%B4%E5%BD%B1%E6%B8%B2%E6%9F%93%E2%91%A0%20%E7%AE%80%E5%8D%95%E9%80%9A%E8%BF%87Projector%E8%BF%9B%E8%A1%8C%E6%8A%95%E5%BD%B1/8E6F7E49-8F85-4ACC-9A91-4EA6593CB609.png)
我们先看一看Start函数，这其中主要前半段是获得主相机 以及添加光照相机。而后面则是给所有的
投射阴影的物体添加上标签，在实际项目当中我们则根据需要自己选择Mask。
需要注意的是，这里替换了Camera的渲染Shader，也就是为什么摄像机渲染出来的图片是黑白的原因。

`Camera.SetReplacementShader`
直接用固定管线写的，意思看一下就可以了。
![](%E5%AE%9E%E6%97%B6%E9%98%B4%E5%BD%B1%E6%B8%B2%E6%9F%93%E2%91%A0%20%E7%AE%80%E5%8D%95%E9%80%9A%E8%BF%87Projector%E8%BF%9B%E8%A1%8C%E6%8A%95%E5%BD%B1/88DE722A-FC24-4993-B394-E7EE5D1E7613.png)
_根据不同的RenderType添加不同的纯色_

事实上作者也不需要将这张图作为ARGB32，毕竟本来就是拿来做黑白图的……（甚至我觉得alpha通道也不需要其实……一个通道足够

实际上到了上面这一步我们已经能够正确的将阴影渲染出来了。
但是下面还有一个LateUpdate，这个函数主要在做的事情实际上是将我们光照相机的位置以及投射物的位置进行调整，保证能将场景当中所有物体都包进去。

![](%E5%AE%9E%E6%97%B6%E9%98%B4%E5%BD%B1%E6%B8%B2%E6%9F%93%E2%91%A0%20%E7%AE%80%E5%8D%95%E9%80%9A%E8%BF%87Projector%E8%BF%9B%E8%A1%8C%E6%8A%95%E5%BD%B1/A2E85A9C-9D83-4E9C-BF02-E8E4DA4FC1FA.png)

## Shader阅读
![](%E5%AE%9E%E6%97%B6%E9%98%B4%E5%BD%B1%E6%B8%B2%E6%9F%93%E2%91%A0%20%E7%AE%80%E5%8D%95%E9%80%9A%E8%BF%87Projector%E8%BF%9B%E8%A1%8C%E6%8A%95%E5%BD%B1/5776256C-1708-4F41-B9AA-6FFAB930A55C.png)
首先我们先看到顶点着色器：
顶点着色器先将顶点位置转移到观察坐标空间(常规操作

_旧版本的话则是_
`mul(UNITY_MATRIX_MVP,*)`


![](%E5%AE%9E%E6%97%B6%E9%98%B4%E5%BD%B1%E6%B8%B2%E6%9F%93%E2%91%A0%20%E7%AE%80%E5%8D%95%E9%80%9A%E8%BF%87Projector%E8%BF%9B%E8%A1%8C%E6%8A%95%E5%BD%B1/E9191D7F-4A04-42F6-98F2-32F2322132F6.png)

而unity_Projector则是将我们的顶点转换到相应的投射坐标。
有兴趣可以看看Unity的CG源码

_旧版本的话则是_
`_Projector`

接下来看一下片元着色器：
![](%E5%AE%9E%E6%97%B6%E9%98%B4%E5%BD%B1%E6%B8%B2%E6%9F%93%E2%91%A0%20%E7%AE%80%E5%8D%95%E9%80%9A%E8%BF%87Projector%E8%BF%9B%E8%A1%8C%E6%8A%95%E5%BD%B1/C950EB91-E40C-4442-8307-967607B9D10A.png)
将阴影颜色与遮罩颜色混合。
最终根据alpha值来显示最终的颜色。

## 性能
将所有东西关闭空场景占有一个drawcall
摄像机渲染轮廓则是每种Mesh一个Drawcall（动态合批）
![](%E5%AE%9E%E6%97%B6%E9%98%B4%E5%BD%B1%E6%B8%B2%E6%9F%93%E2%91%A0%20%E7%AE%80%E5%8D%95%E9%80%9A%E8%BF%87Projector%E8%BF%9B%E8%A1%8C%E6%8A%95%E5%BD%B1/3C5BACAA-B501-4FDC-8C10-1E60A6D5C9D1.png)
绘制RT本身有一个Drawcall（BiltCopy）
![](%E5%AE%9E%E6%97%B6%E9%98%B4%E5%BD%B1%E6%B8%B2%E6%9F%93%E2%91%A0%20%E7%AE%80%E5%8D%95%E9%80%9A%E8%BF%87Projector%E8%BF%9B%E8%A1%8C%E6%8A%95%E5%BD%B1/66CDF02D-0F74-4D7E-B94F-C53C9DA60888.png)

最后渲染Mesh本身占有一个Drawcall（算合批）
![](%E5%AE%9E%E6%97%B6%E9%98%B4%E5%BD%B1%E6%B8%B2%E6%9F%93%E2%91%A0%20%E7%AE%80%E5%8D%95%E9%80%9A%E8%BF%87Projector%E8%BF%9B%E8%A1%8C%E6%8A%95%E5%BD%B1/84ADC0F6-8A03-4FBA-BAE4-73B5C95E0575.png)

算阴影有N个Drawcall（包含接收所有影子的Mesh）
下图的话还要算上一个plane的平面，毕竟要渲染影子
![](%E5%AE%9E%E6%97%B6%E9%98%B4%E5%BD%B1%E6%B8%B2%E6%9F%93%E2%91%A0%20%E7%AE%80%E5%8D%95%E9%80%9A%E8%BF%87Projector%E8%BF%9B%E8%A1%8C%E6%8A%95%E5%BD%B1/0C4AAC16-99FC-4196-9F5C-A3E609254DA6.png)

所以除了接收平时就要处理的N个影子投射者以及N各影子接受者，还要增加N中Mesh在绘制时候的Drawcall以及N个接收者的DrawCall。

所以在物体非常多的情况下事实上还是有些消耗的……不过相比起需要使用深度贴图的ShadowMap来说已经好不少了。在移动平台还算是不错的解决方案了。


## 总结
总的来说这是一个还不错的实时阴影渲染方法，不过依旧有很多可以优化的地方。自动调整Projector以及摄像机位置的方法可以稍微看一下，回头我还是看一下Dynamic Shadow Projector和Fast Shadow Reciver的解决方案，相对而言都比较成熟，学习一下一定收益匪浅。
不过Dynamic Shadow Projector还没细看，稍微看了一些也是似懂非懂，还是慢慢看吧。










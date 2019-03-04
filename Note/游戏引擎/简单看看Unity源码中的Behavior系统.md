# 简单看看Unity源码中的Behavior系统
#日常学习/源码阅读/Unity


因为比较好奇Unity中的回调到底是如何调用的，所以看了一下Unity源码。
从MonoBehavior入手。

## MonoBehavior
发现其中有个变量

 `const ScriptingMethodPtr* m_Methods;`

在Update、Awake、Start的地方实际上时直接调用其中缓存下来的C#函数

```
void MonoBehaviour::Update ()
{
	CallUpdateMethod (MonoScriptCache::kUpdate);
}
```

看到里面就会发现：
```
inline void MonoBehaviour::CallMethodIfAvailable (int methodIndex)
{
	AssertIf (methodIndex < 0 || methodIndex >= MonoScriptCache::kMethodCount);
	ScriptingMethodPtr method = m_Methods[methodIndex];
	if (method == SCRIPTING_NULL)
	{
		return;
	}

	AssertIf (GetInstance() == SCRIPTING_NULL);
	AssertIf (!m_DidAwake);

	if (!IsActive ())
		return;
	
	ScriptingInvocationNoArgs invocation(method);
	invocation.objectInstanceIDContextForException = GetInstanceID();
	invocation.object = GetInstance();
	invocation.Invoke();
}
```


Update等回调函数的回调则全部使用BehaviorManager进行调用的

```
if (IsInstanceValid (instance) && m_Methods[MonoScriptCache::kUpdate])
			GetBehaviourManager().AddBehaviour (m_UpdateNode, executionOrder);
		if (IsInstanceValid (instance) && m_Methods[MonoScriptCache::kFixedUpdate])
			GetFixedBehaviourManager().AddBehaviour (m_FixedUpdateNode, executionOrder);
		if (IsInstanceValid (instance) && m_Methods[MonoScriptCache::kLateUpdate])
			GetLateBehaviourManager().AddBehaviour (m_LateUpdateNode, executionOrder);
```

主循环则是由Player.h这个文件里面的代码进行调用的。

我回头看了Renderer，虽然它是Component但是却不是Behavior，Renderer是统一由RendererManager进行调用的，通过将Renderer添加到渲染队列当中，进行渲染循环。

```
void Renderer::UpdateAllRenderersInternal()
{
	// Update the renderers from the update list:
	// - before updating each, remove from the list
	// - a renderer can add itself again, so only process the original list length
	RendererList::iterator next, listEnd = gRenderersToUpdate.end();
	for( RendererList::iterator i = gRenderersToUpdate.begin(); i != listEnd; i = next )
	{
		next = i;
		next++;
		Renderer& renderer = **i;
		renderer.m_RenderersListNode.RemoveFromList();
		renderer.UpdateRenderer();
	}
}
```


总的来说Unity的结构还是符合组件的思想的，但是硬要说ECS的话其实也不算吧，毕竟行为和数据都没有分离，GameObject只是空壳，但是里面有很多Component，这些Component可以通过不同的系统进行更新，Component应该自动注册到各个系统当中，当销毁的时候自动从各个系统中取消注册。

层次关系是这样的：
 Component 派生 Behavior 派生 MonoBehavior

GameObject被GameObjectManager储存了起来

大概知道怎么做之后明天看看能不能把自己的引擎也改成这样，Mono的嵌入已经搞定了，C#上编写指日可待了！
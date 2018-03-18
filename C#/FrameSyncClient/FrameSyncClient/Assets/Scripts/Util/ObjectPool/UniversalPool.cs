using System;
using System.Collections;
using System.Collections.Generic;
using System.Dynamic;
using UnityEngine;

namespace ResetCore.Util
{
    /// <summary>
    /// 通用对象池
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class UniversalPool<T>
    {
        private UniversalPool() { }

        Stack<T> pool = new Stack<T>();

        Func<T> factory;
        Action<T> destroyAct;
        Action<T> onGet;
        Action<T> onReturn;

        #region property

        /// <summary>
        /// 池子创建的对象总数
        /// </summary>
        public int count { get; private set; }

        /// <summary>
        /// 已激活的池子创建的对象的数量
        /// </summary>
        public int activeCount { get { return count - poolCount; } }

        /// <summary>
        /// 池子中可使用的对象数量
        /// </summary>
        public int poolCount { get { return pool.Count; } }

        #endregion property

        #region public func

        /// <summary>
        /// 创建池
        /// </summary>
        /// <param name="factory"></param>
        /// <param name="destroyAct"></param>
        /// <param name="onGet"></param>
        /// <param name="onReturn"></param>
        /// <param name="startSize"></param>
        /// <returns></returns>
        public static UniversalPool<T> Create(Func<T> factory, Action<T> destroyAct
            , Action<T> onGet = null, Action<T> onReturn = null, int startSize = 0)
        {
            var uniPool = new UniversalPool<T>();
            uniPool.factory = factory;
            uniPool.destroyAct = destroyAct;
            uniPool.onGet = onGet;
            uniPool.onReturn = onReturn;
            uniPool.Init(startSize);
            return uniPool;
        }

        /// <summary>
        /// 从池中获取
        /// </summary>
        /// <returns></returns>
        public T Get()
        {
            T res;
            if (pool.Count > 0)
            {
                res = pool.Pop();
            }
            else
            {
                res = factory();
                count++;
            }
            if (onGet != null)
                onGet(res);
            return res;

        }

        /// <summary>
        /// 归还池
        /// </summary>
        /// <param name="obj"></param>
        public void Return(T obj)
        {
            if (onReturn != null)
                onReturn(obj);
            pool.Push(obj);
        }

        /// <summary>
        /// 销毁池
        /// </summary>
        public void Destroy()
        {
            if (destroyAct == null)
            {
                pool.Clear();
                return;
            }
            for (int i = 0; i < pool.Count; i++)
            {
                destroyAct(pool.Pop());
            }
        }

        #endregion public func

        #region private func

        private void Init(int startSize)
        {
            for (int i = 0; i < startSize; i++)
            {
                var obj = factory();
                pool.Push(obj);
                if (onReturn != null)
                    onReturn(obj);
            }
        }

        #endregion private func

    }

    public static class ListPool<T>
    {
        private static readonly UniversalPool<List<T>> pool = UniversalPool<List<T>>.Create(Factory, Destroy, l=>l.Clear(), l=>l.Clear());

        /// <summary>
        /// 得到池中新的List
        /// </summary>
        /// <returns></returns>
        public static List<T> Get()
        {
            return pool.Get();
        }

        /// <summary>
        /// 返回List
        /// </summary>
        /// <param name="list"></param>
        public static void Return(List<T> list)
        {
            pool.Return(list);
        }

        private static List<T> Factory()
        {
            return new List<T>();
        }

        private static void Destroy(List<T> list)
        {
            list.Clear();
        }
    }

    public static class DictionaryPool<TK, TV>
    {
        private static readonly UniversalPool<Dictionary<TK, TV>> pool = UniversalPool<Dictionary<TK, TV>>.Create(Factory, Destroy, l => l.Clear(), l => l.Clear());

        /// <summary>
        /// 得到池中新的Dictionary
        /// </summary>
        /// <returns></returns>
        public static Dictionary<TK, TV> Get()
        {
            return pool.Get();
        }

        /// <summary>
        /// 返回Dictionary
        /// </summary>
        /// <param name="dictionary"></param>
        public static void Return(Dictionary<TK, TV> dictionary)
        {
            pool.Return(dictionary);
        }

        private static Dictionary<TK, TV> Factory()
        {
            return new Dictionary<TK, TV>();
        }

        private static void Destroy(Dictionary<TK, TV> dictionary)
        {
            dictionary.Clear();
        }
    }

    public static class QueuePool<T>
    {
        private static readonly UniversalPool<Queue<T>> pool = UniversalPool<Queue<T>>.Create(Factory, Destroy, l => l.Clear(), l => l.Clear());

        /// <summary>
        /// 得到池中新的Queue
        /// </summary>
        /// <returns></returns>
        public static Queue<T> Get()
        {
            return pool.Get();
        }

        /// <summary>
        /// 返回Queue
        /// </summary>
        /// <param name="queue"></param>
        public static void Return(Queue<T> queue)
        {
            pool.Return(queue);
        }

        private static Queue<T> Factory()
        {
            return new Queue<T>();
        }

        private static void Destroy(Queue<T> queue)
        {
            queue.Clear();
        }
    }

    public static class StackPool<T>
    {
        private static readonly UniversalPool<Stack<T>> pool = UniversalPool<Stack<T>>.Create(Factory, Destroy, l => l.Clear(), l => l.Clear());

        /// <summary>
        /// 得到池中新的Stack
        /// </summary>
        /// <returns></returns>
        public static Stack<T> Get()
        {
            return pool.Get();
        }

        /// <summary>
        /// 返回Stack
        /// </summary>
        /// <param name="stack"></param>
        public static void Return(Stack<T> stack)
        {
            pool.Return(stack);
        }

        private static Stack<T> Factory()
        {
            return new Stack<T>();
        }

        private static void Destroy(Stack<T> stack)
        {
            stack.Clear();
        }
    }

    public static class HashSetPool<T>
    {
        private static readonly UniversalPool<HashSet<T>> pool = UniversalPool<HashSet<T>>.Create(Factory, Destroy, l => l.Clear(), l => l.Clear());

        /// <summary>
        /// 得到池中新的HashSet
        /// </summary>
        /// <returns></returns>
        public static HashSet<T> Get()
        {
            return pool.Get();
        }

        /// <summary>
        /// 返回HashSet
        /// </summary>
        /// <param name="hashSet"></param>
        public static void Return(HashSet<T> hashSet)
        {
            pool.Return(hashSet);
        }

        private static HashSet<T> Factory()
        {
            return new HashSet<T>();
        }

        private static void Destroy(HashSet<T> hashSet)
        {
            hashSet.Clear();
        }
    }

    public static class CommonObjectPool<T> where T : new()
    {
        private static readonly UniversalPool<T> pool = UniversalPool<T>.Create(Factory, null);

        /// <summary>
        /// 得到池中新的对象
        /// </summary>
        /// <returns></returns>
        public static T Get()
        {
            return pool.Get();
        }

        /// <summary>
        /// 返回对象
        /// </summary>
        /// <param name="hashSet"></param>
        public static void Return(T hashSet)
        {
            pool.Return(hashSet);
        }

        private static T Factory()
        {
            return new T();
        }

    }

    public static class ReusableObjectPool<T> where T : IStorable, new()
    {
        private static readonly UniversalPool<T> pool = UniversalPool<T>.Create(Factory, null, OnGet, OnReturn);

        /// <summary>
        /// 得到池中新的对象
        /// </summary>
        /// <returns></returns>
        public static T Get()
        {
            return pool.Get();
        }

        /// <summary>
        /// 返回对象
        /// </summary>
        /// <param name="obj"></param>
        public static void Return(T obj)
        {
            pool.Return(obj);
        }

        private static void OnGet(T obj)
        {
            obj.isDirty = false;
            obj.Reset();
        }

        //回池需要重置
        private static void OnReturn(T obj)
        {
            obj.isDirty = true;
            obj.BeforeStoreToPool();
        }

        private static T Factory()
        {
            return new T();
        }
    }

}

using System;
using System.Collections;
using System.Collections.Generic;
using System.Security.AccessControl;
using UnityEngine;

namespace ResetCore.Util
{
    public class ClassPool
    {

        private static BetterDictionary<Type, Stack<IStorable>> _stackMap = new BetterDictionary<Type, Stack<IStorable>>();

        /// <summary>
        /// 获取对应的储存栈
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        private static Stack<IStorable> GetStack<T>()
        {
            Type type = typeof(T);
            if (!_stackMap.ContainsKey(type))
            {
                _stackMap[type] = new Stack<IStorable>();
            }
            return _stackMap[type];
        }
        
        private static object _lockObject = new object();

        /// <summary>
        /// 获取对象
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        public static T GetObject<T>() 
            where T : class, IStorable, new()
        {
            lock (_lockObject)
            {
                var objectStack = GetStack<T>();
                T t = null;
                if (objectStack.Count > 0)
                {
                    t = objectStack.Pop() as T;
                    t.Reset();
                }
                else
                {
                    t = new T();
                }

                t.isDirty = false;
                return t;
            }
        }


        /// <summary>
        /// 储存对象
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="t"></param>
        /// <param name="maxStackCount"></param>
        public static void Store<T>(IStorable t, int maxStackCount) 
            where T : class, IStorable, new()
        {
            if(t == null)return;
            Stack<IStorable> objectStack = GetStack<T>();

            if (objectStack.Count < maxStackCount)
            {
                t.isDirty = true;
                t.BeforeStoreToPool();
                if (!objectStack.Contains(t))
                    objectStack.Push(t);
            }
        }

        /// <summary>
        /// 清理对象储存栈
        /// </summary>
        /// <typeparam name="T"></typeparam>
        public static void Clear<T>()
        {
            lock (_lockObject)
            {
                var objectStack = GetStack<T>();
                objectStack.Clear();
            }
        }

    }

    /// <summary>
    /// 可储存的对象
    /// </summary>
    public interface IStorable
    {
        /// <summary>
        /// 已经被修改过，如果需要复用则需要Reset
        /// </summary>
        bool isDirty { get; set; }

        /// <summary>
        /// 重置该对象
        /// </summary>
        void Reset();

        /// <summary>
        /// 储存到池中前需要做的
        /// </summary>
        void BeforeStoreToPool();
    }
}

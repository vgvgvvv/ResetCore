using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace ResetCore.Util
{
    /// <summary>
    /// 对象池
    /// </summary>
    public class ObjectPool
    {

        /// <summary>
        /// 储存池
        /// </summary>
        private static Dictionary<string, ObjectPool> poolDict = new Dictionary<string, ObjectPool>();

        /// <summary>
        /// 禁止被实例化
        /// </summary>
        protected ObjectPool() { }

        #region 双参数池
        /// <summary>
        /// 创建池
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="name"></param>
        /// <returns></returns>
        public static ObjectPool<K, T> CreatePool<K, T>(string name)
        {
            ObjectPool<K, T> newPool = new ObjectPool<K, T>();
            poolDict.Add(name, newPool);
            return newPool;
        }

        /// <summary>
        /// 获取池
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="name"></param>
        /// <returns></returns>
        public static ObjectPool<K, T> GetPool<K, T>(string name)
        {
            if (poolDict.ContainsKey(name))
            {
                return poolDict[name] as ObjectPool<K, T>;
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// 移除池
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="name"></param>
        public static void RemovePool<K, T>(string name)
        {
            if (poolDict.ContainsKey(name))
            {
                poolDict.Remove(name);
            }
        }
        #endregion //双参数池


        #region 单参数池
        /// <summary>
        /// 创建池
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="name"></param>
        /// <returns></returns>
        public static ObjectPool<string, T> CreatePool<T>(string name)
        {
            ObjectPool<string, T> newPool = new ObjectPool<string, T>();
            poolDict.Add(name, newPool);
            return newPool;
        }

        /// <summary>
        /// 获取池
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="name"></param>
        /// <returns></returns>
        public static ObjectPool<string, T> GetPool<T>(string name)
        {
            if (poolDict.ContainsKey(name))
            {
                return poolDict[name] as ObjectPool<string, T>;
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// 移除池
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="name"></param>
        public static void RemovePool<T>(string name)
        {
            if (poolDict.ContainsKey(name))
            {
                poolDict.Remove(name);
            }
        }
        #endregion //单参数池
    }

    public class ObjectPool<K, T> : ObjectPool
    {
        private Dictionary<K, T> pool;

        public ObjectPool()
        {
            pool = new Dictionary<K, T>();
        }

        /// <summary>
        /// 取出池
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        public T Get(K key)
        {
            return pool.ContainsKey(key) ? pool[key] : default(T);
        }

        /// <summary>
        /// 弹出池
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        public T PopOut(K key)
        {
            T value = Get(key);
            Remove(key);
            return value;
        }


        /// <summary>
        /// 放入池
        /// </summary>
        /// <param name="key"></param>
        /// <param name="value"></param>
        public void Put(K key, T value)
        {
            if (!pool.ContainsKey(key))
            {
                pool.Add(key, value);
            }
            else
            {
                pool[key] = value;
            }
        }

        /// <summary>
        /// 删除
        /// </summary>
        /// <param name="key"></param>
        public void Remove(K key)
        {
            if (pool.ContainsKey(key))
            {
                pool.Remove(key);
            }
        }

        /// <summary>
        /// 清理池
        /// </summary>
        public void Clear()
        {
            pool = new Dictionary<K, T>();
        }

        /// <summary>
        /// 是否包含值
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        public bool ContainsValue(T value)
        {
            return pool.ContainsValue(value);
        }

        /// <summary>
        /// 是否包含键
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        public bool ContainsKey(K key)
        {
            return pool.ContainsKey(key);
        }
    }

}

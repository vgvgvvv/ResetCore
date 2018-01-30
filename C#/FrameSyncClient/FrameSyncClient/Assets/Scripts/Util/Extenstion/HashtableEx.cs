using UnityEngine;
using System.Collections;
using System;

namespace ResetCore.Util
{
    public static class HashtableEx
    {
        /// <summary>
        /// 安全地获取Table中的泛型类型，如果获取到就执行动作
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="table"></param>
        /// <param name="name"></param>
        /// <param name="value"></param>
        /// <returns></returns>
        public static bool GetItemSafely<T>(this Hashtable table, string name, Action<T> act)
        {
            if (table.ContainsKey(name))
            {
                T item = (T)table[name];
                if (item == null)
                {
                    return false;
                }
                else
                {
                    act(item);
                    return true;
                }
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// 使用默认参数赋值
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="table"></param>
        /// <param name="name"></param>
        /// <param name="value"></param>
        /// <returns></returns>
        public static bool GetItemSafely<T>(this Hashtable table, string name, out T value)
        {
            value = default(T);
            if (table.ContainsKey(name))
            {
                T item = (T)table[name];
                if (item == null)
                {
                    return false;
                }
                else
                {
                    value = item;
                    return true;
                }
            }
            else
            {
                return false;
            }
        }


    }

}

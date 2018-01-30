using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ResetCore.Util
{
    public static class IEnumerableEx
    {
        /// <summary>
        /// 从集合中找到
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="iEnumerable"></param>
        /// <param name="fliter"></param>
        /// <returns></returns>
        public static T FindFirst<T>(this IEnumerable<T> iEnumerable, Func<T, bool> fliter)
        {
            foreach (var v in iEnumerable)
            {
                if (fliter(v))
                {
                    return v;
                }
            }
            return default(T);
        }

        /// <summary>
        /// 遍历迭代器
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="iEnumerable"></param>
        /// <param name="action"></param>
        public static void Foreach<T>(this IEnumerable<T> iEnumerable, Action<T> action)
        {
            var e = iEnumerable.GetEnumerator();
            while (e.MoveNext())
            {
                if(action == null)
                    continue;
                action(e.Current);
            }
            e.Dispose();
        }
    }

}

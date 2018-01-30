using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

namespace ResetCore.Util
{
    public static class ListEx
    {
        /// <summary>
        /// 遍历列表
        /// </summary>
        /// <typeparam name="T">列表类型</typeparam>
        /// <param name="list">目标表</param>
        /// <param name="act">行为</param>
        public static void ForEach<T>(this List<T> list, Action<int, T> act)
        {
            for (int i = 0; i < list.Count; i++)
            {
                act(i, list[i]);
            }
        }
        /// <summary>
        /// 获得随机列表中元素
        /// </summary>
        /// <typeparam name="T">元素类型</typeparam>
        /// <param name="list">列表</param>
        /// <returns></returns>
        public static T GetRandomItem<T>(this List<T> list)
        {
            return list[UnityEngine.Random.Range(0, list.Count - 1)];
        }

        /// <summary>
        /// 根据权值来获取索引
        /// </summary>
        /// <param name="powers"></param>
        /// <returns></returns>
        public static int GetRandomWithPower(this List<int> powers)
        {
            int sum = 0;
            for (int i = 0; i < powers.Count; i++)
            {
                sum += powers[i];
            }
            int randomNum = UnityEngine.Random.Range(0, sum);
            int currentSum = 0;
            int nextSum = 0;
            for (int i = 0; i < powers.Count; i++)
            {
                nextSum = currentSum + powers[i];
                if (randomNum >= currentSum && randomNum <= nextSum)
                {
                    return i;
                }
                currentSum = nextSum;
            }
            Debug.LogError("权值范围计算错误！");
            return -1;
        }

        /// <summary>
        /// 拷贝到
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="from"></param>
        /// <param name="to"></param>
        /// <param name="begin"></param>
        /// <param name="end"></param>
        public static void CopyTo<T>(this List<T> from, List<T> to, int begin = 0, int end = -1)
        {
            if(begin < 0)
            {
                begin = 0;
            }
            int endIndex = Mathf.Min(from.Count, to.Count) - 1;
            if (end != -1 && end < endIndex)
            {
                endIndex = end;
            }
            for(int i = begin; i < end; i++)
            {
                to[i] = from[i];
            }
        }

        /// <summary>
        /// 将List转为Array
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="from"></param>
        /// <returns></returns>
        public static T[] ToArraySavely<T>(this List<T> from)
        {
            T[] res = new T[from.Count];
            for(int i = 0; i < from.Count; i++)
            {
                res[i] = from[i];
            }
            return res;
        }

        /// <summary>
        /// 尝试获取，如果没有该数则返回null
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="from"></param>
        /// <param name="index"></param>
        /// <returns></returns>
        public static T TryGet<T>(this List<T> from, int index)
        {
            if(from.Count > index)
            {
                return from[index];
            }
            return default(T);
        }
    }

}

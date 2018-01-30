using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

public static class ArrayEx {

    /// <summary>
    /// 遍历数组
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="array"></param>
    /// <param name="act"></param>
	public static void Foreach<T>(this Array array, Action<int, T> act)
    {
        for(int i = 0; i < array.Length; i++)
        {
            act(i, (T)array.GetValue(i));
        }
    }

    /// <summary>
    /// 遍历数组
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="array"></param>
    /// <param name="act"></param>
    public static void Foreach<T>(this Array array, Action<T> act)
    {
        for (int i = 0; i < array.Length; i++)
        {
            act((T)array.GetValue(i));
        }
    }

    /// <summary>
    /// 数组是否包含特定值
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="array"></param>
    /// <param name="obj"></param>
    /// <returns></returns>
    public static bool Contains<T>(this Array array, T obj)
    {
        for (int i = 0; i < array.Length; i++)
        {
            if (array.GetValue(i).Equals(obj))
            {
                return true;
            }
        }
        return false;
    }

    /// <summary>
    /// 获取子数组
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="array"></param>
    /// <param name="sourceStart"></param>
    /// <param name="targetStart"></param>
    /// <param name="length"></param>
    /// <returns></returns>
    public static T[] SubArray<T>(this T[] array, int sourceStart, int length)
    {
        T[] res = new T[length];
        Array.Copy(array, sourceStart, res, 0, length);
        return res;
    }

    /// <summary>
    /// 连接两个数组
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="array"></param>
    /// <param name="anotherArray"></param>
    /// <returns></returns>
    public static T[] Concat<T>(this T[] array, T[] anotherArray)
    {
        T[] res = new T[array.Length + anotherArray.Length];
        Array.Copy(array, 0, res, 0, array.Length);
        Array.Copy(anotherArray, 0, res, array.Length, anotherArray.Length);
        return res;
    }

    /// <summary>
    /// 切分数组至多个
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="array"></param>
    /// <param name="lengthList"></param>
    /// <returns></returns>
    public static List<T[]> SplitByLength<T>(this T[] array, params int[] lengthList)
    {
        List<T[]> res = new List<T[]>();
        int startIndex = 0;
        T[] tempArr;
        int length;
        for (int i = 0; i < lengthList.Length; i++)
        {
            if (startIndex + lengthList[i] > array.Length)
            {
                length = array.Length - startIndex;
            }
            else
            {
                length = lengthList[i];
            }

            tempArr = new T[length];
            Array.Copy(array, startIndex, tempArr, 0, length);
            res.Add(tempArr);
            startIndex += length;
            tempArr = null;

            if (startIndex >= array.Length)
                break;
        }

        return res;
    }

    /// <summary>
    /// 切出头部数组，并且返回剩余部分
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="array"></param>
    /// <param name="length"></param>
    /// <returns></returns>
    public static T[] CutHeadByLength<T>(ref T[] array, int length)
    {
        if (length > array.Length)
            length = array.Length;
        T[] head = new T[length];
        T[] tail = new T[array.Length - length];
        
        Array.Copy(array, 0, head, 0, length);
        Array.Copy(array, length, tail, 0, array.Length - length);
        array = tail;
        return head;
    }
}

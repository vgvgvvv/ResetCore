using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ResetCore.Util
{
    /// <summary>
    /// 可以使用数组访问的Dictionary
    /// </summary>
    /// <typeparam name="KeyT"></typeparam>
    /// <typeparam name="ValueT"></typeparam>
    public class DictionaryList<KeyT, ValueT> : IDisposable
    {

        private Dictionary<KeyT, ValueT> _dic = new Dictionary<KeyT, ValueT>();
        private List<ValueT> _list = new List<ValueT>();

        /// <summary>
        /// 获取一个值
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        public ValueT GetValue(KeyT key)
        {
            ValueT tar = default(ValueT);
            _dic.TryGetValue(key, out tar);
            return tar;
        }

        /// <summary>
        /// 获取特定个数的值
        /// </summary>
        /// <param name="index"></param>
        /// <returns></returns>
        public ValueT GetValueAt(int index)
        {
            return _list[index];
        }

        /// <summary>
        /// 添加一个项
        /// </summary>
        /// <param name="key"></param>
        /// <param name="value"></param>
        public bool Add(KeyT key, ValueT value)
        {
            if (!_dic.ContainsKey(key))
            {
                _dic.Add(key, value);
                _list.Add(value);
                return true;
            }
            return false;
        }

        /// <summary>
        /// 移除一个项目
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        public bool Remove(KeyT key)
        {
            ValueT tar = default(ValueT);
            _dic.TryGetValue(key, out tar);

            if (tar != null)
            {
                _list.Remove(tar);
                _dic.Remove(key);
            }
            else
            {
                return false;
            }
            return true;
        }

        /// <summary>
        /// 清理数据结构
        /// </summary>
        public void Clear()
        {
            _dic.Clear();
            _list.Clear();
        }

        /// <summary>
        /// 数量
        /// </summary>
        public int Count
        {
            get
            {
                return _dic.Count;
            }
        }


        public void Dispose()
        {
            Clear();
            _dic = null;
            _list = null;
        }
    }
}

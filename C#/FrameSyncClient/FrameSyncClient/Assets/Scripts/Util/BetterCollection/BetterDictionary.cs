using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ResetCore.Util
{
    public class BetterDictionary<TKey, TValue> : Dictionary<TKey, TValue>
    {

        public delegate bool CheckList(KeyValuePair<TKey, TValue> map);

        private readonly List<KeyValuePair<TKey, TValue>> _keyValuePairs = new List<KeyValuePair<TKey, TValue>>();

        public List<KeyValuePair<TKey, TValue>> KeyValuePairs
        {
            get
            {
                _keyValuePairs.Clear();
                var e = GetEnumerator();

                while (e.MoveNext())
                {
                    TKey key = e.Current.Key;
                    TValue value = e.Current.Value;
                    KeyValuePair<TKey, TValue> kvp = new KeyValuePair<TKey, TValue>();
                    _keyValuePairs.Add(kvp);
                }
                e.Dispose();
                return _keyValuePairs;
            }
        }

        private List<TValue> _valueList = null;
        public List<TValue> GetValueList()
        {
            if(_valueList == null)
                _valueList = new List<TValue>();
            else
                _valueList.Clear();
            var e = GetEnumerator();

            while (e.MoveNext())
            {
                _valueList.Add(e.Current.Value);
            }
            e.Dispose();
            return _valueList;
        }

        public void GetKeyValuePairs(out List<KeyValuePair<TKey, TValue>> keyValuePairs)
        {
            keyValuePairs = new List<KeyValuePair<TKey, TValue>>();

            var e = GetEnumerator();

            while (e.MoveNext())
            {
                TKey key = e.Current.Key;
                TValue value = e.Current.Value;
                KeyValuePair<TKey, TValue> kvp = new KeyValuePair<TKey, TValue>(key, value);
                keyValuePairs.Add(kvp);
            }
            e.Dispose();

        }

        /// <summary>
        /// 更好的遍历 
        /// </summary>
        /// <param name="callback"></param>
        public void BetterForeach(Action<KeyValuePair<TKey, TValue>> callback)
        {
            KeyValuePairs.Clear();

            var e = GetEnumerator();

            while (e.MoveNext())
            {
                TKey key = e.Current.Key;
                TValue value = e.Current.Value;
                KeyValuePair<TKey, TValue> kvp = new KeyValuePair<TKey, TValue>(key, value);
                KeyValuePairs.Add(kvp);
            }
            e.Dispose();

            for (int i = 0; i < KeyValuePairs.Count; i++)
            {
                callback(KeyValuePairs[i]);
            }
            KeyValuePairs.Clear();
        }

        /// <summary>
        /// 更好的检查函数
        /// </summary>
        /// <param name="callback"></param>
        /// <returns></returns>
        public bool CheckBetterForeach(CheckList callback)
        {
            KeyValuePairs.Clear();

            var e = GetEnumerator();

            while (e.MoveNext())
            {
                TKey key = e.Current.Key;
                TValue value = e.Current.Value;
                KeyValuePair<TKey, TValue> kvp = new KeyValuePair<TKey, TValue>(key, value);
                KeyValuePairs.Add(kvp);
            }
            e.Dispose();

            for (var i = 0; i < KeyValuePairs.Count; i++)
            {
                if (false != callback(KeyValuePairs[i])) continue;
                KeyValuePairs.Clear();
                return true;
            }
            KeyValuePairs.Clear();
            return true;
        }

        /// <summary>
        /// 更好的遍历
        /// </summary>
        /// <param name="callback"></param>
        /// <param name="indexStart"></param>
        /// <param name="indexEnd"></param>
        public void BetterForeach(Action<KeyValuePair<TKey, TValue>> callback, int indexStart, int indexEnd)
        {
            KeyValuePairs.Clear();

            var e = GetEnumerator();

            while (e.MoveNext())
            {
                TKey key = e.Current.Key;
                TValue value = e.Current.Value;
                KeyValuePair<TKey, TValue> kvp = new KeyValuePair<TKey, TValue>(key, value);
                KeyValuePairs.Add(kvp);
            }
            e.Dispose();

            indexEnd = indexEnd > KeyValuePairs.Count ? KeyValuePairs.Count : indexEnd;
            for (int i = indexStart; i < indexEnd; i++)
            {
                callback(KeyValuePairs[i]);
            }
            KeyValuePairs.Clear();
        }

        /// <summary>
        /// 更好的回调
        /// </summary>
        /// <param name="callback"></param>
        public void BetterForeach(Func<int, KeyValuePair<TKey, TValue>, bool> callback)
        {
            KeyValuePairs.Clear();

            var e = GetEnumerator();

            while (e.MoveNext())
            {
                TKey key = e.Current.Key;
                TValue value = e.Current.Value;
                KeyValuePair<TKey, TValue> kvp = new KeyValuePair<TKey, TValue>(key, value);
                KeyValuePairs.Add(kvp);
            }
            e.Dispose();

            for (int i = 0; i < KeyValuePairs.Count; i++)
            {
                if(false == callback(i, KeyValuePairs[i])) break;
            }
            KeyValuePairs.Clear();
        }

        /// <summary>
        /// 暂时将要清理的东西缓存起来
        /// </summary>
        private List<TKey> _removeCache = null;
        public void RemoveFromCache(TKey key)
        {
            if (_removeCache == null) _removeCache = new List<TKey>();
            _removeCache.Add(key);
        }

        /// <summary>
        /// 将缓存的键全部移除
        /// </summary>
        public void RemoveFromCache()
        {
            if(_removeCache == null) return;
            for (int i = 0; i < _removeCache.Count; i++)
            {
                Remove(_removeCache[i]);
            }
            _removeCache.Clear();
        }

        /// <summary>
        /// 清理
        /// </summary>
        new public void Clear()
        {
            base.Clear();
            if(_removeCache != null)
                _removeCache.Clear();
            _keyValuePairs.Clear();
        }

    }

}

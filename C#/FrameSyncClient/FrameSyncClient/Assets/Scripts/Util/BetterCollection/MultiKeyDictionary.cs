using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace ResetCore.Util
{
    /// <summary>
    /// 三值词典
    /// </summary>
    /// <typeparam name="T1"></typeparam>
    /// <typeparam name="T2"></typeparam>
    /// <typeparam name="T3"></typeparam>
    public class MultiKeyDictionary<T1, T2, T3> : Dictionary<T1, Dictionary<T2, T3>>
    {
        new public Dictionary<T2, T3> this[T1 key]
        {
            get
            {
                if (!ContainsKey(key))
                    Add(key, new Dictionary<T2, T3>());

                Dictionary<T2, T3> returnObj;
                TryGetValue(key, out returnObj);

                return returnObj;
            }
        }

        /// <summary>
        /// 是否包含键值
        /// </summary>
        /// <param name="key1"></param>
        /// <param name="key2"></param>
        /// <returns></returns>
        public bool ContainsKey(T1 key1, T2 key2)
        {
            Dictionary<T2, T3> returnObj;
            TryGetValue(key1, out returnObj);
            if (returnObj == null)
                return false;

            return returnObj.ContainsKey(key2);
        }
    }

}

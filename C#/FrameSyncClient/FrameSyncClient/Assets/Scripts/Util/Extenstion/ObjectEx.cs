using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ResetCore.Util
{
    public static class ObjectEx
    {
        /// <summary>
        /// 用于避免null
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="obj"></param>
        /// <param name="def"></param>
        /// <returns></returns>
        public static T IfNull<T>(this object obj, T def)
        {
            if(obj == null)
            {
                return def;
            }
            return (T)obj;
        }

    }

}

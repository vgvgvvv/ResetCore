using UnityEngine;
using System.Collections;
using System;

namespace ResetCore.Util
{
    public static class EnumEx
    {
        /// <summary>
        /// int转enum
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="id"></param>
        /// <returns></returns>
        public static T GetValue<T>(int id)
        {
            return (T)Enum.ToObject(typeof(T), id);
        }

        /// <summary>
        /// string转enum
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="name"></param>
        /// <returns></returns>
        public static T GetValue<T>(string name)
        {
            return (T)Enum.Parse(typeof(T), name);
        }
    }

}

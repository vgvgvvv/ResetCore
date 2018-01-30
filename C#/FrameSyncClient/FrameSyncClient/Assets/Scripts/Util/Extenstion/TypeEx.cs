using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

namespace ResetCore.Util
{
    public static class TypeEx
    {
        /// <summary>
        /// 获取默认值
        /// </summary>
        /// <param name="targetType"></param>
        /// <returns></returns>
        public static object DefaultForType(this Type targetType)
        {
            return targetType.IsValueType ? Activator.CreateInstance(targetType) : null;
        }

    }

}

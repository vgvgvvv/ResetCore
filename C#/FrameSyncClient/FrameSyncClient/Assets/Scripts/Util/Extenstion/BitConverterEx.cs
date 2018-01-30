using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

namespace ResetCore.Util
{
    public static class BitConverterEx
    {

        /// <summary>
        /// 获取基本值类型的byte
        /// </summary>
        /// <returns></returns>
        public static byte[] GetBytes(object obj)
        {
            if (obj is bool)
            {
                return BitConverter.GetBytes((bool)obj);
            }
            else if(obj is char)
            {
                return BitConverter.GetBytes((char)obj);
            }
            else if (obj is int)
            {
                return BitConverter.GetBytes((int)obj);
            }
            else if (obj is short)
            {
                return BitConverter.GetBytes((short)obj);
            }
            else if (obj is long)
            {
                return BitConverter.GetBytes((long)obj);
            }
            else if (obj is float)
            {
                return BitConverter.GetBytes((float)obj);
            }
            else if (obj is double)
            {
                return BitConverter.GetBytes((double)obj);
            }
            else if (obj is uint)
            {
                return BitConverter.GetBytes((uint)obj);
            }
            else if (obj is ulong)
            {
                return BitConverter.GetBytes((ulong)obj);
            }
            else if (obj is ushort)
            {
                return BitConverter.GetBytes((ushort)obj);
            }
            return new byte[0];
        }
    }

}

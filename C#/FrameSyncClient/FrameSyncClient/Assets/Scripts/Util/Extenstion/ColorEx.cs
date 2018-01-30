using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

namespace ResetCore.Util
{
    public static class ColorEx
    {
        /// <summary>
        /// 颜色转16进制
        /// </summary>
        /// <param name="color"></param>
        /// <returns></returns>
        public static string Color2String(this Color color)
        {
            StringBuilder builder = new StringBuilder();
            builder.Append("#")
                .Append(Convert.ToString((int)color.r, 16))
                .Append(Convert.ToString((int)color.g, 16))
                .Append(Convert.ToString((int)color.b, 16));
            return builder.ToString();
        }

        /// <summary>
        /// 16进制转颜色
        /// </summary>
        /// <param name="color"></param>
        /// <returns></returns>
        public static Color String2Color(this string color)
        {
            if (color.StartsWith("#"))
                color = color.Substring(1);
            int r = Convert.ToInt32(color.Substring(0, 2), 16);
            int g = Convert.ToInt32(color.Substring(2, 2), 16);
            int b = Convert.ToInt32(color.Substring(4, 2), 16);
            return new Color(r, g, b);
        }
        
    }

}

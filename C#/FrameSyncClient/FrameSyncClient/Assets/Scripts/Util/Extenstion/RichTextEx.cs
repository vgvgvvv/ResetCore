using System.Collections;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

namespace ResetCore.Util.RichText
{
    public static class RichTextEx
    {

        /// <summary>
        /// 使文本有颜色
        /// </summary>
        /// <param name="source"></param>
        /// <param name="color"></param>
        /// <returns></returns>
        public static string MakeColor(this string source, string color)
        {
            StringBuilder builder = new StringBuilder();
            builder.Append("<color=").Append(color).Append(">").Append(source).Append("</color>");
            return builder.ToString();
        }

        /// <summary>
        /// 使文本有颜色
        /// </summary>
        /// <param name="source"></param>
        /// <param name="color"></param>
        /// <returns></returns>
        public static string MakeColor(this string source, Color color)
        {
            return MakeColor(source, color.Color2String());
        }

        /// <summary>
        /// 变成粗体 
        /// </summary>
        /// <param name="source"></param>
        /// <returns></returns>
        public static string MakeBold(this string source)
        {
            StringBuilder builder = new StringBuilder();
            builder.Append("<b>").Append(source).Append("</b>");
            return builder.ToString();
        }

        /// <summary>
        /// 变成粗体 
        /// </summary>
        /// <param name="source"></param>
        /// <returns></returns>
        public static string MakeItalics(this string source)
        {
            StringBuilder builder = new StringBuilder();
            builder.Append("<i>").Append(source).Append("</i>");
            return builder.ToString();
        }

        /// <summary>
        /// 改变字体大小
        /// </summary>
        /// <param name="source"></param>
        /// <param name="size"></param>
        /// <returns></returns>
        public static string MakeSize(this string source, int size)
        {
            StringBuilder builder = new StringBuilder();
            builder.Append("<size=").Append(size).Append(">").Append(source).Append("</size>");
            return builder.ToString();
        }

        /// <summary>
        /// 改变材质
        /// </summary>
        /// <param name="source"></param>
        /// <param name="matId"></param>
        /// <returns></returns>
        public static string MakeMaterial(this string source, int matId)
        {
            StringBuilder builder = new StringBuilder();
            builder.Append("<material=").Append(matId).Append(">").Append(source).Append("</material>");
            return builder.ToString();
        }

        /// <summary>
        /// 插入图像
        /// </summary>
        /// <param name="source"></param>
        /// <param name="matId"></param>
        /// <param name="size"></param>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <returns></returns>
        public static string Quad(this string source, int matId, int size, float x, float y, float width, float height)
        {
            StringBuilder builder = new StringBuilder();
            builder.Append("< quad material=").Append(matId)
                .Append(" size=").Append(size)
                .Append(" x=").Append(x)
                .Append(" y=").Append(y)
                .Append(" width=").Append(width)
                .Append(" height=").Append(height)
                .Append(">").Append(source).Append("</material>");
            return builder.ToString();
        }
    }
}


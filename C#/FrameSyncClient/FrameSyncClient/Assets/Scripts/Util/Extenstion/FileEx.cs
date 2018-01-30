using UnityEngine;
using System.Collections;
using System.IO;

namespace ResetCore.Util
{
    public static class FileEx
    {
        /// <summary>
        /// 保存文本
        /// </summary>
        /// <param name="text"></param>
        /// <param name="path"></param>
        public static void SaveText(string text, string path)
        {
            if (File.Exists(path))
            {
                File.Delete(path);
            }
            using (FileStream fs = new FileStream(path, FileMode.Create, FileAccess.Write))
            {
                using (StreamWriter sr = new StreamWriter(fs))
                {
                    sr.Write(text);//开始写入值
                }
            }
        }

        /// <summary>
        /// 读取文本
        /// </summary>
        /// <param name="file"></param>
        /// <returns></returns>
        public static string ReadText(this FileInfo file)
        {
            var result = string.Empty;
            using (FileStream fs = new FileStream(file.FullName, FileMode.Open, FileAccess.Read))
            {
                using (StreamReader sr = new StreamReader(fs))
                {
                    result = sr.ReadToEnd();
                }
            }
            return result;
        }

#if UNITY_EDITOR
        /// <summary>
        /// 打开文件夹
        /// </summary>
        /// <param name="path"></param>
        public static void OpenFolder(string path)
        {
            System.Diagnostics.Process.Start("explorer.exe", path);
        }
#endif

        /// <summary>
        /// 获取文件夹名
        /// </summary>
        /// <param name="fileName"></param>
        /// <returns></returns>
        public static string GetDirectoryName(string fileName)
        {
            fileName = PathEx.MakePathStandard(fileName);
            return fileName.Substring(0, fileName.LastIndexOf('/'));
        }

        /// <summary>
        /// 获取文件名
        /// </summary>
        /// <param name="path"></param>
        /// <param name="separator"></param>
        /// <returns></returns>
        public static string GetFileName(string path, char separator = '/')
        {
            path = PathEx.MakePathStandard(path);
            return path.Substring(path.LastIndexOf(separator) + 1);
        }

        /// <summary>
        /// 获取不带后缀的文件名
        /// </summary>
        /// <param name="fileName"></param>
        /// <param name="separator"></param>
        /// <returns></returns>
        public static string GetFileNameWithoutExtention(string fileName, char separator = '/')
        {
            return GetFilePathWithoutExtention(GetFileName(fileName, separator));
        }
        /// <summary>
        /// 获取不带后缀的文件路径
        /// </summary>
        /// <param name="fileName"></param>
        /// <returns></returns>
        public static string GetFilePathWithoutExtention(string fileName)
        {
            if (fileName.Contains("."))
                return fileName.Substring(0, fileName.LastIndexOf('.'));
            return fileName;
        }
        /// <summary>
        /// 获取streamingAssetsPath
        /// </summary>
        /// <param name="fileName"></param>
        /// <returns></returns>
        public static string GetStreamPath(string fileName)
        {
            string str = Application.streamingAssetsPath + "/" + fileName;
            if (Application.platform != RuntimePlatform.Android)
            {
                str = "file://" + str;
            }
            return str;
        }

    }

}

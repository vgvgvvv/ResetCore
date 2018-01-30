using UnityEngine;
using System.Collections;
using System.IO;

namespace ResetCore.Util
{
    public static class DirectoryEx
    {
        /// <summary>
        /// 拷贝文件夹
        /// </summary>
        /// <param name="from"></param>
        /// <param name="to"></param>
        /// <param name="copySubDirs"></param>
        public static void DirectoryCopy(string from, string to, bool copySubDirs)
        {
            DirectoryInfo dir = new DirectoryInfo(from);
            DirectoryInfo[] dirs = dir.GetDirectories();
            if (!dir.Exists)
            {
                throw new DirectoryNotFoundException(
                    "Source directory does not exist or could not be found: "
                    + from);
            }
            if (!Directory.Exists(to))
            {
                Directory.CreateDirectory(to);
            }
            FileInfo[] files = dir.GetFiles();
            foreach (FileInfo file in files)
            {
                string temppath = Path.Combine(to, file.Name);
                file.CopyTo(temppath, false);
            }
            if (copySubDirs)
            {
                foreach (DirectoryInfo subdir in dirs)
                {
                    string temppath = Path.Combine(to, subdir.Name);
                    DirectoryCopy(subdir.FullName, temppath, copySubDirs);
                }
            }
        }

        /// <summary>
        /// 获取文件
        /// </summary>
        public static FileInfo GetFile(this DirectoryInfo dir, string fileName, SearchOption option = SearchOption.TopDirectoryOnly)
        {
            FileInfo[] files = dir.GetFiles(fileName, option);
            if (files.Length == 0) return null;
            return files[0];
        }

        /// <summary>
        /// 是否存在文件
        /// </summary>
        /// <param name="dir"></param>
        /// <param name="fileName"></param>
        public static bool ContainsFile(this DirectoryInfo dir, string fileName, SearchOption option = SearchOption.TopDirectoryOnly)
        {
            FileInfo[] files = dir.GetFiles(fileName, option);
            return (files.Length > 0);
        }

        /// <summary>
        /// 获取目录
        /// </summary>
        /// <param name="dir"></param>
        /// <param name="dirName"></param>
        /// <param name="option"></param>
        /// <returns></returns>
        public static DirectoryInfo GetDirectroy(this DirectoryInfo dir, string dirName, SearchOption option = SearchOption.TopDirectoryOnly)
        {
            DirectoryInfo[] dirs = dir.GetDirectories(dirName, option);
            if (dirs.Length == 0) return null;
            return dirs[0];
        }
        
        /// <summary>
        /// 是否存在目录
        /// </summary>
        /// <param name="dir"></param>
        /// <param name="dirName"></param>
        /// <param name="option"></param>
        /// <returns></returns>
        public static bool ContainsDirectroy(this DirectoryInfo dir, string dirName, SearchOption option = SearchOption.TopDirectoryOnly)
        {
            DirectoryInfo[] dirs = dir.GetDirectories(dirName, option);
            return (dirs.Length > 0);
        }
    }

}

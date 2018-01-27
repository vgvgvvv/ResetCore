using System;
using System.Collections.Generic;

namespace ReFrame.Util.Context
{
    public static class InterfaceBinder
    {
        private static readonly Dictionary<Type, Dictionary<string, object>> 
            ContextPool = new Dictionary<Type, Dictionary<string, object>>();
        
        /// <summary>
        /// 绑定接口
        /// </summary>
        /// <param name="name"></param>
        /// <typeparam name="T"></typeparam>
        /// <typeparam name="V"></typeparam>
        public static void Bind<T, V>(string name = "default") where V : class, T, new() 
        {
            Dictionary<string, object> targetInterfaceDict;
            Type targetInterface = typeof(T);
            if (!ContextPool.TryGetValue(targetInterface, out targetInterfaceDict))
            {
                targetInterfaceDict = new Dictionary<string, object>();
                ContextPool.Add(targetInterface, targetInterfaceDict);
            }

            object targetObject;
            if (!targetInterfaceDict.TryGetValue(name, out targetObject))
            {
                var result = new V();
                targetInterfaceDict.Add(name, result);
            }
            else
            {
                targetInterfaceDict[name] = new V();
            }
        }

        /// <summary>
        /// 获得绑定的相应类
        /// </summary>
        /// <param name="name"></param>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        public static T Get<T>(string name = "default") where T : class
        {
            Dictionary<string, object> targetInterfaceDict;
            Type targetInterface = typeof(T);
            if (!ContextPool.TryGetValue(targetInterface, out targetInterfaceDict))
            {
                return null;
            }
            object targetObject;
            if (!targetInterfaceDict.TryGetValue(name, out targetObject))
            {
                return null;
            }
            return targetObject as T;
        }
    }
}
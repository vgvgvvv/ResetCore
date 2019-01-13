using System;
using System.Collections.Generic;
using System.Reflection;
using ReFrame.Util.Utility;

namespace ReFrame.Util
{
    /// <summary>
    /// 快速获取类型ID
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public static class MUUID
    {
        private static readonly Dictionary<Type, ulong> uuidMap = new Dictionary<Type, ulong>();
        public static ulong Get(Type type)
        {
            Assert.IsNull(type);

            if (uuidMap.TryGetValue(type, out var result))
            {
                return result;
            }

            var uuidMethod = typeof(MUUID<>).MakeGenericType(type)
                .GetProperty("UUID", BindingFlags.Static | BindingFlags.Public);

            if (uuidMethod == null)
            {
                Context.Logger.LogError($"Cannot find uuid for {type.Name}");
                return 0;
            }

            result = Convert.ToUInt64(uuidMethod.GetValue(null));
            uuidMap.Add(type, result);

            return result;
        }
    }

    /// <summary>
    /// 快速获取类型ID
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public static class MUUID<T>
    {
        private static Func<ulong> _getuuid = DefaultGetUUID;

        private static ulong uuid = 0;

        public static void Init(Func<ulong> getter)
        {
            Assert.IsNull(getter);
            _getuuid = getter;
        }

        public static ulong UUID
        {
            get
            {
                if (uuid == 0)
                {
                    uuid = _getuuid();
                }

                return uuid;
            }
        }

        public static ulong DefaultGetUUID()
        {
            return Hash.Get(typeof(T).Name);
        }
    }
}
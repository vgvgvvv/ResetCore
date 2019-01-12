using System;
using ReFrame.Core;

namespace ReFrame.Trigger
{
    public interface IArg<T> : IBase
    {
        /// <summary>
        /// 实际值
        /// </summary>
        T Value { get; }

        /// <summary>
        /// 默认值
        /// </summary>
        T DefaultValue { get; }

    }
}
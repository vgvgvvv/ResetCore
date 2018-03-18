using System;
using ReRx.Operators;

namespace ReRx
{
    /// <summary>
    /// 观察者，给订阅者发送消息
    /// </summary>
    public static partial class Observable
    {
        /// <summary>
        /// 范围
        /// </summary>
        /// <param name="start"></param>
        /// <param name="end"></param>
        /// <returns></returns>
        public static IObservable<int> Range(int start, int end)
        {
            return new RangeObservable(start, end);
        }
        
    }
}
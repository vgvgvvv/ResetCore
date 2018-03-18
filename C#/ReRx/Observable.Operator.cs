using System;
using ReRx.Operators;

namespace ReRx
{
    public static partial class Observable
    {
        public static IObservable<T> Where<T>(this IObservable<T> source, Func<T, bool> condition)
        {
            return new WhereObservable<T>(source, condition);
        }
    }
}
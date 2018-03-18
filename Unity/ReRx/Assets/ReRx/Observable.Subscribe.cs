using System;

namespace ReRx
{
    public static partial class Observable
    {
        /// <summary>
        /// 订阅
        /// </summary>
        /// <param name="observable"></param>
        /// <param name="onNext"></param>
        /// <param name="onComplete"></param>
        /// <param name="onError"></param>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        public static IDisposable Subscribe<T>(this IObservable<T> observable, Action<T> onNext, Action onComplete = null, Action<Exception> onError = null)
        {
            return observable.Subscribe(Observer<T>.Get(onNext, onComplete, onError));
        }
    }
}
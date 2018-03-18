
using System;

namespace ReRx.Operators
{
    public abstract class OperatorObservableBase<T> : IObservable<T>
    {
        public IDisposable Subscribe(IObserver<T> observer)
        {
            return SubscribeCore(observer);
        }
        
        protected abstract IDisposable SubscribeCore(IObserver<T> observer);
    }
}
using System;

namespace ReRx.Operators
{
    public abstract class OperatorObserverBase<TSource, TResult> : IDisposable, IObserver<TSource>
    {
        
        protected internal volatile IObserver<TResult> observer;

        public OperatorObserverBase(IObserver<TResult> observer)
        {
            this.observer = observer;
        }
        
        public abstract void OnNext(TSource value);

        public abstract void OnError(Exception error);

        public abstract void OnCompleted();

        public virtual void Dispose()
        {
            observer = null;
        }
    }
}
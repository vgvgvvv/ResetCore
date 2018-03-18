using System;

namespace ReRx.Operators
{
    public class WhereObservable<T> : OperatorObservableBase<T>
    {
        private IObservable<T> _source;
        private Func<T, bool> _condition;
        
        public WhereObservable(IObservable<T> source, Func<T, bool> condition)
        {
            _source = source;
            _condition = condition;
        }
        
        protected override IDisposable SubscribeCore(IObserver<T> observer)
        {
            observer = new Where<T>(this, observer);
            return _source.Subscribe(observer);
        }

        class Where<T> : OperatorObserverBase<T, T>
        {
            private WhereObservable<T> _parent;
            
            public Where(WhereObservable<T> parent, IObserver<T> observer) : base(observer)
            {
                _parent = parent;
            }

            public override void OnNext(T value)
            {
                var pass = false;
                try
                {
                    pass = _parent._condition(value);
                }
                catch (Exception e)
                {
                    try{ observer.OnError(e); } finally { Dispose(); }
                    return;
                }
                if (pass)
                {
                    observer.OnNext(value);
                }
            }

            public override void OnError(Exception error)
            {
                observer.OnError(error);
            }

            public override void OnCompleted()
            {
                observer.OnCompleted();
            }
        }
    }
}
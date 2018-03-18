using System;

namespace ReRx.Operators
{
    public class RangeObservable : OperatorObservableBase<int>
    {
        private int _start;
        private int _end;
        
        public RangeObservable(int start, int end)
        {
            _start = start;
            _end = end;
        }
        
        protected override IDisposable SubscribeCore(IObserver<int> observer)
        {
            observer = new Range(observer);
            for (int i = 0; i < _end; i++)
            {
                int v = _start + i;
                observer.OnNext(v);
            }
            observer.OnCompleted();

            return null;
        }
        
        class Range : OperatorObserverBase<int, int>
        {
            public Range(IObserver<int> observer) : base(observer)
            {
            }

            public override void OnNext(int value)
            {
                observer.OnNext(value);
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
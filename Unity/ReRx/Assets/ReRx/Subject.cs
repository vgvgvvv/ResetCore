using System;
using System.Collections.Generic;
using ReRx.Interface;

namespace ReRx
{
    /// <summary>
    /// 可订阅主题
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class Subject<T> : ISubject<T>, IDisposable
    {
        private bool isStop = false;
        private bool isDispose = false;
        Exception lastError;
        private List<IObserver<T>> outObserver = new List<IObserver<T>>();
        
        
        public void OnCompleted()
        {
            if(isStop || isDispose) return;
            isStop = true;

            foreach (var observer in outObserver)
            {
                observer.OnCompleted();
            }
        }

        public void OnError(Exception error)
        {
            if(isStop || isDispose) return;
            isStop = true;
            lastError = error;
            foreach (var observer in outObserver)
            {
                observer.OnError(error);
            }
        }

        public void OnNext(T value)
        {
            foreach (var observer in outObserver)
            {
                observer.OnNext(value);
            }
        }

        public IDisposable Subscribe(IObserver<T> observer)
        {
            if (isStop || isDispose)
            {
                if (lastError != null)
                {
                    observer.OnError(lastError);
                }
                else
                {
                    observer.OnCompleted();
                }
                return null;
            }
                
            
            outObserver.Add(observer);

            return new Subscription(this, observer);

        }

        public void Dispose()
        {
            isDispose = true;
            outObserver.Clear();
        }


        /// <summary>
        /// 单个订阅
        /// </summary>
        class Subscription : IDisposable
        {
            private Subject<T> parent;
            private IObserver<T> unsubscribeTarget;
            
            public Subscription(Subject<T> parent, IObserver<T> unsubscribeTarget)
            {
                this.parent = parent;
                this.unsubscribeTarget = unsubscribeTarget;
            }
            
            public void Dispose()
            {
                parent.outObserver.Remove(unsubscribeTarget);
                unsubscribeTarget = null;
                parent = null;
            }
        }
    }
}
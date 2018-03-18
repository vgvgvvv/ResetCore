using System;

namespace ReRx
{
    /// <summary>
    /// 订阅者，接收消息并且处理
    /// </summary>
    public class Observer<T> : IObserver<T>
    {
        private Action<T> _onNext;
        private Action _onComplete;
        private Action<Exception> _onError;

        private bool _isStop = false;
        
        private Observer(){}
        
        public static Observer<T> Get(Action<T> onNext, Action onComplete = null, Action<Exception> onError = null)
        {
            var observer = new Observer<T>();
            observer._onNext = onNext;
            observer._onComplete = onComplete;
            observer._onError = onError;
            observer._isStop = false;
            return observer;
        }

        public void OnCompleted()
        {
            if (_onComplete != null && !_isStop)
                _onComplete();
        }

        public void OnError(Exception error)
        {
            if (_onError != null && !_isStop)
                _onError(error);
        }

        public void OnNext(T value)
        {
            if (_onNext != null && !_isStop)
                _onNext(value);
        }
    }
}
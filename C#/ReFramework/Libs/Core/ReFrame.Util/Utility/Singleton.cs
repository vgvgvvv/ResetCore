namespace ReFrame.Util.Utility
{
    public abstract class Singleton<T> where T : Singleton<T>, new()
    {

        private static T _Instance = null;

        protected virtual bool autoInit
        {
            get { return true; }
        }

        public static T Instance
        {
            get
            {
                if (_Instance == null)
                {
                    _Instance = new T();
                    if(_Instance.autoInit)
                        _Instance.Init();
                }
                return _Instance;
            }
        }

        public virtual void Init() { }

    }

}
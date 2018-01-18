namespace ReFrame.Util.Utility
{
    public class Reusable
    {
        private static uint hash = 0;
         /// <summary>
         /// 每个类型的Hash
         /// </summary>
        public uint Hash => hash;

        /// <summary>
        /// 是否已经被销毁
        /// </summary>
        public bool Destroyed { get; private set; }

        protected Reusable()
        {
            if (hash == 0)
            {
                hash = HashUtil.GetHash(GetType().Name);
            }
            Destroyed = false;
        }

        protected virtual void OnDestroy()
        {
            Destroyed = true;
        }

        protected virtual void OnReuse()
        {
            Destroyed = false;
        }
        
        /// <summary>
        /// 获得新的
        /// </summary>
        /// <returns></returns>
        public static T Get<T>() where T:Reusable, new()
        {
            var obj = ObjectPool.GetObject<T>(HashUtil.GetHash(nameof(T)));
            obj.OnReuse();
            return obj;
        }
        
        public void Destroy()
        {
            OnDestroy();
            ObjectPool.ReturnObject(this);
        }

    }
}
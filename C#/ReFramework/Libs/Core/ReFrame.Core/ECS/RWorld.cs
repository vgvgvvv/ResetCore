using System.Collections.Generic;
using ReFrame.Util.Utility;

namespace ReFrame.Core
{
    public class RWorld
    {


        public RWorld()
        {
            _managers = new Dictionary<uint, RManager>();
        }

        #region Manager管理

        private Dictionary<uint, RManager> _managers;

        /// <summary>
        /// 添加管理器
        /// </summary>
        /// <param name="manager"></param>
        /// <typeparam name="T"></typeparam>
        public void AddManager<T>(T manager) where T:RManager
        {
            _managers.Add(HashUtil.GetHash(nameof(T)), manager);
        }

        /// <summary>
        /// 获取管理器
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        public T GetManager<T>() where T:RManager
        {
            if (_managers.TryGetValue(HashUtil.GetHash(nameof(T)), out var manager))
            {
                return manager as T;
            }
            return null;
        }

        #endregion
        

        #region 生命周期

        public virtual void Awake()
        {
            AddManager(new REntityManager());
        }

        public virtual void Start()
        {
            Init();
        }

        protected virtual void Init()
        {
            foreach (var keyValuePair in _managers)
            {
                keyValuePair.Value.Init();
            }
        }

        public virtual void Update()
        {
            foreach (var keyValuePair in _managers)
            {
                keyValuePair.Value.Update();
            }
        }

        public virtual void OnDestroy()
        {
            Uninit();
        }

        protected virtual void Uninit()
        {
            foreach (var keyValuePair in _managers)
            {
                keyValuePair.Value.Uninit();
            }
        }

        #endregion
        
        
        
        
    }
}
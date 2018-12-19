using System.Collections.Generic;
using ReFrame.Util.Utility;

namespace ReFrame.Core
{
    public abstract class REntity : RObject
    {
        
        public ulong UID { get; set; }
        
        #region 组件操作

        /// <summary>
        /// 包含所有组件
        /// </summary>
        private List<RComponent> Components { get; set; }
        /// <summary>
        /// 等待被移除
        /// </summary>
        private readonly List<RComponent> delayRemove = new List<RComponent>();

        
        /// <summary>
        /// 添加Component
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        public T AddComponent<T>() where T : RComponent, new()
        {
            var component = RObject.Get<T>();
            Components.Add(component);
            component.Awake();
            return component;
        }

        /// <summary>
        /// 获取Component
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        public T GetComponent<T>() where T:RComponent
        {
            return Components.Find((com) => com is T && com.Destroyed == false) as T;
        }
        

        #endregion
        
        protected REntity()
        {
            Components = new List<RComponent>();
        }

        #region 生命周期

        public virtual void Awake(){}

        protected override void OnReuse()
        {
            base.OnReuse();
            Components.Clear();
            delayRemove.Clear();
            Awake();
        }

        public virtual void Update()
        {
            //移除已被销毁的对象
            for (int i = 0; i < delayRemove.Count; i++)
            {
                Components.Remove(delayRemove[i]);
            }
            delayRemove.Clear();
            for (int i = 0; i < Components.Count; i++)
            {
                if (Components[i].Destroyed)
                    delayRemove.Add(Components[i]);
                else
                    Components[i].Update();
            }
        }
        
        /// <summary>
        /// 销毁对象
        /// </summary>
        protected override void OnDestroy()
        {
            for (int i = 0; i < Components.Count; i++)
            {
                Components[i].Destroy();
            }
            Components.Clear();
            delayRemove.Clear();
            base.OnDestroy();
        }

        #endregion
        
    }
}
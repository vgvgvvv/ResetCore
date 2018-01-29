using System;
using FrameSync.Managers.Ticker;

namespace FrameSync.Managers
{

    public enum TickMode
    {
        Local,
        Server
    } 
    
    /// <summary>
    /// 处理Tick事件
    /// </summary>
    public class TickManager : IManager
    {

        private event Action OnUpdate;
        private event Action OnLateUpdate;
        private event Action OnFixedUpdate;
        
        public TickMode Mode { get; private set; }

        private uint _currentTickCount = 0;
        
        public void Init()
        {
            Mode = TickMode.Server;
        }

        public void Update()
        {
            if (OnUpdate != null) 
                OnUpdate();
        }

        public void LateUpdate()
        {
            if (OnLateUpdate != null) 
                OnLateUpdate();
        }

        public void FixedUpdate()
        {
            if (Mode == TickMode.Local)
            {
                if (OnFixedUpdate != null) 
                    OnFixedUpdate();
            }
        }

        public void UnInit()
        {
            OnUpdate -= OnUpdate;
            OnLateUpdate -= OnLateUpdate;
            OnFixedUpdate -= OnFixedUpdate;
        }

        #region Ticker操作

        public void AddUpdateTick(ITickUpdate ticker)
        {
            OnUpdate += ticker.Update;
        }

        public void AddLateUpdateTick(ITickLateUpdate ticker)
        {
            OnLateUpdate += ticker.LateUpdate;
        }

        public void AddFixedUpdateTick(ITickFixedUpdate ticker)
        {
            OnFixedUpdate += ticker.FixedUpdate;
        }
        
        public void RemoveUpdateTick(ITickUpdate ticker)
        {
            OnUpdate -= ticker.Update;
        }

        public void RemoveLateUpdateTick(ITickLateUpdate ticker)
        {
            OnLateUpdate -= ticker.LateUpdate;
        }

        public void RemoveFixedUpdateTick(ITickFixedUpdate ticker)
        {
            OnFixedUpdate -= ticker.FixedUpdate;
        }
        
        #endregion
        
        public void DriveToTickCount(uint fixedTickCount)
        {
            while (fixedTickCount > _currentTickCount)
            {
                FixedUpdate();
            }
        }
    }
}
namespace FrameSync.Managers
{
    /// <summary>
    /// 处理同步事件
    /// </summary>
    public class GameSyncManager : IManager
    {
        public void Init()
        {
            
        }

        public void Update()
        {
            
        }

        public void LateUpdate()
        {
            
        }

        public void FixedUpdate()
        {
            
        }

        public void UnInit()
        {
            
        }
        
        //TODO Update中检查输入，如果需要更新则发送至服务端
        
        //TODO 监听接收到的消息，如果收到消息则驱动TickManager
    }
}
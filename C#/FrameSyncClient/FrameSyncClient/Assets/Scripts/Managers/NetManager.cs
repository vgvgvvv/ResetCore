using ReFrame.Network;
using ReFrame.Network.Protocol;

namespace FrameSync.Managers
{
    /// <summary>
    /// 处理网络事件
    /// </summary>
    public class NetManager : IManager
    {
        private TcpClient client = new TcpClient();
        
        public void Init()
        {
            
        }

        public void Update()
        {
            while (client.ProtocolQueue.Count > 0)
            {
                var protocol = client.ProtocolQueue.Dequeue();
                protocol.Process();
            }
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

        //TODO 实现消息队列
    }
}
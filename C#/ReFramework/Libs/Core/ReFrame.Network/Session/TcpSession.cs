
using System;

namespace ReFrame.Network
{
    public class TcpSession : IClientSession
    {
        
        public string Address { get; private set; }
        public int Port { get; private set; }

        //tcp同步锁
        private object tcpLockObject = new object();
        
        private TcpSocket _socket = new TcpSocket();
        
        public ISessionPipeline Pipeline { get; private set; }

        public TcpSession()
        {
            Pipeline = new DefaultSessionPipeline(this);
        }
        
        /// <summary>
        /// 连接
        /// </summary>
        /// <param name="address"></param>
        /// <param name="port"></param>
        public void Connect(string address, int port)
        {
            this.Address = address;
            this.Port = port;
            
            _socket.onReceive += TcpOnReceive;
            
            _socket.Connect(Address, Port);
            Pipeline.FireConnect();
        }

        /// <summary>
        /// 发送消息
        /// </summary>
        /// <param name="msg"></param>
        public void Send(object msg)
        {
            Pipeline.FireWrite(msg);
        }

        /// <summary>
        /// 直接发送消息
        /// </summary>
        /// <param name="msg"></param>
        public void RawSend(object msg)
        {
            _socket.Send(msg as byte[]);
        }

        /// <summary>
        /// 处理信息
        /// </summary>
        /// <param name="msg"></param>
        public void HandleMessage(object msg)
        {
            Pipeline.FireRead(msg);
        }

        /// <summary>
        /// 断开连接
        /// </summary>
        public void Disconnect()
        {
            Pipeline.FireDisconnect();
        }
        
        private void TcpOnReceive(int len, byte[] source)
        {
            lock (tcpLockObject)
            {
                //TODO 使用Buffer进行复用
                byte[] target = new byte[len];
                Array.Copy(source, target, len);
                HandleMessage(target);
            }
        }

        
        
    }
}
using System;

namespace ReFrame.Network
{
    public interface ISession
    {
        
        string Address { get; }
        
        int Port { get; }
        
        ISessionPipeline Pipeline { get; }
        
        
        /// <summary>
        /// 连接服务器
        /// </summary>
        /// <param name="adress"></param>
        /// <param name="port"></param>
        void Connect(string adress, int port);
        
        /// <summary>
        /// 发送消息
        /// </summary>
        /// <param name="bytes"></param>
        void Send(object msg);

        /// <summary>
        /// 不经过处理直接发送消息
        /// </summary>
        /// <param name="msg"></param>
        void RawSend(object msg);

        /// <summary>
        /// 处理消息
        /// </summary>
        /// <param name="bytes"></param>
        void HandleMessage(object bytes);
        
        /// <summary>
        /// 断开连接
        /// </summary>
        void Disconnect();
        
    }
}
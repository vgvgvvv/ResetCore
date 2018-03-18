using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using ReFrame.Network.Protocol;

namespace ReFrame.Network
{
    public class TcpClient
    {
        private TcpSocket _socket = new TcpSocket();
        private object tcpLockObject = new object();
        private readonly MemoryStream _sharedSendStream = new MemoryStream();
        private readonly MemoryStream _sharedReciveStream = new MemoryStream();
        
        private const int HeadLength = 4;

        public readonly Queue<IProtocol> ProtocolQueue = new Queue<IProtocol>();
        

        public TcpClient()
        {
            _socket.onConnect += OnConnect;
            _socket.onCloseSocket += OnCloseSocket;
            _socket.onReceive += OnReceive;
            _socket.onSend += OnSend;
            _socket.onError += OnError;
        }

        public void Connect(string address, int port)
        {
            _socket.Connect(address, port);
        }

        public void Send(IProtocol protocol)
        {
            protocol.SerializePackage(_sharedSendStream);
            _socket.Send(_sharedSendStream.GetBuffer(), (int)_sharedSendStream.Length);
        }

        private void Reveive(int len, byte[] indata)
        {
            
            
        }


        #region Socket回调

        private void OnConnect(Exception e = null)
        {
            
        }

        private void OnCloseSocket(CloseType type, SocketState state, Exception e = null)
        {
            
        }
        
        private void OnReceive(int len, byte[] data)
        {
            lock (tcpLockObject)
            {
                Reveive(len, data);
            }
        }

        private void OnSend(int len)
        {
            
        }

        private void OnError(SocketState state, Exception e = null)
        {
            
        }

        #endregion Socket回调
        
        /// <summary>
        /// 获取包长度
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        private int GetDataLength(byte[] data)
        {
            if (data.Length < HeadLength)
                return 0;

            byte[] lengthByte = data.SubArray(0, 4);
            if (!BitConverter.IsLittleEndian)
                Array.Reverse(lengthByte);
            int length = BitConverter.ToInt32(lengthByte, 0);

            return length;
        }
        
    }
}
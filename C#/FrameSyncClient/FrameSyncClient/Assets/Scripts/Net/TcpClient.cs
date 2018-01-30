using System;
using System.Collections;
using System.Collections.Generic;
using ReFrame.Network.Protocol;

namespace ReFrame.Network
{
    public class TcpClient
    {
        private TcpSocket _socket = new TcpSocket();
        private object tcpLockObject = new object();
        private byte[] dataBuffer = new byte[1024];
        
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

        public void Send(byte[] bytes)
        {
            _socket.Send(bytes);
        }

        private void Reveive(int len, byte[] data)
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
        
    }
}
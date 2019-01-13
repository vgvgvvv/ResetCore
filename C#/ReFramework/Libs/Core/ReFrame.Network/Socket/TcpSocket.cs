using System.Collections;
using System.Net.Sockets;
using System;
using System.Diagnostics;
using System.Net;
using ReFrame.Util.Context;
using ReFrame.Util.Debug;

namespace ReFrame.Network {
    public class TcpSocket {

        private static readonly IDebuger logger = ContextValue<IDebuger>.Value;

        /// <summary>
        ///远端端口
        /// </summary>
        public int remotePort { get; private set; }
        /// <summary>
        /// 远端地址
        /// </summary>
        public string remoteAddress { get; private set; }
        /// <summary>
        /// 本地端口
        /// </summary>
        public int localPort { get; private set; }
        /// <summary>
        /// 本地地址
        /// </summary>
        public string localAddress { get; private set; }
        /// <summary>
        /// Socket变量
        /// </summary>
        private Socket socket;

        /// <summary>
        /// 是否为主动关闭
        /// </summary>
        private bool isCloseSelf = false;

        /// <summary>
        /// 接收数据缓冲区
        /// </summary>
        private byte[] receiveBuffer = new byte[1024 * 5];

        //Socket锁
        private readonly object locker = new object();

        /// <summary>
        /// 是否连接
        /// </summary>
        public bool isConneted {
            get {
                if (socket == null || !socket.Connected) {
                    return false;
                } else {
                    return true;
                }
            }
        }

        

        /// <summary>
        /// 当前Socket状态
        /// </summary>
        public SocketState currentState { get; private set; }

        #region 公开的回调函数

        public TcpSocketListenDelegate onListen { get; set; }
        public TcpSocketConnectDelegate onConnect { get; set; }
        public TcpSocketCloseSocketDelegate onCloseSocket { get; set; }
        public TcpSocketErrorDelegate onError { get; set; }
        public TcpSocketReceiveDelegate onReceive { get; set; }
        public TcpSocketSendDelegate onSend { get; set; }

        #endregion

        public TcpSocket(Socket socket = null)
        {

            acceptCallback = new AsyncCallback(OnAccept);
            connectCallback = new AsyncCallback(OnConnect);
            disconnectCallback = new AsyncCallback(OnDisconnect);
            sendCallback = new AsyncCallback(OnSend);
            receiveCallback = new AsyncCallback(OnReceive);

            if (socket != null) {
                this.socket = socket;

                //远端地址
                IPEndPoint remote = socket.RemoteEndPoint as IPEndPoint;
                remoteAddress = remote.Address.ToString();
                remotePort = remote.Port;

                //本地地址
                IPEndPoint local = socket.LocalEndPoint as IPEndPoint;
                localAddress = local.Address.ToString();
                localPort = local.Port;
            }
        }


        #region 公开方法用于执行各种Socket行为
        /// <summary>
        /// 开始监听等待接收到新信息的回调
        /// </summary>
        public bool BeginListen(int port, int queueSize = 1000) {
            currentState = SocketState.BEGIN_LISTEN;

            localAddress = "127.0.0.1";
            localPort = port;

            socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            try {
                lock (locker) {
                    socket.Bind(new IPEndPoint(IPAddress.Any, localPort));
                    socket.Listen(queueSize);
                    socket.BeginAccept(acceptCallback, null);
                }
            } catch (Exception exp) {
                CallError(exp);
                return false;
            }

            if (onListen != null)
                onListen();

            return true;
        }

        /// <summary>
        /// 停止监听
        /// </summary>
        public void StopListen() {

            currentState = SocketState.END_LISTEN;
            //设置主动关闭标志位
            isCloseSelf = true;

            try {
                //停止监听
                if (socket != null) {
                    lock (locker) {
                        socket.Close();
                    }
                }
            } catch (Exception exp) {
                //引发报错事件
                CallError(exp);
            }
        }

        /// <summary>
        /// 连接等待成功连接的回调
        /// </summary>
        public bool Connect(string address, int port) {
            currentState = SocketState.BEGIN_CONNECT;
            isCloseSelf = false;

            remoteAddress = address;
            remotePort = port;

            socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try {
                //尝试连接
                lock (locker) {
                    socket.BeginConnect(remoteAddress, remotePort, connectCallback, null);
                }
            } catch (Exception e) {
                CallError(e);
                return false;
            }
            return true;
        }

        /// <summary>
        /// 断开连接等待断开成功的回调
        /// </summary>
        public void Disconnect() {
            //判断是否已连接
            if (!isConneted) {
                logger.LogError("NetPost", "TCP已经断开！");
                return;
            }

            currentState = SocketState.BEGIN_DISCONNECT;
            //设置主动关闭位
            isCloseSelf = true;

            try {
                //开始断开连接
                lock (locker) {
                    socket.BeginDisconnect(false, disconnectCallback, null);
                }
            } catch (Exception exp) {
                //引发报错事件
                CallError(exp);
            }
        }

        /// <summary>
        /// 开始接受消息等待回调
        /// </summary>
        public void BeginReceive() {
            currentState = SocketState.BEGIN_RECEIVE;
            try {
                //开始接收数据
                lock (locker) {
                    socket.BeginReceive(receiveBuffer, 0, receiveBuffer.Length, 0, receiveCallback, null);
                }
            } catch (Exception exp) {
                //引发报错事件
                CallError(exp);
            }
        }

        /// <summary>
        /// 发送消息并等待发送成功的回调
        /// </summary>
        public void Send(byte[] data) {
            currentState = SocketState.BEGIN_SEND;
            try {
                //开始接收数据
                lock (locker) {
                    socket.BeginSend(data, 0, data.Length, 0, sendCallback, null);
                }
            } catch (Exception exp) {
                //引发报错事件
                CallError(exp);
            }
        }
        #endregion//公开方法

        #region 回调函数
        /// <summary>
        /// 当接收完成时回调
        /// </summary>
        private AsyncCallback acceptCallback;
        private void OnAccept(IAsyncResult iar) {
            Socket client;
            currentState = SocketState.ACCEPT;
            try {
                //获取客户端套接字
                client = socket.EndAccept(iar);
                //继续等待连接
                socket.BeginAccept(acceptCallback, null);
            } catch (Exception e) {
                CallError(e);
            }

            if (onConnect != null)
                onConnect();


        }
        /// <summary>
        /// 当连接完成时回调
        /// </summary>
        private AsyncCallback connectCallback;
        private void OnConnect(IAsyncResult iar) {
            currentState = SocketState.END_CONNECT;

            try {
                //结束连接
                socket.EndConnect(iar);

                //读取本地地址
                IPEndPoint local = socket.LocalEndPoint as IPEndPoint;
                localAddress = local.Address.ToString();
                localPort = local.Port;
            } catch (Exception exp) {
                //引发报错事件
                CallError(exp);
                return;
            }

            //引发连接成功事件
            if (onConnect != null)
                onConnect();
        }

        /// <summary>
        /// 当断开连接时回调
        /// </summary>
        private AsyncCallback disconnectCallback;
        private void OnDisconnect(IAsyncResult iar) {
            currentState = SocketState.END_DISCONNECT;

            try {
                //结束断开连接
                socket.EndDisconnect(iar);
            } catch (Exception exp) {
                //引发报错事件
                CallError(exp);
                CloseSocket(CloseType.MY_POSSITIVE, exp);

                return;
            }

            CloseSocket(CloseType.MY_POSSITIVE);

        }
        /// <summary>
        /// 当发送完成时回调
        /// </summary>
        private AsyncCallback sendCallback;
        private void OnSend(IAsyncResult iar) {
            //发送的数据长度
            int len = 0;
            currentState = SocketState.END_SEND;

            try {
                //结束发送
                len = socket.EndSend(iar);
            } catch (Exception exp) {
                //关闭套接字
                CloseSocket(CloseType.THEIR_PASSIVE, exp);

                //引发报错事件
                CallError(exp);

                return;
            }

            //引发发送数据事件
            if (onSend != null)
                onSend(len);
        }
        /// <summary>
        /// 当接收信息时回调
        /// </summary>
        private AsyncCallback receiveCallback;
        private void OnReceive(IAsyncResult iar) {
            //接收到的数据长度
            int len;
            currentState = SocketState.END_RECEIVE;
            try {
                //结束接收
                len = socket.EndReceive(iar);

                //接收到零长度数据处理
                if (len == 0) {
                    //如果是主动关闭
                    if (isCloseSelf)
                        return;

                    CloseSocket(CloseType.THEIR_POSSITIVE);

                    return;
                }
            } catch (Exception exp) {
                //如果是主动关闭
                if (isCloseSelf)
                    return;

                CloseSocket(CloseType.THEIR_POSSITIVE, exp);

                //引发报错事件
                CallError(exp);

                return;
            }

            //引发接收数据事件
            if (onReceive != null)
                onReceive(len, receiveBuffer);

            //继续接收
            BeginReceive();
        }

        /// <summary>
        /// 关闭Socket回调
        /// </summary>
        /// <param name="type"></param>
        /// <param name="state"></param>
        /// <param name="socketCode"></param>
        /// <param name="socketMessage"></param>
        private void CloseSocket(CloseType type, Exception e = null) {
            //关闭套接字
            socket.Close();
            logger.Log("断开TCP成功！");
            //引发断线事件
            if (onCloseSocket != null)
                onCloseSocket(type, currentState, e);
        }

        /// <summary>
        /// 错误回调
        /// </summary>
        /// <param name="state"></param>
        /// <param name="socketCode"></param>
        /// <param name="socketMessage"></param>
        private void CallError(Exception e) {
            if (e is SocketException) {
                logger.LogException(e);
                logger.LogError("SocketError", ((SocketException)e).SocketErrorCode.ToString());
            }
            if (onError != null)
                onError(currentState, e);
        }


        #endregion//回调函数


    }

}

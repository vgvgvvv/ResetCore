using System.Net.Sockets;
using System;
using System.Diagnostics;
using System.Net;
using ReFrame.Util.Context;
using ReFrame.Util.Debug;

namespace ReFrame.Network
{
    public class UdpSocket
    {
        private static readonly IDebuger logger = ContextValue<IDebuger>.Value;

        public string remoteAddress { get; private set; }
        public int remotePort { get; private set; }
        public string localAddress { get; private set; }
        public int localPort { get; private set; }

        private Socket socket;

        private AsyncCallback receiveCallback;
        private AsyncCallback sendCallback;

        //各类回调事件回调
        public UdpSocketListenDelegate onListen { get; set; }
        public UdpSocketBindDelegate onBind { get; set; }
        public UdpSocketReceiveDelegate onReceive { get; set; }
        public UdpSocketErrorDelegate onError { get; set; }

        //接收缓存
        private byte[] receiveBuffer = new byte[1024 * 1024];

        //广播地址
        private EndPoint endpointAny = new IPEndPoint(IPAddress.Any, 0);
        //远端地址
        private EndPoint remoteEndpoint;

        /// <summary>
        /// 当前Socket状态
        /// </summary>
        public SocketState currentState { get; private set; }

        private bool isClose = true;

        public UdpSocket()
        {
            receiveCallback = new AsyncCallback(OnReceive);
            sendCallback = new AsyncCallback(OnSend);
        }

        //开始监听
        public bool StartListen(int localPt)
        {

            currentState = SocketState.BEGIN_LISTEN;
            localPort = localPt;

            try
            {
                socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                socket.Bind(new IPEndPoint(IPAddress.Any, localPort));
                socket.BeginReceiveFrom(receiveBuffer, 0, receiveBuffer.Length, 0, ref endpointAny, receiveCallback, null);
            }
            catch (Exception exp)
            {
                //引发监听失败事件
                CallError(exp);

                return false;
            }

            //引发监听成功事件
            if (onListen != null)
                onListen();

            return true;

        }

        //绑定到本地接口
        public bool BindLocalEndPoint(int localPt)
        {
            isClose = false;
            currentState = SocketState.BIND;
            try
            {
                //绑定本地端口
                socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                socket.Bind(new IPEndPoint(IPAddress.Any, localPt));

                //读取本地地址和端口
                IPEndPoint local = socket.LocalEndPoint as IPEndPoint;
                localAddress = local.Address.ToString();
                localPort = local.Port;
            }
            catch (Exception exp)
            {
                //引发绑定失败事件
                CallError(exp);

                return false;
            }

            //引发绑定成功事件
            if (onBind != null)
                onBind();

            return true;
        }

        //绑定到远端地址
        public bool BindRemoteEndPoint(string remoteAddr, int remotePt, 
            int localPt, bool autoRebind = true)
        {
            isClose = false;
            currentState = SocketState.BIND;
            //保存地址
            SaveRemoteAddress(remoteAddr, remotePt);
            localPort = localPt;

            try
            {
                socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

                //判断是否绑定本地端口
                if (localPort != 0)
                {
                    if (autoRebind)
                        RebindLocalPort(localPt);
                    else
                    {
                        //绑定本地端口
                        socket.Bind(new IPEndPoint(IPAddress.Any, localPt));
                        
                    }

                    //读取本地地址和端口
                    IPEndPoint local = socket.LocalEndPoint as IPEndPoint;
                    localAddress = local.Address.ToString();
                    localPort = local.Port;
                }
            }
            catch (Exception exp)
            {
                //引发绑定失败事件
                CallError(exp);

                return false;
            }

            //引发绑定成功事件
            if (onBind != null)
                onBind();

            return true;
        }

        //停止连接
        public void Stop()
        {
            currentState = SocketState.CLOSE;
            try
            {
                //关闭套接字
                socket.Close();
                isClose = true;
                logger.Log("断开UDP成功！");
            }
            catch (Exception exp)
            {
                //引发错误事件
                CallError(exp);
            }
        }

        //开始接收
        public bool BeginReceive()
        {
            if (isClose)
                return false;

            currentState = SocketState.BEGIN_RECEIVE;
            try
            {
                //开始接收
                socket.BeginReceiveFrom(receiveBuffer, 0, receiveBuffer.Length, 0, ref endpointAny, receiveCallback, null);

                return true;
            }

            catch (Exception exp)
            {
                //引发错误事件
                CallError(exp);

                return false;
            }
        }

        //向远端发送消息
        public void Send(byte[] data, int len)
        {
            if (isClose)
                return;
            currentState = SocketState.BEGIN_SEND;
            try
            {
                //开始发送
                socket.BeginSendTo(data, 0, len, 0, remoteEndpoint, sendCallback, null);
            }
            catch (Exception exp)
            {
                //引发错误事件
                CallError(exp);
            }
        }

        //向远端发送消息
        public void Send(byte[] data, int len, string remoteAddr, int remotePt)
        {
            if (isClose)
                return;

            currentState = SocketState.BEGIN_SEND;

            //保存远端地址
            SaveRemoteAddress(remoteAddr, remotePt);

            try
            {
                //开始发送
                socket.BeginSendTo(data, 0, len, 0, remoteEndpoint, sendCallback, null);
            }
            catch (Exception exp)
            {
                //引发错误事件
                CallError(exp);
            }
        }

        //重新绑定接口
        private void RebindLocalPort(int pt)
        {
            int port = pt;

            bool success = false;
            while (!success)
            {
                try
                {
                    socket.Bind(new IPEndPoint(IPAddress.Any, port++));
                    success = true;
                }
                catch { }
            }
        }

        private void OnReceive(IAsyncResult iar)
        {
            if (isClose)
                return;

            currentState = SocketState.END_RECEIVE ;

            //发来数据的远端地址
            EndPoint remote = new IPEndPoint(0, 0);
            int len = 0;

            try
            {
                //结束接收
                len = socket.EndReceiveFrom(iar, ref remote);
            }
            catch (Exception exp)
            {
                //引发错误事件
                CallError(exp);
                return;
            }

            //引发接收数据事件
            if (onReceive != null)
                onReceive(len, receiveBuffer, ((IPEndPoint)remote).Address.ToString(), ((IPEndPoint)remote).Port);

            //继续接收
            BeginReceive();
        }

        private void OnSend(IAsyncResult iar)
        {
            if (isClose)
                return;

            currentState = SocketState.END_SEND;
            try
            {
                //结束发送
                socket.EndSendTo(iar);
            }
            catch (Exception exp)
            {
                //引发错误事件
                CallError(exp);
            }
        }

        /// <summary>
        /// 错误回调
        /// </summary>
        /// <param name="state"></param>
        /// <param name="socketCode"></param>
        /// <param name="socketMessage"></param>
        private void CallError(Exception e)
        {
            if (e is SocketException)
            {
                logger.LogException(e);
                logger.LogError("SocketError", ((SocketException)e).SocketErrorCode.ToString());
            }
            if (onError != null)
                onError(currentState, e);
        }

        //保存远端地址
        private void SaveRemoteAddress(string address, int port)
        {
            //保存地址
            remoteAddress = address;
            remotePort = port;

            //生成端点
            remoteEndpoint = new IPEndPoint(IPAddress.Parse(remoteAddress), remotePort);
        }
    }
}

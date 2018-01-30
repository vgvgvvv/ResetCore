using System.Net.Sockets;
using System;

namespace ReFrame.Network 
{
    #region Tcp套接字代理
    /// <summary>
    /// Tcp套接字监听代理
    /// </summary>
    /// <param name="code">错误码</param>
    /// <param name="socketCode">套接字错误码</param>
    /// <param name="socketMessage">套接字错误信息</param>
    public delegate void TcpSocketListenDelegate(Exception e = null);

    /// <summary>
    /// Tcp套接字客户端连接代理
    /// </summary>
    /// <param name="socket">客户端套接字</param>
    public delegate void TcpSocketClientConnectedDelegate(Socket socket);

    /// <summary>
    /// Tcp套接字连接代理
    /// </summary>
    /// <param name="code">错误码</param>
    /// <param name="socketCode">套接字错误码</param>
    /// <param name="socketMessage">套接字错误信息</param>
    public delegate void TcpSocketConnectDelegate(Exception e = null);

    /// <summary>
    /// Tcp套接字断开连接代理
    /// </summary>
    /// <param name="type">断开连接类型</param>
    /// <param name="phase">套接字阶段</param>
    /// <param name="socketCode">套接字错误码</param>
    /// <param name="socketMessage">套接字错误信息</param>
    public delegate void TcpSocketCloseSocketDelegate(CloseType type, SocketState state, Exception e = null);

    /// <summary>
    /// Tcp套接字接收到数据代理
    /// </summary>
    /// <param name="len">数据长度</param>
    /// <param name="data">数据内容</param>
    public delegate void TcpSocketReceiveDelegate(int len, byte[] data);

    /// <summary>
    /// Tcp套接字发送数据代理
    /// </summary>
    /// <param name="len">数据长度</param>
    public delegate void TcpSocketSendDelegate(int len);

    /// <summary>
    /// Tcp套接字发生错误代理
    /// </summary>
    /// <param name="phase">套接字阶段</param>
    /// <param name="socketCode">套接字错误码</param>
    /// <param name="socketMessage">套接字错误信息</param>
    public delegate void TcpSocketErrorDelegate(SocketState state, Exception e = null);
    #endregion//Tcp套接字代理

    #region Udp套接字代理
    /// <summary>
    /// Udp套接字监听代理
    /// </summary>
    /// <param name="code">错误码</param>
    /// <param name="socketCode">套接字错误码</param>
    /// <param name="socketMessage">套接字错误信息</param>
    public delegate void UdpSocketListenDelegate(Exception e = null);

    /// <summary>
    /// Udp套接字端口绑定代理
    /// </summary>
    /// <param name="code">错误码</param>
    /// <param name="socketCode">套接字错误码</param>
    /// <param name="socketMessage">套接字错误信息</param>
    public delegate void UdpSocketBindDelegate(Exception e = null);

    /// <summary>
    /// Udp套接字接收到数据代理
    /// </summary>
    /// <param name="len">数据长度</param>
    /// <param name="data">数据内容</param>
    /// <param name="remoteAddress">远端地址</param>
    /// <param name="remotePort">远端端口</param>
    public delegate void UdpSocketReceiveDelegate(int len, byte[] data, string remoteAddress, int remotePort);

    /// <summary>
    /// Udp套接字发生错误代理
    /// </summary>
    /// <param name="phase">套接字活动阶段</param>
    /// <param name="socketCode">套接字错误码</param>
    /// <param name="socketMessage">套接字错误信息</param>
    public delegate void UdpSocketErrorDelegate(SocketState state, Exception e = null);
    #endregion //Udp套接字代理
}

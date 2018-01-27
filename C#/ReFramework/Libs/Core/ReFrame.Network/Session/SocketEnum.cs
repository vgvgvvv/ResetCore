
namespace ReFrame.Network
{
    public enum SocketState
    {
        /// <summary>
        /// 未知
        /// </summary>
        UNKNOWN = 0,

        /// <summary>
        /// 绑定端口
        /// </summary>
        BIND,

        /// <summary>
        /// 开始监听
        /// </summary>
        BEGIN_LISTEN,

        /// <summary>
        /// 结束监听
        /// </summary>
        END_LISTEN,

        /// <summary>
        /// 接收客户端连接
        /// </summary>
        ACCEPT,

        /// <summary>
        /// 开始连接
        /// </summary>
        BEGIN_CONNECT,

        /// <summary>
        /// 结束连接
        /// </summary>
        END_CONNECT,

        /// <summary>
        /// 开始发送数据
        /// </summary>
        BEGIN_SEND,

        /// <summary>
        /// 结束发送数据
        /// </summary>
        END_SEND,

        /// <summary>
        /// 开始接收数据
        /// </summary>
        BEGIN_RECEIVE,

        /// <summary>
        /// 结束接收数据
        /// </summary>
        END_RECEIVE,

        /// <summary>
        /// 开始断开连接
        /// </summary>
        BEGIN_DISCONNECT,

        /// <summary>
        /// 结束断开连接
        /// </summary>
        END_DISCONNECT,

        /// <summary>
        /// 关闭
        /// </summary>
        CLOSE
    }

    /// <summary>
    /// 关闭类型
    /// </summary>
    public enum CloseType
    {
        /// <summary>
        /// 未知
        /// </summary>
        UNKOWN,

        /// <summary>
        /// 我方主动断开
        /// </summary>
        MY_POSSITIVE,

        /// <summary>
        /// 我方被动断开
        /// </summary>
        MY_PASSIVE,

        /// <summary>
        /// 对方主动断开
        /// </summary>
        THEIR_POSSITIVE,

        /// <summary>
        /// 对方被动断开
        /// </summary>
        THEIR_PASSIVE
    }

    public enum NetType
    {
        TCP,
        UDP
    }
}

using System;

namespace ReFrame.Network
{
    public interface ISessionInboundHandler : ISessionHandler
    {
        void SessionConnected(ISessionContext next);
        void SessionDisconnect(ISessionContext next);
        void SessionRead(ISessionContext next, object msg);
        void ExceptionCaught(ISessionContext next, Exception e);
    }
}
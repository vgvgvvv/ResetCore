namespace ReFrame.Network
{
    public interface ISessionOutboundHandler : ISessionHandler
    {
        void SessionWrite(ISessionContext next, object msg);
    }
}
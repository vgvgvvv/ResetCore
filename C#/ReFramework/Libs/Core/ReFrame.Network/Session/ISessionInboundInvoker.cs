namespace ReFrame.Network
{
    public interface ISessionInboundInvoker
    {
        void FireRead(object msg);
    }
}
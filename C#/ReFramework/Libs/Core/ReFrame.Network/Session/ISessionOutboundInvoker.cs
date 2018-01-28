namespace ReFrame.Network
{
    public interface ISessionOutboundInvoker
    {
        void FireConnect();

        void FireDisconnect();
        
        void FireWrite(object msg);
    }
}
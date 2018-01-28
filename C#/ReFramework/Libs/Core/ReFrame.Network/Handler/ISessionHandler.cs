namespace ReFrame.Network
{
    public interface ISessionHandler
    {
        void HandlerAdded(ISessionContext context);

        void HandlerRemoved(ISessionContext context);
    }
}
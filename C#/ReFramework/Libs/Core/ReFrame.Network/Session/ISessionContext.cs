namespace ReFrame.Network
{
    public interface ISessionContext : ISessionInboundInvoker, ISessionOutboundInvoker
    {
        
        ISessionHandler Handler { get; }
        
        ISessionPipeline Pipeline { get; }
        
    }
}
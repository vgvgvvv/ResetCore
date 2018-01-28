namespace ReFrame.Network
{
    public interface ISessionPipeline : ISessionInboundInvoker, ISessionOutboundInvoker
    {

        ISession Session { get; }
        
        ISessionPipeline AddInbound(string name, ISessionInboundHandler handler);

        ISessionPipeline AddOutbound(string name, ISessionOutboundHandler handler);

    }
}
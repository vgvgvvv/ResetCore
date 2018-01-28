namespace ReFrame.Network
{
    public class DefaultSessionContext : AbstractSessionContext
    {
        public string Name { get;  }
        
        public DefaultSessionContext(string name, ISessionPipeline pipeline, ISessionHandler handler) : base(pipeline)
        {
            this.Name = name;
            this.Handler = handler;
            if (Handler != null)
            {
                this.IsInbound = Handler is ISessionInboundHandler;
                this.IsOutbound = Handler is ISessionOutboundHandler;
            }
        }

        public override void FireConnect()
        {
            InvokeConnect(this); 
        }

        public override void FireDisconnect()
        {
            InvokeDisconnect(this); 
        }

        public override void FireWrite(object msg)
        {
            InvokeWrite(this, msg); 
        }

        public override void FireRead(object msg)
        {
            InvokeRead(this, msg); 
        }
        
    }
}
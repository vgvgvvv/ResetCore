using System;
using System.Collections.Generic;

namespace ReFrame.Network
{
    public abstract class AbstractSessionContext : ISessionContext
    {
        public ISession Session { get; protected set; }
        public ISessionHandler Handler { get; protected set; }
        public ISessionPipeline Pipeline { get; protected set; }
        
        public AbstractSessionContext Next { get; set; }
        public AbstractSessionContext Prev { get; set; }
        
        public bool IsInbound { get; protected set; }
        public bool IsOutbound { get; protected set; }

        internal AbstractSessionContext(ISessionPipeline pipeline)
        {
            this.Session = pipeline.Session;
            this.Pipeline = pipeline;
        }

        public abstract void FireConnect();
        
        public abstract void FireDisconnect();

        public abstract void FireRead(object msg);

        public abstract void FireWrite(object msg);


        public static void InvokeConnect(AbstractSessionContext cxt)
        {
            ((ISessionInboundHandler)cxt.Handler).SessionConnected(cxt.Next);
        }

        public static void InvokeDisconnect(AbstractSessionContext cxt)
        {
            ((ISessionInboundHandler)cxt.Handler).SessionDisconnect(cxt.Next);
        }

        public static void InvokeRead(AbstractSessionContext cxt, object msg)
        {
            ((ISessionInboundHandler)cxt.Handler).SessionRead(cxt.Next, msg);
        }

        public static void InvokeWrite(AbstractSessionContext cxt, object msg)
        {
            ((ISessionOutboundHandler)cxt.Handler).SessionWrite(cxt.Next, msg);
        }
        
       
        
    }
}
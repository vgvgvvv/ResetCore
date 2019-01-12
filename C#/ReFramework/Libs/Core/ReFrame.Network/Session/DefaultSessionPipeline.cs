using System;
using System.Collections.Generic;
using ReFrame.Util.Context;
using ReFrame.Util.Debug;
using ResetCore.Util;

namespace ReFrame.Network
{
    public class DefaultSessionPipeline : ISessionPipeline
    {
        private static IDebuger logger = ContextValue<IDebuger>.Value;
        
        public ISession Session { get; }
        

        private readonly LinkedList<AbstractSessionContext> _inboundList = new LinkedList<AbstractSessionContext>();
        private readonly LinkedList<AbstractSessionContext> _outboundList = new LinkedList<AbstractSessionContext>();

        internal DefaultSessionPipeline(ISession session)
        {
            Session = session;
        }

        
        public ISessionPipeline AddInbound(string name, ISessionInboundHandler handler)
        {
            var context = new DefaultSessionContext(name, this, handler);
            if (_inboundList.Count > 0)
            {
                _inboundList.Last.Value.Next = context;
                context.Prev = _inboundList.Last.Value;
            }
            _inboundList.AddLast(context);
            handler.HandlerAdded(context);
            return this;
        }

        public ISessionPipeline AddOutbound(string name, ISessionOutboundHandler handler)
        {
            var context = new DefaultSessionContext(name, this, handler);
            if (_outboundList.Count > 0)
            {
                _outboundList.Last.Value.Next = context;
                context.Prev = _outboundList.Last.Value;
            }
            _outboundList.AddLast(context);
            handler.HandlerAdded(context);
            return this;
        }

        public void FireRead(object msg)
        {
            if (_inboundList.Count == 0)
                return;
            AbstractSessionContext.InvokeRead(_inboundList.First.Value, msg); 
        }

        public void FireConnect()
        {
            if (_inboundList.Count == 0)
                return;
            AbstractSessionContext.InvokeConnect(_inboundList.First.Value); 
        }

        public void FireDisconnect()
        {
            if (_inboundList.Count == 0)
                return;
            AbstractSessionContext.InvokeDisconnect(_inboundList.First.Value); 
        }

        public void FireWrite(object msg)
        {
            if (_outboundList.Count == 0)
                return;
            AbstractSessionContext.InvokeWrite(_outboundList.First.Value, msg); 
        }
    }

    
}
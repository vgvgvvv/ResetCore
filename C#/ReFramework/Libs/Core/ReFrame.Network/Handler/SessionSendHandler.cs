using System;

namespace ReFrame.Network
{
    public class SessionSendHandler : ISessionOutboundHandler
    {

        private ISession _session;
        public SessionSendHandler(ISession session)
        {
            this._session = session;
        }
        
        public void SessionWrite(ISessionContext next, object msg)
        {
            _session.RawSend(msg);
        }


        public void HandlerAdded(ISessionContext context)
        {
        }

        public void HandlerRemoved(ISessionContext context)
        {
        }
    }
}
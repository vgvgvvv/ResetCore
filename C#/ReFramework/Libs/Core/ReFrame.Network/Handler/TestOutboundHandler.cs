using System;

namespace ReFrame.Network
{
    public class TestOutboundHandler : ISessionOutboundHandler
    {
        public void HandlerAdded(ISessionContext context)
        {
        }

        public void HandlerRemoved(ISessionContext context)
        {
        }

        public void SessionWrite(ISessionContext next, object msg)
        {
            var bytes = BitConverter.GetBytes((int)msg);
            next.FireWrite(bytes);
        }
    }
}
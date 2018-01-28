using System;

namespace ReFrame.Network
{
    public class DefualtSessionFactory : ISessionFactory
    {
        public ISession GetSession(Type type)
        {
            return Activator.CreateInstance(type) as ISession;
        }
    }
}
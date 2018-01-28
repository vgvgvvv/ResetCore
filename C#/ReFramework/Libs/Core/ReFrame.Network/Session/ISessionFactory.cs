using System;

namespace ReFrame.Network
{
    public interface ISessionFactory
    {
        ISession GetSession(Type type);
    }
}
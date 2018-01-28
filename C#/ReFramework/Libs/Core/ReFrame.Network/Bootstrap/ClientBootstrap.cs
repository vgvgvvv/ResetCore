using System;
using System.Net.Sockets;

namespace ReFrame.Network
{
    public class ClientBootstrap : Bootstrap<ClientBootstrap, IClientSession>
    {
        public string Address { get; private set; }
        
        public int Port { get; private set; }
        
        
        private ISessionFactory _sessionFactory;
        private Type _sessionType;
        private Action<ISession> _initAction;

        public ClientBootstrap SessionFactory<F>() where F : ISessionFactory, new()
        {
            _sessionFactory = new F();
            return this;
        }
        
        public ClientBootstrap Session<S>() where S : IClientSession
        {
            _sessionType = typeof(S);
            return this;
        }
        
        public ClientBootstrap AddressAndPort(string address, int port)
        {
            this.Address = address;
            this.Port = port;
            return this;
        }

        public ClientBootstrap Init(Action<ISession> initAction)
        {
            _initAction = initAction;
            return this;
        }
        
        public ISession Connect()
        {
            _sessionFactory = _sessionFactory ?? new DefualtSessionFactory();
            var _session = _sessionFactory.GetSession(_sessionType);
            _initAction(_session);
            _session.Pipeline.AddOutbound("SessionSend", new SessionSendHandler(_session));
            _session.Connect(Address, Port);
            return _session;
        }

    }
}
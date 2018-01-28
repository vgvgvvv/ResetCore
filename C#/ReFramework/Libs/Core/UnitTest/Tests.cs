using System;
using System.Threading;
using NUnit.Framework;
using NUnit.Framework.Internal;
using ReFrame.Network;

namespace UnitTest
{
    [TestFixture]
    public class Tests
    {
        [Test]
        public void TestClient()
        {
            var bootstrap = new ClientBootstrap();
            var session = bootstrap.AddressAndPort("127.0.0.1", 8888)
                .Session<TcpSession>()
                .Init(_session =>
                {
                    _session.Pipeline.AddOutbound("Test", new TestOutboundHandler());
                })
                .Connect();

            Console.WriteLine(1000);
            session.Send(1000);
        }
    }
}
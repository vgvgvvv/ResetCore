using System.IO;

namespace ReFrame.Network.Protocol
{
    public class TestProtocol : IProtocol
    {
        public override ushort MessageId { get; set; }
        public override void Process()
        {
            throw new System.NotImplementedException();
        }

        public override void Serialize(MemoryStream stream)
        {
            throw new System.NotImplementedException();
        }

        public override void DeSerialize(MemoryStream stream)
        {
            throw new System.NotImplementedException();
        }
    }
}
namespace ReFrame.Network.Protocol
{
    public class ProtocolContext
    {
        public enum ProtocolEnum
        {
            Test, 
        }
        
        public static void Regist()
        {
            ProtocolFactory<TestProtocol>.RegistFactory((ushort)ProtocolEnum.Test);
        }
    }
}
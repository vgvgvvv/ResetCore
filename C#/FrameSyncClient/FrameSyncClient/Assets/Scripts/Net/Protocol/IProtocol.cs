using System;
using System.Collections.Generic;
using System.IO;
using NUnit.Framework;
using ResetCore.Util;

namespace ReFrame.Network.Protocol
{
    public class ProtocolHead
    {
        /// <summary>
        /// 包长度，包含自身长度以及信息id长度
        /// </summary>
        public uint length;
        /// <summary>
        /// 信息Id
        /// </summary>
        public ushort messageId;

        public void Serialize(MemoryStream stream)
        {
            stream.Write(BitConverter.GetBytes(length), 0, sizeof(uint));
            stream.Write(BitConverter.GetBytes(messageId), 0, sizeof(ushort));
        }
        
    }
    
    public abstract class IProtocol
    {
        /// <summary>
        /// MessageId
        /// </summary>
        public abstract ushort MessageId { get; set; }

        /// <summary>
        /// 处理
        /// </summary>
        public abstract void Process();

        /// <summary>
        /// 序列化
        /// </summary>
        /// <param name="stream"></param>
        public abstract void Serialize(MemoryStream stream);

        /// <summary>
        /// 反序列化
        /// </summary>
        /// <param name="stream"></param>
        public abstract void DeSerialize(MemoryStream stream);

        
        /// <summary>
        /// 序列化为Package
        /// </summary>
        /// <param name="stream"></param>
        public void SerializePackage(MemoryStream stream)
        {
            stream.SetLength(0);
            stream.Position = 0;

            var begin = stream.Position;
            var head = CommonObjectPool<ProtocolHead>.Get();
            head.length = 0;
            head.messageId = MessageId;
            head.Serialize(stream);
            
            Serialize(stream);
            long position = stream.Position;
            int length = (int)(position - begin);
            
            //重写长度头
            stream.Position = begin;
            stream.Write(BitConverter.GetBytes(length), 0, 4);
            stream.Position = position;
        }
        
    }

    public class ProtoFacotoryPool : Singleton<ProtoFacotoryPool>
    {
        private Dictionary<int, ProtocolFactory> _pool = new BetterDictionary<int, ProtocolFactory>();

        public void Regist(ushort id, ProtocolFactory factory)
        {
            _pool.Add(id, factory);
        }

        public ProtocolFactory Get(ushort id)
        {
            ProtocolFactory factory;
            _pool.TryGetValue(id, out factory);
            return factory;
        }
        
    }
    
    public abstract class ProtocolFactory
    {
        public abstract IProtocol GetProtocol();

        public IProtocol GetProtocol(MemoryStream stream)
        {
            var head = CommonObjectPool<ProtocolHead>.Get();
        }
    }
    
    public class ProtocolFactory<T> : ProtocolFactory where T : IProtocol, new()
    {

        private ushort messageId;
        
        public static void RegistFactory(ushort id)
        {
            ProtoFacotoryPool.Instance.Regist(id, new ProtocolFactory<T>(id));
        }
        
        public ProtocolFactory(ushort messageId)
        {
            this.messageId = messageId;
        }
        
        public override IProtocol GetProtocol()
        {
            var protocol = CommonObjectPool<T>.Get();
            protocol.MessageId = messageId;
            return protocol;
        }
        
    }
}
using System;
using System.ComponentModel;
using System.Xml;

namespace ReFrame.Util
{
    public static class XmlSerialize<T>
    {
        private static Func<XmlElement, T> _readFunc = DefaultReadFunc;
        private static Action<XmlElement, T> _writeAct = DefaultWriteAct;

        public static void Init(Func<XmlElement, T> readFunc, Action<XmlElement, T> writeAct)
        {
            _readFunc = readFunc;
            _writeAct = writeAct;
        }

        public static T Read(XmlElement ele)
        {
            return _readFunc(ele);
        }

        public static void Write(XmlElement ele, T value)
        {
            _writeAct(ele, value);
        }

        private static T DefaultReadFunc(XmlElement ele)
        {
            
        }

        private static void DefaultWriteAct(XmlElement ele, T value)
        {
            XmlDocument doc;
            XmlWriter writer;
        }
    }
}
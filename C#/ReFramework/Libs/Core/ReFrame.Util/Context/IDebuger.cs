using System;

namespace ReFrame.Util
{
    public interface IDebuger
    {
        void Log(string log);
        void Log(string tag, string log);

        void LogWarning(string log);
        void LogWarning(string tag, string log);

        void LogError(string log);
        void LogError(string tag, string log);

        void LogException(Exception e);
        void LogException(string tag, Exception e);
    }
}
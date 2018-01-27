using System;
using ReFrame.Util.Debug;
using UnityEngine;

namespace Reframe.Unity.Utility
{
    public class UnityDebuger : IDebuger
    {
        public void Log(string log)
        {
            Debug.unityLogger.Log(log);
        }

        public void Log(string tag, string log)
        {
            Debug.unityLogger.Log(tag, log);
        }

        public void LogWarning(string log)
        {
            Debug.unityLogger.LogWarning("default", log);
        }

        public void LogWarning(string tag, string log)
        {
            Debug.unityLogger.LogWarning(tag, log);
        }

        public void LogError(string log)
        {
            Debug.unityLogger.LogError("default", log);
        }

        public void LogError(string tag, string log)
        {
            Debug.unityLogger.LogError(tag, log);
        }

        public void LogException(Exception e)
        {
            Debug.unityLogger.LogException(e);
        }

        public void LogException(string tag, Exception e)
        {
            Debug.unityLogger.Log(tag, e);
        }
    }
}

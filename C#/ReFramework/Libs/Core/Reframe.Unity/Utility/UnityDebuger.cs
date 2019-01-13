using System;
using ReFrame.Util.Debug;
using UnityEngine;

namespace Reframe.Unity.Utility
{
    public class UnityDebuger : IDebuger
    {
        private static readonly string DEFAULT_TAG = "DEFAULT";

        public void Log(string log)
        {
            Log(DEFAULT_TAG, log);
        }

        public void Log(string tag, string log)
        {
            Debug.Log($"[{tag}] {log}");
        }

        public void LogWarning(string log)
        {
            LogWarning(DEFAULT_TAG, log);
        }

        public void LogWarning(string tag, string log)
        {
            Debug.LogWarning($"[{tag}] {log}");
        }

        public void LogError(string log)
        {
            LogError(DEFAULT_TAG, log);
        }

        public void LogError(string tag, string log)
        {
            Debug.LogError($"[{tag}] {log}");
        }

        public void LogException(Exception e)
        {
            LogException(DEFAULT_TAG, e);
        }

        public void LogException(string tag, Exception e)
        {
            Debug.Log($"[{tag}] {e}");
        }
    }
}

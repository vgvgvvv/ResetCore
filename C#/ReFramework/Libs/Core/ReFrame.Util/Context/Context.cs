using ReFrame.Util;

namespace ReFrame.Util
{
    public static class Context
    {
        public static IDebuger Logger => ContextValue<IDebuger>.Value;
    }
}
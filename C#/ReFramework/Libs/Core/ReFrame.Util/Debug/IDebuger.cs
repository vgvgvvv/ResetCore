namespace ReFrame.Util.Debug
{
    public interface IDebuger
    {
        void Log();
        void LogWarning();
        void LogError();
    }
}
using ReFrame.Core;

namespace ReFrame.Trigger
{
    public interface IEvent : IBase
    {
        
    }

    public interface IEvent<T1> : IEvent
    {
        IArg<T1> Arg1 { get; }
    }

    public interface IEvent<T1,T2> : IEvent<T1>
    {
        IArg<T2> Arg2 { get; }
    }

    public interface IEvent<T1, T2, T3> : IEvent<T1, T2>
    {
        IArg<T3> Arg3 { get; }
    }
}
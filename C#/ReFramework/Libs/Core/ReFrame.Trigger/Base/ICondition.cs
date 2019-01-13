using System;
using ReFrame.Core;

namespace ReFrame.Trigger
{

    public interface ICondition<T1, T2, T3> : ICondition<T1, T2>
    {
        IArg<T1> Arg3 { get; }
    }

    public interface ICondition<T1, T2> : ICondition<T1>
    {
        IArg<T1> Arg2 { get; }
    }

    public interface ICondition<T1> : ICondition
    {
        IArg<T1> Arg1 { get; }
    }

    public interface ICondition : IBase
    {
        bool Pass();
    }
}
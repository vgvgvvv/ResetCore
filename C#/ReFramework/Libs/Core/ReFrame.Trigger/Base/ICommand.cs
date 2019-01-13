using System;
using ReFrame.Core;

namespace ReFrame.Trigger
{
    public interface ICommand<T1, T2, T3> : ICommand<T1, T2>
    {
        IArg<T3> Arg3 { get; }
    }

    public interface ICommand<T1, T2> : ICommand<T1>
    {
        IArg<T2> Arg2 { get; }
    }

    public interface ICommand<T1> : ICommand
    {
        IArg<T1> Arg1 { get; }
    }

    public interface ICommand : IBase
    {
        void Execute(Action callback);
    }
}
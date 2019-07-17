using System;

namespace TaskSequence
{
    public class BaseTask
    {
        
    }

    public class EntryTask : BaseTask
    {
        
    }

    public class ContainerTask : BaseTask
    {
        
    }

    public class ActionTask : BaseTask
    {
        
    }

    public class BatTask : ActionTask
    {
        public string BatPath;
        public string Args;
        public string WorkDirectory;
    }
}
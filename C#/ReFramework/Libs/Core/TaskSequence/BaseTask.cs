using System;
using System.Collections.Generic;

namespace TaskSequence
{
    public class BaseTask
    {
        public string Name;
        public string AppPath;
        public string Args;
        public string WorkDirectory;
        public string Comment;
        public string SubTaskName;

        public virtual void Run(Action callback)
        {
            callback();
        }
    }

    
}
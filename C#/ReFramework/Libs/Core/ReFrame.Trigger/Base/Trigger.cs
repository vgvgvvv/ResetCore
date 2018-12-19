using System.Collections.Generic;
using ResetCore.Util;

namespace ReFrame.Trigger
{
    public class Trigger : ITrigger
    {
        public List<IEvent> Events { get; }
        public List<ICondition> Conditions { get; }
        public List<ICommand> Commands { get; }

        private Queue<ICommand> CommandQueue = new Queue<ICommand>();

        /// <summary>
        /// 触发
        /// </summary>
        public void OnTrigger()
        {
            foreach (var condition in Conditions)
            {
                if (condition.Pass())
                {
                    return;
                }
            }
        }

        /// <summary>
        /// 中断处理
        /// </summary>
        public void OnCancel()
        {
            
        }

    }
}
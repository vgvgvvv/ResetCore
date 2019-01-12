using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ReFrame.Trigger
{
    public interface ITrigger
    {
        List<IEvent> Events { get; }
        List<ICondition> Conditions { get; }
        List<ICommand> Commands { get; }

        void OnTrigger();
        void OnCancel();
    }
}

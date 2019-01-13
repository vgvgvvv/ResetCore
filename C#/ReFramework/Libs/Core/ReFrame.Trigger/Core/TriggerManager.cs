using System.Collections.Generic;
using ReFrame.Util;

namespace ReFrame.Trigger
{
    public class TriggerManager
    {
        Dictionary<ulong, HashSet<ITrigger>> TriggerCollection = new Dictionary<ulong, HashSet<ITrigger>>();

        public void LoadAllTrigger(IDataSerialize<ITrigger> )
        {

        }

        private void RegisterTrigger(Trigger trigger)
        {

        }

        public void TriggerWithEvent<T>() where T : IEvent
        {
            if (!TriggerCollection.TryGetValue(MUUID<T>.UUID, out var triggers))
            {
                return;
            }

            foreach (var trigger in triggers)
            {
                trigger.OnTrigger();
            }
        }
    }
}
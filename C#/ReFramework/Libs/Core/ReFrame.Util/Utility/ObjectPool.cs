using System.Collections.Generic;

namespace ReFrame.Util.Utility
{
    public static class ObjectPool
    {
        private static readonly Dictionary<uint, Queue<Reusable>> ReusePool = 
            new Dictionary<uint, Queue<Reusable>>();

        /// <summary>
        /// 获取对象
        /// </summary>
        /// <param name="hash"></param>
        /// <returns></returns>
        public static T GetObject<T>(uint hash) where T:Reusable, new()
        {
            Queue<Reusable> queue;
            if (!ReusePool.TryGetValue(hash, out queue))
            {
                queue = new Queue<Reusable>();
                ReusePool[hash] = queue;
            }
            if (queue.Count == 0)
            {
                return new T();
            }
            return queue.Dequeue() as T;
        }

        /// <summary>
        /// 归还对象
        /// </summary>
        /// <param name="obj"></param>
        public static void ReturnObject(Reusable obj)
        {
            Queue<Reusable> queue;
            if (!ReusePool.TryGetValue(obj.Hash, out queue))
            {
                queue = new Queue<Reusable>();
                ReusePool[obj.Hash] = queue;
            }
            queue.Enqueue(obj);
        }
    }
}
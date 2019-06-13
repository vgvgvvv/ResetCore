using System;
using System.Threading;
using NUnit.Framework;

namespace CSLearning.MultThread
{
    public class ThreadLocalTest
    {
        public ThreadLocal<int> _threadlocal =
            new ThreadLocal<int>(() =>
            {
                return Thread.CurrentThread.ManagedThreadId;
            });

        [Test]
        public void Test()
        {
            new Thread(() =>
            {
                for (int x = 0; x < _threadlocal.Value; x++)
                {
                    Console.WriteLine("First Thread: {0}", x);
                    Thread.Sleep(1);
                }
            }).Start();

            new Thread(() =>
            {
                for (int x = 0; x < _threadlocal.Value; x++)
                {
                    Console.WriteLine("Second Thread: {0}", x);
                    Thread.Sleep(1);
                }
            }).Start();

        }
    }
}
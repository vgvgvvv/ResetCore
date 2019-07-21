using System;
using NUnit.Framework;
using TaskSequence;

namespace UnitTest.TestTaskSequence
{
    public class TaskSeqTest
    {
        [Test]
        public void RunTestSeq()
        {
            var taskQueue = TaskQueue.LoadFromFile(
                @"E:\Projects\ResetCore\C#\ReFramework\Libs\Core\UnitTest\TestTaskSequence\test_task_seq.json");
            taskQueue.Run();
        }
    }
}
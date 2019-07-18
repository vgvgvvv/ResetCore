using System;
using System.Collections.Generic;
using System.IO;
using LitJson;
using ResetCore.Util;

namespace TaskSequence
{
    public class TaskQueue
    {

        private readonly Queue<BaseTask> _taskQueue = new Queue<BaseTask>();
        private ActionQueue _actionQueue = new ActionQueue();
        
        
        public static TaskQueue LoadFromFile(string filePath)
        {
            var json = File.ReadAllText(filePath);
            var jsonData = JsonMapper.ToObject<JsonData>(json);
            
            TaskQueue queue = new TaskQueue();
            
            foreach (var obj in jsonData)
            {
                queue._taskQueue.Enqueue((BaseTask)obj);
            }
            
            return queue;
        }
        
        public void Run(Action callback)
        {
            while (_taskQueue.Count > 0)
            {
                _actionQueue.AddAction(_taskQueue.Dequeue().Run);
            }
            _actionQueue.AddAction(callback);
        } 
    }
}
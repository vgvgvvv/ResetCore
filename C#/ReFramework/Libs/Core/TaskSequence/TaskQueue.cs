using System;
using System.Collections.Generic;
using System.IO;
using LitJson;
using NUnit.Framework;
using ResetCore.Util;

namespace TaskSequence
{
    public partial class TaskQueue
    {

        private readonly List<BaseTask> _taskQueue = new List<BaseTask>();
        private readonly ActionQueue _actionQueue = new ActionQueue();
        private readonly Dictionary<string, string> _environmentVarDict = new Dictionary<string, string>();
        
        
        public static TaskQueue LoadFromFile(string filePath)
        {
            var json = File.ReadAllText(filePath);
            var taskArr = JsonMapper.ToObject<BaseTask[]>(json);
            
            TaskQueue queue = new TaskQueue();

            var envFile = filePath + ".config";
            if (File.Exists(envFile))
            {
                queue.LoadEnvironmentVar(envFile);
            }
            
            foreach (var task in taskArr)
            {
                if (!string.IsNullOrEmpty(task.Enable) && task.Enable.ToLower() == "false")
                {
                    continue;
                }
                queue._taskQueue.Add(task.Init());
            }
            
            return queue;
        }

        public void LoadEnvironmentVar(string filePath)
        {
            var json = File.ReadAllText(filePath);
            var jsonData = JsonMapper.ToObject<JsonData>(json);

            foreach (var key in jsonData.Keys)
            {
                _environmentVarDict.Add(key, jsonData[key].ToString());
            }
        }
        
        public void Run(Action callback = null)
        {
            foreach(var task in _taskQueue)
            {
                _actionQueue.AddAction((cb) =>
                {
                    Console.WriteLine($"\n========================{task.Comment}===========================\n");
                    task.Run(cb, _environmentVarDict);
                });
            }

            if (callback != null)
            {
                _actionQueue.AddAction(callback);
            }
        } 
    }
}
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;

namespace TaskSequence
{
    public class BaseTask
    {
        public string Name;
        public string AppPath;
        public string Args;
        public string WorkDirectory;
        public string Comment;
        public string Enable;

        public BaseTask Init()
        {
            if (string.IsNullOrEmpty(AppPath))
            {
                AppPath = "cmd.exe";
            }

            if (string.IsNullOrEmpty(WorkDirectory))
            {
                WorkDirectory = Directory.GetCurrentDirectory();
            }

            return this;
        }

        public virtual void Run(Action callback, Dictionary<string, string> environmentVar = null)
        {
            if (AppPath == "cmd.exe")
            {
                RunCmd(callback, environmentVar);
            }
            else
            {
                RunApp(callback, environmentVar);
            }
        }

        private void RunCmd(Action callback, Dictionary<string, string> environmentVar)
        {
            Process myProcess = new Process();

            ProcessStartInfo myProcessStartInfo = new ProcessStartInfo(AppPath);

            myProcessStartInfo.WorkingDirectory = WorkDirectory;
            myProcessStartInfo.UseShellExecute = false;
            myProcessStartInfo.RedirectStandardInput = true;
            myProcessStartInfo.RedirectStandardError = true;
            myProcessStartInfo.RedirectStandardOutput = true;
            myProcessStartInfo.CreateNoWindow = true;

            if (environmentVar != null)
            {
                foreach (var kvp in environmentVar)
                {
                    myProcessStartInfo.EnvironmentVariables.Add(kvp.Key, kvp.Value);
                }
            }

            myProcess.StartInfo = myProcessStartInfo;

            myProcess.Start();

            myProcess.StandardInput.WriteLine(Args + " & exit");

            myProcess.StandardInput.AutoFlush = true;

            string output = myProcess.StandardOutput.ReadToEnd();

            myProcess.WaitForExit();
            myProcess.Close();
            Console.WriteLine(output);

            if (callback != null)
            {
                callback();
            }
        }

        private void RunApp(Action callback, Dictionary<string, string> environmentVar)
        {
            Process myProcess = new Process();

            ProcessStartInfo myProcessStartInfo = new ProcessStartInfo(AppPath, Args);

            myProcessStartInfo.WorkingDirectory = WorkDirectory;

            if (environmentVar != null)
            {
                foreach (var kvp in environmentVar)
                {
                    myProcessStartInfo.EnvironmentVariables.Add(kvp.Key, kvp.Value);
                }
            }

            myProcess.StartInfo = myProcessStartInfo;

            myProcess.Start();

            myProcess.WaitForExit();

            if (callback != null)
            {
                callback();
            }
        }
    }

    
}
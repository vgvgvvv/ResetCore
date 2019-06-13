using System.Collections;
using System.Collections.Generic;
using Unity.Jobs;
using UnityEngine;

/**
 * IJob: 执行一次的简单作业
 * IJobParallelFor: 为一系列值执行的作业
 * IJobParallelForTransform: 能访问transforms的作业
 */
public struct MyJob : IJob
{

    public void Execute()
    {
        
    }
}

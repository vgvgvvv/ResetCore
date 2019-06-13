using System.Collections;
using System.Collections.Generic;
using Unity.Jobs;
using UnityEngine;

public class JobHandler : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        MyJob job = new MyJob();
        JobHandle jobHandle = job.Schedule();
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}

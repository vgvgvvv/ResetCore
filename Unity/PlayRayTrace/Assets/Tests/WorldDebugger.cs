using System.Collections;
using System.Collections.Generic;
using RayTracing;
using UnityEngine;

public class WorldDebugger : MonoBehaviour
{
    World world = new World();

    void Update()
    {
        world.Trace();
    }

    void OnDrawGizmos()
    {
        world.DrawGizmos();
    }
}

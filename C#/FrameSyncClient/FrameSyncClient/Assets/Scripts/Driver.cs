using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace FrameSync
{
    public class Driver : MonoBehaviour {
        
        private void Awake()
        {
            Application.targetFrameRate = 60;
            Input.multiTouchEnabled = true;
            Screen.sleepTimeout = SleepTimeout.NeverSleep;
        }

        // Use this for initialization
        void Start () {
		    World.Instance.InitWorld();
        }
	
        // Update is called once per frame
        void Update () {
		
            World.Instance.Update();
        }

        private void LateUpdate()
        {
            World.Instance.LateUpdate();
        }

        private void FixedUpdate()
        {
            World.Instance.FixedUpdate();
        }

        private void OnApplicationQuit()
        {
            World.Instance.OnApplicationQuit();
        }
    }
}

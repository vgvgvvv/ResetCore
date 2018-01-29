using System;
using System.Collections;
using System.Collections.Generic;
using FrameSync.Managers;
using ReFrame.Util.Utility;
using UnityEngine;

namespace FrameSync
{
    public class World : Singleton<World>
    {
	    private Dictionary<Type, IManager> _managers = new Dictionary<Type, IManager>();

	    public T GetManager<T>() where T : class, IManager
	    {
		    return _managers[typeof(T)] as T;
	    }

	    public void AddManager<T>() where T : class, IManager, new()
	    {
		    _managers.Add(typeof(T), new T());
	    }
	    
        public void InitWorld()
        {
	        AddManager<InputManager>();
	        AddManager<NetManager>();
			AddManager<GameSyncManager>();
	        AddManager<TickManager>();
	        
	        foreach (IManager manager in _managers.Values)
	        {
		        manager.Init();
	        }
        }

        public void Update()
        {
	        foreach (IManager manager in _managers.Values)
	        {
		        manager.Update();
	        }
        }

        public void LateUpdate()
        {
	        foreach (IManager manager in _managers.Values)
	        {
		        manager.LateUpdate();
	        }
        }

        public void FixedUpdate()
        {
	        foreach (IManager manager in _managers.Values)
	        {
		        manager.FixedUpdate();
	        }
        }

	    public void OnApplicationQuit()
	    {
		    foreach (IManager manager in _managers.Values)
		    {
			    manager.UnInit();
		    }
	    }
    }
}

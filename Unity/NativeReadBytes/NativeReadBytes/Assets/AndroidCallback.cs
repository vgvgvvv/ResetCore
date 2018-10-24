using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AndroidCallback : MonoBehaviour {

	public void Log(string log)
	{
		Debug.Log(log);
	}
	
	public void LogWarn(string log)
	{
		Debug.LogWarning(log);
	}
	
	public void LogError(string log)
	{
		Debug.LogError(log);
	}
	
}

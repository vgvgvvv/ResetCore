using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class TestShake : MonoBehaviour
{



#if UNITY_IOS
	[DllImport("__Internal")]
	public static extern void Shake();
#endif
	
	void OnGUI()
	{

		if (GUILayout.Button("Shake", GUILayout.Height(200), GUILayout.Width(200)))
		{
//			Handheld.Vibrate();
#if UNITY_IOS
			Shake();
#endif
		}
	}
}

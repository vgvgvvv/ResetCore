using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;
using UnityEngine.UI;

public class HideLog : MonoBehaviour
{

    private static AndroidJavaClass _unityPlayerClass;
    public static AndroidJavaClass UnityPlayerClass
    {
        get
        {
            if (_unityPlayerClass == null)
            {
                _unityPlayerClass = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
            }
            return _unityPlayerClass;
        }
    }

    private static AndroidJavaObject _currentActivity;

    public static AndroidJavaObject CurrentActivity
    {
        get
        {
            if (_currentActivity == null)
            {
                _currentActivity = UnityPlayerClass.GetStatic<AndroidJavaObject>("currentActivity");
            }
            return _currentActivity;
        }
    }

    public RawImage rawImg;
//
//    [DllImport("__Internal")]
//    public static extern void iosPlayMovie(string name);

    void Awake()
    {
//        iosPlayMovie("cg");
    }

	// Use this for initialization
	void Hide ()
	{
	    StartCoroutine(DoHide());
	}
	

    IEnumerator DoHide()
    {
        yield return new WaitForSeconds(3);
        rawImg.gameObject.SetActive(false);
    }
}

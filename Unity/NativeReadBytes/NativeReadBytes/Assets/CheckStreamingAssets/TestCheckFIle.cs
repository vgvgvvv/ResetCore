using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public class TestCheckFIle : MonoBehaviour {

	// Use this for initialization
	void Start () {
		Debug.Log("StreamingAssetExist : " + StreamingAssetExist("Test/aaa.txt"));
	}
	
    public bool StreamingAssetExist(string filePath)
    {
        if (Application.platform == RuntimePlatform.Android)
        {
            return Android.CurrentActivity.Call<bool>("CheckAssetExist", filePath);
        }
        return File.Exists(Path.Combine(Application.streamingAssetsPath, filePath));
    }
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScreenManager : MonoBehaviour
{

    public RectTransform TestCanvas;

	// Use this for initialization
	void Start ()
	{
        Debug.Log("OriginSize is " + Screen.width + ", " + Screen.height);
	    StartCoroutine(UpdateRect());
	}
	
	// Update is called once per frame
	void Update ()
	{
	    
	}


    public void TryGetScreenScale(out Rect result)
    {
        var screenManagerClass = new AndroidJavaClass("nativeread.reset.com.nativeread.ScreenManager");
        if (screenManagerClass.CallStatic<int>("supportGoogleAPI") >= 28)
        {
            int[] cutout = screenManagerClass.CallStatic<int[]>("getNotchInGoogle");
            int buttom = cutout[0];
            int top = cutout[1];
            int left = cutout[2];
            int right = cutout[3];
            int finalTop = Mathf.Max(top, buttom);
            int finalLeft = Mathf.Max(left, right);
            result = new Rect(finalLeft, finalTop, Screen.width - finalLeft * 2, Screen.height - finalTop * 2);
            Debug.Log("GoogleType:" + result.ToString());
        }
        else
        {
            var notchHeight = screenManagerClass.CallStatic<int>("TryGetNotchSize");
            if (notchHeight < 0)
            {
                result = new Rect(0, 0, Screen.width, Screen.height);
                Debug.Log("None:" + result.ToString());
            }
            else
            {
                result = new Rect(notchHeight, 0, Screen.width - notchHeight * 2, Screen.height);
                Debug.Log("Offical:" + result.ToString());
            }
        }

    }

    private static void ApplySafeArea(RectTransform panel, Rect area)
    {
        var anchorMin = area.position;
        var anchorMax = area.position + area.size;
        anchorMin.x /= Screen.width;
        anchorMin.y /= Screen.height;
        anchorMax.x /= Screen.width;
        anchorMax.y /= Screen.height;

        //iOS对旋转已经做了相应的处理
        if (Application.platform != RuntimePlatform.IPhonePlayer && Screen.orientation == ScreenOrientation.LandscapeRight)
        {
            var newAnchorMin = new Vector2(1 - anchorMax.x, 1 - anchorMax.y);
            var newAnchorMax = new Vector2(1 - anchorMin.x, 1 - anchorMin.y);
            anchorMin = newAnchorMin;
            anchorMax = newAnchorMax;
        }

        panel.anchorMin = anchorMin;
        panel.anchorMax = anchorMax;

    }

    IEnumerator UpdateRect()
    {
        while (true)
        {
            
            if (Application.isEditor)
            {
                ApplySafeArea(TestCanvas, new Rect(0, 0, Screen.width, Screen.height));
            }
            else
            {
                Rect result;
                TryGetScreenScale(out result);
                ApplySafeArea(TestCanvas, result);
            }
            yield return new WaitForSeconds(2);
        }
       
    }

}

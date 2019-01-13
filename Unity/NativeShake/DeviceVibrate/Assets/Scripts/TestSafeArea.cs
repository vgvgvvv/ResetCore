using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestSafeArea : MonoBehaviour
{


	public Canvas canvas;
	private RectTransform _rectTransform;
	
	// Use this for initialization
	void Start ()
	{
		_rectTransform = canvas.GetComponent<RectTransform>();
		StartCoroutine(UpdateScreenInfo());
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	IEnumerator UpdateScreenInfo()
	{
		while (true)
		{
			yield return new WaitForSeconds(2);
			Debug.Log(string.Format("{0}, {1}", Screen.width.ToString(), Screen.height.ToString()));
			Debug.Log(Screen.safeArea.ToString());
			var x = Screen.safeArea.x / Screen.width;
			var y = Screen.safeArea.y / Screen.height;
			var width = Screen.safeArea.width / Screen.width;
			var height = Screen.safeArea.height / Screen.height;
			_rectTransform.anchorMin = new Vector2(x, y);
			_rectTransform.anchorMax = new Vector2(x + width, y + height);
		}
	}
	
}

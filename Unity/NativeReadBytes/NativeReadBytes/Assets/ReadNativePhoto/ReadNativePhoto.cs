using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using UnityEngine;
using UnityEngine.UI;
using Object = UnityEngine.Object;

public class ReadNativePhoto : MonoBehaviour
{
    public RawImage img;
    private string imgpath;

    private NativeTexture currentRawTexture;


    void Awake()
    {
        imgpath = Path.Combine(Application.persistentDataPath, "test.png");
        Debug.Log(imgpath);
        gameObject.AddComponent<NativeTextureManager>();
    }
    void OnGUI()
    {
        if (GUILayout.Button("TakeSnap", GUILayout.Height(150), GUILayout.Width(150)))
        {
            Debug.Log("TakeSnap " + imgpath);
            TakeScreenShotFromScreenSize(tex =>
            {
                SaveTexture(tex, imgpath);
            });
        }

        if (GUILayout.Button("LoadByCS", GUILayout.Height(150), GUILayout.Width(150)))
        {
            Debug.Log("Load By CS " + imgpath);
            if (!File.Exists(imgpath))
            {
                Debug.LogError(imgpath + " not exist!!!");
            }
            var bytes = File.ReadAllBytes(imgpath);
            Texture2D texture = new Texture2D(1280, 800);
            texture.LoadImage(bytes);
            img.texture = texture;
        }
        
        if (GUILayout.Button("Load", GUILayout.Height(150), GUILayout.Width(150)))
        {
            Debug.Log("Load " + imgpath);
            if (!File.Exists(imgpath))
            {
                Debug.LogError(imgpath + " not exist!!!");
            }
            currentRawTexture = new NativeTexture(imgpath);
            img.texture = currentRawTexture.Tex;
        }
        
        if (GUILayout.Button("LoadFromStreamingAssets", GUILayout.Height(150), GUILayout.Width(150)))
        {
            Debug.Log("LoadFromStreamingAssets " + "test.png");
            currentRawTexture = new NativeTexture("test.png", true);
            img.texture = currentRawTexture.Tex;
        }

        if (GUILayout.Button("UnLoad", GUILayout.Height(150), GUILayout.Width(150)))
        {
            Debug.Log("UnLoad");
            currentRawTexture.Dispose();
        }
    }
    
    public void TakeScreenShotFromScreenSize(Action<Texture2D> callback)
    {
        TakeScreenShot(new Rect(Screen.width * 0, Screen.height * 0, Screen.width, Screen.height), callback);
    }

    public void TakeScreenShot(Rect rect, Action<Texture2D> callback)
    {
        StartCoroutine(DoTakeScreenShot(rect, callback));
    }
    
    private IEnumerator DoTakeScreenShot(Rect rect, Action<Texture2D> callback)
    {
        yield return new WaitForEndOfFrame();

        Texture2D screenShot = new Texture2D((int)rect.width, (int)rect.height, TextureFormat.RGB24, false);
        screenShot.ReadPixels(rect, 0, 0, false);
        screenShot.Apply();

        if (callback != null)
            callback(screenShot);
    }
    
    public static void SaveTexture(Texture2D texture, string path, bool destroyAfterSave = true)
    {
        if (!Directory.Exists(path))
            Directory.CreateDirectory(Path.GetDirectoryName(path));

        var bytes = texture.EncodeToPNG();
        File.WriteAllBytes(path, bytes);

        if (destroyAfterSave)
        {
            Object.Destroy(texture);
        }
    }
}

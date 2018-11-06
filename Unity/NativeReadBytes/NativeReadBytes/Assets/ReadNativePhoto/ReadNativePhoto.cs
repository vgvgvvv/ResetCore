using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;
using UnityEngine.UI;
using Object = UnityEngine.Object;

public class ReadNativePhoto : MonoBehaviour
{

    public RawImage img;
    private string imgpath;

    private RawTexture currentRawTexture;


    void Awake()
    {
        imgpath = Path.Combine(Application.persistentDataPath, "test.png");
    }
    void OnGUI()
    {
        if (GUILayout.Button("TakeSnap", GUILayout.Height(100), GUILayout.Width(100)))
        {
            ScreenCapture.CaptureScreenshot(imgpath);
        }

        if (GUILayout.Button("Load", GUILayout.Height(100), GUILayout.Width(100)))
        {
            if (Application.isMobilePlatform)
            {
                if (currentRawTexture != null)
                {
                    currentRawTexture.Dispose();
                }
                currentRawTexture = new RawTexture(imgpath, 1280, 800);
                img.texture = currentRawTexture.Tex;
            }
            else
            {
                byte[] buff = new byte[1024];
                using (var fs = File.OpenRead(imgpath))
                {
                    using (var ms = new MemoryStream())
                    {
                        int offset = 0;
                        while (true)
                        {
                            int num = fs.Read(buff, 0, buff.Length);
                            if (num <= 0)
                            {
                                break;
                            }
                            ms.Write(buff, 0, num);
                            offset += num;
                        }

                        Texture2D texture = new Texture2D(1280, 800);
                        texture.LoadRawTextureData(ms.ToArray());
                        img.texture = texture;
                    }
                }
            }

        }

        if (GUILayout.Button("UnLoad", GUILayout.Height(100), GUILayout.Width(100)))
        {
            if (Application.isMobilePlatform)
            {
                currentRawTexture.Dispose();
            }
            else
            {
                Object.Destroy(img.texture);
            }
        }
    }
}

public class RawTexture : IDisposable
{
    private IntPtr ptr = IntPtr.Zero;
    public Texture2D Tex { get; private set; }

    public RawTexture(string path, int width, int height)
    {
        Tex = new Texture2D(width, height);
        var size = ReadNativeByte.ReadRawBytes(path, ref ptr);
        if (size < 0)
        {
            Debug.LogError(string.Format("size < 0 : {0}", size));
        }
        Tex.LoadRawTextureData(ptr, size);
        ReadNativeByte.ReleaseBytes(ptr);
    }

    public void Dispose()
    {
        Object.Destroy(Tex);
        GC.SuppressFinalize(this);
    }
}

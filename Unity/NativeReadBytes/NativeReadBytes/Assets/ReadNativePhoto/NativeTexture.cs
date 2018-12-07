

using System;
using System.IO;
using System.Runtime.InteropServices.ComTypes;
using UnityEngine;


public class NativeTexture : IDisposable
{
    public Texture2D Tex { get; private set; }
    private IntPtr _loader;
    public int Width { get; private set; }
    public int Height { get; private set; }
    
    public NativeTexture(string fileName, bool fromStreamingAssets = false)
    {

#if UNITY_ANDROID
        _loader = PngLib.CreateLoader();
        if (!fromStreamingAssets)
        {
            if (!PngLib.LoadWithPath(_loader, fileName))
            {
                Debug.LogError("Load With Path Error");
                return;
            }
        }
        else
        {
            if (!PngLib.LoadFromStreamingAssets(_loader, fileName))
            {
                Debug.LogError("Load From StreamingAssets Error");
                return;
            }
        }
        
        Width = PngLib.GetWidth(_loader);
        Height = PngLib.GetHeight(_loader);
        Tex = new Texture2D(Width, Height, TextureFormat.RGBA32, false);
        PngLib.SetTexture(_loader, Tex.GetNativeTexturePtr());
        NativeTextureManager.Instance.UpdateTexture();
        
#elif UNITY_IOS
        _loader = PngLib.CreateLoader();
        if (!fromStreamingAssets)
        {
            PngLib.Load(_loader, fileName);
        }
        else
        {
            PngLib.Load(_loader, Path.Combine(Application.streamingAssetsPath, fileName));
        }
        Width = PngLib.GetWidth(_loader);
        Height = PngLib.GetHeight(_loader);
        Tex = Texture2D.CreateExternalTexture(Width, Height, TextureFormat.ARGB32, false, true,
            PngLib.GetTexturePtr(_loader));
#endif
    }

    public void Dispose()
    {
        
#if UNITY_ANDROID
        PngLib.DestroyLoader(_loader);
        UnityEngine.Object.Destroy(Tex);
        Tex = null;
        _loader = IntPtr.Zero;
        Width = 0;
        Height = 0;
        GC.SuppressFinalize(this);
#elif UNITY_IOS
        PngLib.DestroyLoader(_loader);
        UnityEngine.Object.Destroy(Tex);
        Tex = null;
#endif
    }
}

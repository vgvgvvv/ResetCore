

using System;
using System.Runtime.InteropServices.ComTypes;
using UnityEngine;

public class NativeTexture : IDisposable
{
    public Texture2D Tex { get; private set; }
    private IntPtr _loader;
    public int Width { get; private set; }
    public int Height { get; private set; }
    
    public NativeTexture(string fileName)
    {
        _loader = PngLib.CreateLoader();
        if (!PngLib.LoadWithPath(_loader, fileName))
        {
            Debug.LogError("Load With Path Error");
            return;
        }
        Width = PngLib.GetWidth(_loader);
        Height = PngLib.GetHeight(_loader);
        Tex = new Texture2D(Width, Height, TextureFormat.RGBA32, false);
        PngLib.SetTexture(_loader, Tex.GetNativeTexturePtr());
        NativeTextureManager.Instance.UpdateTexture();
    }

    public void Dispose()
    {
        PngLib.DestroyLoader(_loader);
        UnityEngine.Object.Destroy(Tex);
        Tex = null;
        _loader = IntPtr.Zero;
        Width = 0;
        Height = 0;
        GC.SuppressFinalize(this);
    }
}
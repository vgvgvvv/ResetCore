using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public static class PngLib
{
	
	#if UNITY_IOS
	public const string libName = "__Internal";
    #else
	public const string libName = "NativeLib";
	#endif
	
	#if UNITY_ANDROID
	[DllImport(libName)]
	public static extern IntPtr CreateLoader();

	[DllImport(libName)]
	public static extern void DestroyLoader(IntPtr loader);

	[DllImport(libName)]
	public static extern bool Load(IntPtr loader, IntPtr data, int size);

	[DllImport(libName)]
	public static extern bool LoadWithPath(IntPtr loader, string fileName);
	
	[DllImport(libName)]
	public static extern bool LoadFromStreamingAssets(IntPtr loader, string fileName);

	[DllImport(libName)]
	public static extern void SetTexture(IntPtr loader, IntPtr texture);

	[DllImport(libName)]
	public static extern void UpdateTextureImmediate(IntPtr loader);

	[DllImport(libName)]
	public static extern int GetWidth(IntPtr loader);

	[DllImport(libName)]
	public static extern int GetHeight(IntPtr loader);

	[DllImport(libName)]
	public static extern IntPtr GetPngRenderEventFunc();
	#elif UNITY_IOS

	[DllImport(libName)]
	public static extern IntPtr CreateLoader();
	[DllImport(libName)]
	public static extern bool Load(IntPtr loader, string fileName);
	[DllImport(libName)]
	public static extern void DestroyLoader(IntPtr loader);
	[DllImport(libName)]
	public static extern int GetWidth(IntPtr loader);
	[DllImport(libName)]
	public static extern int GetHeight(IntPtr loader);
	[DllImport(libName)]
	public static extern IntPtr GetTexturePtr(IntPtr loader);
	[DllImport(libName)]
	public static extern System.IntPtr CreateNativeTexture(string filename);
	[DllImport(libName)]
	public static extern System.IntPtr CreateNativeTextureWithFullPath(string filePath);
	[DllImport(libName)]
	public static extern void DestroyNativeTexture(System.IntPtr tex);
	
	#endif
}

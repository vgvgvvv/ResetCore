using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public static class PngLib
{
	[DllImport("NativeLib")]
	public static extern IntPtr CreateLoader();

	[DllImport("NativeLib")]
	public static extern void DestroyLoader(IntPtr loader);

	[DllImport("NativeLib")]
	public static extern bool Load(IntPtr loader, IntPtr data, int size);

	[DllImport("NativeLib")]
	public static extern bool LoadWithPath(IntPtr loader, string fileName);

	[DllImport("NativeLib")]
	public static extern void SetTexture(IntPtr loader, IntPtr texture);

	[DllImport("NativeLib")]
	public static extern void UpdateTextureImmediate(IntPtr loader);

	[DllImport("NativeLib")]
	public static extern int GetWidth(IntPtr loader);

	[DllImport("NativeLib")]
	public static extern int GetHeight(IntPtr loader);

	[DllImport("NativeLib")]
	public static extern IntPtr GetPngRenderEventFunc();
}

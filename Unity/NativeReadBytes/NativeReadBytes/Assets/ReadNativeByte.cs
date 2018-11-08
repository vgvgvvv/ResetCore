using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class ReadNativeByte
{
	#if UNITY_IOS
	public const string libName = "__Internal";
    #else
	public const string libName = "NativeLib";
	#endif
	
	#if UNITY_ANDROID

	[DllImport(libName)]
	public static extern int ReadAssetsBytes(string name, ref IntPtr ptr);
	
	[DllImport(libName)]
	public static extern int ReadAssetsBytesWithOffset(string name, ref IntPtr ptr, int offset, int length);
	
	[DllImport(libName)]
	public static extern int Add(int a, int b);
	
	#endif
	
    [DllImport(libName)]
    public static extern int ReadRawBytes(string name, ref IntPtr ptr);

    [DllImport(libName)]
	public static extern void ReleaseBytes(IntPtr ptr);
	
	
	
}

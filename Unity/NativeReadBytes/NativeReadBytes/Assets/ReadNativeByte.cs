using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class ReadNativeByte
{

	[DllImport("NativeLib")]
	public static extern int ReadBytes(string name, ref IntPtr ptr);

	[DllImport("NativeLib")]
	public static extern void ReleaseBytes(IntPtr ptr);
	
	[DllImport("NativeLib")]
	public static extern int Add(int a, int b);
	
}

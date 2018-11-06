using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Security;
using UnityEngine;

public class TestReadByte : MonoBehaviour {

	MemoryStream stream = new MemoryStream();

	private void OnGUI()
	{
		if (GUILayout.Button("Test", GUILayout.Height(100), GUILayout.Width(100)))
		{
			var ptr = IntPtr.Zero;
			int size = ReadNativeByte.ReadAssetsBytesWithOffset("Test.txt", ref ptr, sizeof(int), sizeof(int));
			Debug.Log("Size:" + size.ToString());
			if (size > 0)
			{
				if (ptr == IntPtr.Zero)
				{
					Debug.LogError("Read Failed!!!");
				}
				stream.SetLength(size);
				stream.Position = 0;
				Marshal.Copy(ptr, stream.GetBuffer(), 0, size);
				var reader = new BinaryReader(stream);
				Debug.Log(reader.ReadInt32().ToString());
				ReadNativeByte.ReleaseBytes(ptr);
			}
		}
		
		if (GUILayout.Button("Test2", GUILayout.Height(100), GUILayout.Width(100)))
		{
			Debug.Log(ReadNativeByte.Add(100, 200).ToString());
		}
		
	}

	// Update is called once per frame
	void Update () {
		
	}
	
}

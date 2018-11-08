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
		string rawBytesPath = Path.Combine(Application.persistentDataPath, "raw.txt");

		#if UNITY_ANDROID
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
		#endif
		
		if (GUILayout.Button("CreateRawBytes", GUILayout.Height(100), GUILayout.Width(100)))
		{
			if (File.Exists(rawBytesPath))
			{
				File.Delete(rawBytesPath);
			}

			using (var fs = File.Create(rawBytesPath))
			{
				using (var writer = new BinaryWriter(fs))
				{
					writer.Write("Hello Raw Bytes!");
				}
			}
		}

		if (GUILayout.Button("ReadRawBytes", GUILayout.Height(100), GUILayout.Width(100)))
		{
			var ptr = IntPtr.Zero;
			var size = ReadNativeByte.ReadRawBytes(rawBytesPath, ref ptr);
			if (size <= 0)
			{
				Debug.LogError(string.Format("read error errcode={0}", size.ToString()));
				return;
			}
			stream.SetLength(size);
			stream.Position = 0;
			Marshal.Copy(ptr, stream.GetBuffer(), 0, size);
			var reader = new BinaryReader(stream);
			Debug.Log(reader.ReadString());
			ReadNativeByte.ReleaseBytes(ptr);
		}
		
	}

	// Update is called once per frame
	void Update () {
		
	}
	
}

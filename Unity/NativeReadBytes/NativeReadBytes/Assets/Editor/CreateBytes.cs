using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;

public class CreateBytes {

	[MenuItem("Test/CreateBytes")]
	public static void Create()
	{
		var path = Path.Combine(Application.streamingAssetsPath, "Test.txt");
		if (File.Exists(path))
		{
			File.Delete(path);
		}
		using (var fs = File.Create(path))
		{
			using (var writer = new BinaryWriter(fs))
			{
				int a = 10;
				writer.Write(a);
				int b = 100;
				writer.Write(b);
			}
		}
	}
	
}

using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public class PngMaker
{
    public static void CreatePng(int width, int height, Color[] colors, string path)
    {
        if (width * height != colors.Length)
        {
            return;
        }

        Texture2D tex = new Texture2D(width, height, TextureFormat.ARGB32, false);
        tex.SetPixels(colors);
        tex.Apply();
        byte[] bytes = tex.EncodeToPNG();
        var directory = Path.GetDirectoryName(path);
        if (!Directory.Exists(directory))
        {
            Directory.CreateDirectory(directory);
        }
        using (var bw = new BinaryWriter(new FileStream(path, FileMode.Create)))
        {
            bw.Write(bytes);
        }
    }
}

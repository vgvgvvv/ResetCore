using System.Collections;
using System.Collections.Generic;
using System.IO;
using RayTracing;
using UnityEditor;
using UnityEngine;

public class TestPngMaker
{

    [MenuItem("RayTrace/Test/MakePng")]
    public static void CreateTextureTest()
    {
        int width = 1920;
        int height = 1080;

        Color[] colors = new Color[width * height];

        //画图
        for (int i = 0; i < height; i++)
        {
            //画横线
            for (int j = 0; j < width; j++)
            {
                colors[j + i * width] =
                    new Color(
                        (float) i / height,
                        (float) j / width,
                        0.2f);
            }
        }

        PngMaker.CreatePng(width, height, colors, Path.Combine(Application.streamingAssetsPath, "Test.png"));
        AssetDatabase.Refresh();

    }

    [MenuItem("RayTrace/Test/TraceWorld")]
    public static void CreateTrace()
    {
        var colors = new World().Trace();
        PngMaker.CreatePng(200, 100, colors, Path.Combine(Application.streamingAssetsPath, "Tracing.png"));
        AssetDatabase.Refresh();
    }

}

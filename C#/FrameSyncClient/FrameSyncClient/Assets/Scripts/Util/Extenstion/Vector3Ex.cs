using UnityEngine;
using System.Collections;


public static class Vector3Ex
{
    /// <summary>
    /// 改变X值
    /// </summary>
    /// <param name="vec"></param>
    /// <param name="x"></param>
    /// <returns></returns>
    public static Vector3 ChangeX(this Vector3 vec, float x)
    {
        Vector3 newVector =
            new Vector3(x, vec.y, vec.z);
        return newVector;
    }

    /// <summary>
    /// 改变Y值
    /// </summary>
    /// <param name="vec"></param>
    /// <param name="y"></param>
    /// <returns></returns>
    public static Vector3 ChangeY(this Vector3 vec, float y)
    {
        Vector3 newVector =
             new Vector3(vec.x, y, vec.z);
        return newVector;
    }

    /// <summary>
    /// 改变Z值
    /// </summary>
    /// <param name="vec"></param>
    /// <param name="z"></param>
    /// <returns></returns>
    public static Vector3 ChangeZ(this Vector3 vec, float z)
    {
        Vector3 newVector =
            new Vector3(vec.x, vec.y, z);
        return newVector;
    }

}

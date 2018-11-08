using System.Collections;
using UnityEngine;


public class NativeTextureManager : MonoBehaviour
{
    #if UNITY_ANDROID
    public static NativeTextureManager Instance { get; private set; }
    
    private void Awake()
    {
        Instance = this;
    }

    private bool isWaitingToUpdate = false;
    public void UpdateTexture()
    {
        if (isWaitingToUpdate)
        {
            return;
        }
        isWaitingToUpdate = true;
        StartCoroutine(IssuePluginEvent());
    }        
    
    private IEnumerator IssuePluginEvent()
    {
        yield return new WaitForEndOfFrame();
        GL.IssuePluginEvent(PngLib.GetPngRenderEventFunc(), 0);
        isWaitingToUpdate = false;
    }
    #endif
}
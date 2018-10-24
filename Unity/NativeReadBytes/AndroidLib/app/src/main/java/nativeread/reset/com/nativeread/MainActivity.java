package nativeread.reset.com.nativeread;

import android.os.Bundle;
import android.util.Log;

import com.unity3d.player.UnityPlayer;
import com.unity3d.player.UnityPlayerActivity;

public class MainActivity extends UnityPlayerActivity {

    static{
        System.loadLibrary("NativeLib");
    }

    @Override
    protected void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        Log.d("Unity", "SetAssetManager");
        SetAssetManager();
    }

    public void SetAssetManager(){
        NativeHelper.SetAssetManager(getAssets());
    }



}

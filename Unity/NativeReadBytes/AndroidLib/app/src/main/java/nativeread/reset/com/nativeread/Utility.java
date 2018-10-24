package nativeread.reset.com.nativeread;

import com.unity3d.player.UnityPlayer;

public class Utility {

    public static void Log(String log){
        UnityPlayer.UnitySendMessage("AndroidCallback", "Log", log);
    }

    public static void LogWarn(String log){
        UnityPlayer.UnitySendMessage("AndroidCallback", "LogWarn", log);
    }

    public static void LogError(String log){
        UnityPlayer.UnitySendMessage("AndroidCallback", "LogError", log);
    }
}

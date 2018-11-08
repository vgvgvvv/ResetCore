package videoplayer.test.com.videoplayer;

import android.content.Intent;
import android.os.Bundle;
import android.os.PersistableBundle;
import android.support.annotation.Nullable;

import com.unity3d.player.UnityPlayerActivity;

public class MainActivity extends UnityPlayerActivity {

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState, @Nullable PersistableBundle persistentState) {
        super.onCreate(savedInstanceState, persistentState);

    }

    /**
     * 播放动画
     */
    public void StartMoviePlayer() {
        Intent intent = new Intent(this, VideoPlayerActivity.class);
        intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
        this.startActivity(intent);
    }
}

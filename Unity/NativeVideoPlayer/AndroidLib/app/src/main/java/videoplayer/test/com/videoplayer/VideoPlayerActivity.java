package videoplayer.test.com.videoplayer;

import android.app.Activity;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Bundle;
import android.os.PersistableBundle;
import android.support.annotation.Nullable;
import android.util.Log;
import android.view.View;
import android.widget.VideoView;

import com.unity3d.player.UnityPlayer;

public class VideoPlayerActivity extends Activity{

    VideoView videoView;
    int moviepos = -1;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState, @Nullable PersistableBundle persistentState) {
        super.onCreate(savedInstanceState, persistentState);
        setContentView(GetR("layout", "movie_activity"));
        videoView = findViewById(GetR("id", "videoView"));

    }

    private int GetR(String field, String name){
        return Utility.GetR(this, field, name);
    }

    @Override
    protected void onStart() {
        super.onStart();
        String uriPath = "android.resource://" + getPackageName() + "/" + GetR("raw","cg");
        Uri uri = Uri.parse(uriPath);
        videoView.setVideoURI(uri);
        // videoView.setVideoPath(Environment.getExternalStorageDirectory() + "/" + filePath);
        videoView.requestFocus();
        videoView.start();
        videoView.setOnCompletionListener(new MediaPlayer.OnCompletionListener()
        {
            @Override
            public void onCompletion(MediaPlayer mp)
            {
                UnityPlayer.UnitySendMessage("StartScene", "MovieOver", "1");
                VideoPlayerActivity.this.finish();
                overridePendingTransition(0, 0);
            }
        });
        videoView.setOnErrorListener(new MediaPlayer.OnErrorListener() {

            @Override
            public boolean onError(MediaPlayer mp, int what, int extra) {
                // TODO Auto-generated method stub
                Log.d("Unity", "on media player error : " + what + ", " + extra);
                return false;
            }
        });

        videoView.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View v, boolean hasFocus) {
                Log.d("Unity", "on media player focus change : " + hasFocus);
            }
        });

    }

    @Override
    public void onPause()
    {
        moviepos = videoView.getCurrentPosition();
        //videoView.stopPlayback();
        videoView.pause();

        Log.d("Unity", "On Movie Player Pause, pos : " + moviepos);
        super.onPause();
    }

    @Override
    public void onResume() {

        Log.d("Unity", "On Movie Player Resume, pos : " + moviepos);
        // Resume video player
        if(moviepos >= 0) {
            videoView.start();
            videoView.seekTo(moviepos);
            moviepos = -1;
        }

        super.onResume();
    }

    @Override
    public void onDestroy() {

        Log.d("Unity", "On Movie Player Destroy");
        super.onDestroy();
    }

    /**
     * 拦截返回键消息
     */
    @Override
    public void onBackPressed() { }
}

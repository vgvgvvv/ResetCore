package videoplayer.test.com.videoplayer;

import android.app.Activity;
import android.content.ContentResolver;
import android.content.Context;
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
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(GetR("layout", "movie_activity"));
        Log.d("Unity", "onCreate: ");
        videoView = (VideoView)findViewById(GetR("id", "videoView"));
        if(videoView == null){
            Log.e("Unity", "videoView not found");
        }
    }

    private int GetR(String field, String name){
        return Utility.GetR(this, field, name);
    }

    @Override
    protected void onStart() {
        super.onStart();
        Log.d("Unity", "onStart: ");
        Uri uri = resourceToUri(this, GetR("raw", "cg"));
        Log.d("Unity", "url is " + uri.toString());
        videoView.setVideoURI(uri);
        videoView.requestFocus();
        videoView.start();
        videoView.setOnCompletionListener(new MediaPlayer.OnCompletionListener()
        {
            @Override
            public void onCompletion(MediaPlayer mp)
            {
                UnityPlayer.UnitySendMessage("Canvas", "Hide", "");
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

    public static Uri resourceToUri(Context context, int resID) {
        return Uri.parse(ContentResolver.SCHEME_ANDROID_RESOURCE + "://" +
                context.getResources().getResourcePackageName(resID) + '/' +
                context.getResources().getResourceTypeName(resID) + '/' +
                context.getResources().getResourceEntryName(resID) );
    }
}

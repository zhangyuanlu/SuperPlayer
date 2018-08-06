package com.zyl.superplayer;

import android.Manifest;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.provider.Settings;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity implements View.OnClickListener,SeekBar.OnSeekBarChangeListener,Runnable{

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private String[] permissions={Manifest.permission.WRITE_EXTERNAL_STORAGE};
    private List<String> mPermissionList=new ArrayList<>();
    private final int mRequestCode=100;
    private AlertDialog mPermissionDialog;
    private SeekBar seekBar;
    private Button bt_play,bt_prev,bt_next;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(Build.VERSION.SDK_INT>=21){
            getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                    | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                    | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                    | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                    | View.SYSTEM_UI_FLAG_FULLSCREEN
                    | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
        }
        setContentView(R.layout.activity_main);
        initView();
        if(Build.VERSION.SDK_INT>=Build.VERSION_CODES.M){
            for(int i=0;i<permissions.length;i++){
                if(ContextCompat.checkSelfPermission(this,permissions[i])!=PackageManager.PERMISSION_GRANTED){
                    mPermissionList.add(permissions[i]);
                }
            }
            if(!mPermissionList.isEmpty()){
                ActivityCompat.requestPermissions(this,permissions,mRequestCode);
            }else{

            }
        }
    }

    private void initView() {
        seekBar=findViewById(R.id.seekBar);
        bt_play=findViewById(R.id.bt_play);
        bt_prev=findViewById(R.id.bt_pre);
        bt_next=findViewById(R.id.bt_next);
        bt_play.setOnClickListener(this);
        bt_prev.setOnClickListener(this);
        bt_next.setOnClickListener(this);
        seekBar.setMax(1000);
        seekBar.setOnSeekBarChangeListener(this);
        new Thread(this).start();
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if(Build.VERSION.SDK_INT>=21){
            getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                    | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                    | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                    | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                    | View.SYSTEM_UI_FLAG_FULLSCREEN
                    | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        boolean hasPermissionDismiss=false;
        if(mRequestCode==requestCode){
            for(int i=0;i<grantResults.length;i++){
                if(grantResults[i]==-1){
                    hasPermissionDismiss=true;
                }
                if(hasPermissionDismiss){
                    showPermissionDialog();
                }else{

                }
            }
        }
    }

    private void showPermissionDialog(){
        if(mPermissionDialog==null){
            mPermissionDialog=new AlertDialog.Builder(this)
                    .setMessage("本应用依赖必要的权限，请予以授权")
                    .setPositiveButton("设置", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            cancelPermissionDialog();
                            Uri packUri=Uri.parse("package:"+MainActivity.this.getPackageName());
                            Intent intent=new Intent(Settings.ACTION_APPLICATION_DETAILS_SETTINGS,packUri);
                            startActivity(intent);
                        }
                    })
                    .setNegativeButton("取消", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            cancelPermissionDialog();
                        }
                    })
                    .create();
            mPermissionDialog.show();
        }
    }
    private void cancelPermissionDialog(){
        mPermissionDialog.cancel();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.bt_play:{
                playOrPause();
                break;
            }
            case R.id.bt_pre:{

                break;
            }
            case R.id.bt_next:{

                break;
            }
            default:
                break;
        }
    }

    @Override
    public void run() {
        while (true){
            seekBar.setProgress((int)(getPlayPos()*1000));
            try {
                Thread.sleep(40);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    protected void onStop() {
        super.onStop();
        if(!isPausing()){
            playOrPause();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
     //   closePlayer();
    }

    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {

    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {
        seekPlayPos((double) seekBar.getProgress()/(double)seekBar.getMax());
    }

    public native double getPlayPos();
    public native void seekPlayPos(double pos);
    public native void playOrPause();
    public native void closePlayer();
    public native boolean isPausing();
}

package com.zyl.superplayer;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.Surface;
import android.view.SurfaceHolder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class XPlayView extends GLSurfaceView implements SurfaceHolder.Callback,GLSurfaceView.Renderer{
    public XPlayView(Context context) {
        super(context);
    }

    public XPlayView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        //初始化opengl EGL显示
        initView(holder.getSurface());
        setRenderer(this);  //Android O以上需要
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {

    }

    @Override
    public void onDrawFrame(GL10 gl) {

    }


    /**
     * 初始化opengl EGL显示
     * @param holder
     */
    public native void initView(Surface surface);
}

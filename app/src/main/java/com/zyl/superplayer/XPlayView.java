package com.zyl.superplayer;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.Surface;
import android.view.SurfaceHolder;

public class XPlayView extends GLSurfaceView implements SurfaceHolder.Callback {
    public XPlayView(Context context) {
        super(context);
    }

    public XPlayView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        initView(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    /**
     * 初始化opengl EGL显示
     * @param holder
     */
    public native void initView(Surface surface);
}

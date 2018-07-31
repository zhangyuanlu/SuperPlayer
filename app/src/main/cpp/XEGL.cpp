//
// Created by 张远路 on 2018/7/24.
//

#include "XEGL.h"
#include "android/native_window_jni.h"
#include "EGL/egl.h"
#include "XLog.h"
class CXEGL:public XEGL{
public:
    EGLDisplay eglDisplay=EGL_NO_DISPLAY;
    EGLSurface eglSurface=EGL_NO_SURFACE;
    EGLContext eglContext=EGL_NO_CONTEXT;
    virtual void Draw()
    {
        if(eglDisplay==EGL_NO_DISPLAY||eglSurface==EGL_NO_SURFACE)
        {
            XLOGE("EGL_NO_DISPLAY OR EGL_NO_SURFACE");
            return;
        }
        eglSwapBuffers(eglDisplay,eglSurface);
    }
    virtual bool Init(void *window)
    {
        ANativeWindow *aNativeWindow=(ANativeWindow*)window;

        //初始化EGL
        //1.获取EGLDisplay对象，显示设备
        eglDisplay=eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if(eglDisplay==EGL_NO_DISPLAY)
        {
            XLOGE("eglGetDisplay failed");
            return false;
        }
        XLOGI("eglGetDisplay success");
        //2.初始化display
        if(eglInitialize(eglDisplay,0,0)!=EGL_TRUE)
        {
            XLOGE("eglInitialize failed");
            return false;
        }
        XLOGI("eglInitialize success");
        //3.获取配置并创建surface
        EGLint configSpec[]={
                EGL_RED_SIZE,8,
                EGL_GREEN_SIZE,8,
                EGL_BLUE_SIZE,8,
                EGL_SURFACE_TYPE,EGL_WINDOW_BIT,
                EGL_NONE
        };
        EGLConfig config=0;
        EGLint numConfigs=0;
        if(eglChooseConfig(eglDisplay,configSpec,&config,1,&numConfigs)!=EGL_TRUE)
        {
            XLOGE("eglChooseConfig failed");
            return false;
        }
        XLOGI("eglChooseConfig success");
        eglSurface=eglCreateWindowSurface(eglDisplay,config,aNativeWindow,NULL);
        if(eglSurface==EGL_NO_SURFACE)
        {
            XLOGE("eglCreateWindowSurface failed");
            return false;
        }
        XLOGI("eglCreateWindowSurface success");
        //4.创建并打开EGL上下文
        const EGLint ctxAttr[]={EGL_CONTEXT_CLIENT_VERSION,2,EGL_NONE};
        eglContext=eglCreateContext(eglDisplay,config,EGL_NO_CONTEXT,ctxAttr);
        if(eglContext==EGL_NO_CONTEXT)
        {
            XLOGE("eglCreateContext failed");
            return false;
        }
        XLOGI("eglCreateContext success");
        if(eglMakeCurrent(eglDisplay,eglSurface,eglSurface,eglContext)!=EGL_TRUE)
        {
            XLOGE("eglMakeCurrent failed");
            return false;
        }
        XLOGI("eglMakeCurrent success");
        return true;
    }
};
XEGL *XEGL::Get()
{
    static CXEGL egl;
    return &egl;
}
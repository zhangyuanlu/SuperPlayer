#include <jni.h>
#include <string>
#include "IDemux.h"
#include "FFDemux.h"
#include "IDecode.h"
#include "FFDecode.h"
#include "android/native_window_jni.h"
#include "XEGL.h"
#include "XShader.h"
#include "IVideoView.h"
#include "GLVideoView.h"
#include "FFResample.h"
#include "IAudioPlay.h"
#include "SLAudioPlay.h"

class TestObs:public IObserver{
public:
    void Update(XData data)
    {
    //    XLOGI("TestObs Update size is %d!",data.size);
    }
};

IVideoView *view=NULL;

extern "C" JNIEXPORT jstring

JNICALL
Java_com_zyl_superplayer_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    ////////测试代码///////////
    TestObs *testObs=new TestObs();
    IDemux *de=new FFDemux();
 //   de->AddObs(testObs);
    de->Open("/mnt/sdcard/ww.mp4");

    IDecode *vdecode=new FFDecode();
    vdecode->Open(de->GetVParam());

    IDecode *adecode=new FFDecode();
    adecode->Open(de->GetAParam());

    de->AddObs(vdecode);
    de->AddObs(adecode);

    view=new GLVideoView();
    vdecode->AddObs(view);

    IResample *resample=new FFResample();
    XParameter outPara=de->GetAParam();

    resample->Open(de->GetAParam(),outPara);
    adecode->AddObs(resample);

    IAudioPlay *audioPlay=new SLAudioPlay();
    audioPlay->StartPlay(outPara);
    resample->AddObs(audioPlay);


    de->Start();
    vdecode->Start();
    adecode->Start();
//    XSleep(3000);
//    de->Stop();

    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zyl_superplayer_XPlayView_initView(JNIEnv *env, jobject instance, jobject surface) {

    // TODO
    ANativeWindow *aNativeWindow=ANativeWindow_fromSurface(env,surface);
    view->SetRender(aNativeWindow);
  //  XEGL::Get()->Init(aNativeWindow);
  //  XShader shader;
  //  shader.Init();
}
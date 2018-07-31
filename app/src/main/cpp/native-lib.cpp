#include <jni.h>
#include <string>
#include "android/native_window_jni.h"
#include "FFPlayerBuilder.h"
#include "IPlayerProxy.h"
static IPlayer *player=NULL;
extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm,void *res)
{
    /*
    FFPlayerBuilder::InitDXVA(vm);

    player=FFPlayerBuilder::Get()->BuilderPlayer();
    player->Open("/sdcard/ww.mp4");
    player->StartPlay();
     */
    IPlayerProxy::Get()->Init(vm);
    IPlayerProxy::Get()->Open("/sdcard/ww.mp4");
    IPlayerProxy::Get()->StartPlay();
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zyl_superplayer_XPlayView_initView(JNIEnv *env, jobject instance, jobject surface) {

    // TODO
    ANativeWindow *aNativeWindow=ANativeWindow_fromSurface(env,surface);
  //  if(player)
  //      player->InitView(aNativeWindow);
    IPlayerProxy::Get()->InitView(aNativeWindow);
  //  XEGL::Get()->Init(aNativeWindow);
  //  XShader shader;
  //  shader.Init();
}
#include <jni.h>
#include <string>
#include "android/native_window_jni.h"
#include "FFPlayerBuilder.h"
#include "IPlayerProxy.h"
#include "IDemux.h"

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
    IPlayerProxy::Get()->InitView(aNativeWindow);
}extern "C"
JNIEXPORT jdouble JNICALL
Java_com_zyl_superplayer_MainActivity_getPlayPos(JNIEnv *env, jobject instance) {

    // TODO
    return IPlayerProxy::Get()->PlayPos();
}extern "C"
JNIEXPORT void JNICALL
Java_com_zyl_superplayer_MainActivity_seekPlayPos(JNIEnv *env, jobject instance, jdouble pos) {

    // TODO
   IPlayerProxy::Get()->SeekPos(pos);
}extern "C"
JNIEXPORT void JNICALL
Java_com_zyl_superplayer_MainActivity_playOrPause(JNIEnv *env, jobject instance) {

    // TODO
    IPlayerProxy::Get()->SetPause(!IPlayerProxy::Get()->IsPause());
}extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zyl_superplayer_MainActivity_isPausing(JNIEnv *env, jobject instance) {

    // TODO
    return IPlayerProxy::Get()->IsPause();
}extern "C"
JNIEXPORT void JNICALL
Java_com_zyl_superplayer_XPlayView_closePlayer(JNIEnv *env, jobject instance) {

    // TODO
    IPlayerProxy::Get()->Close();
}extern "C"
JNIEXPORT jint JNICALL
Java_com_zyl_superplayer_MainActivity_getTotalMs(JNIEnv *env, jobject instance) {

    // TODO
    return IPlayerProxy::Get()->GetTotalMs();
}
//
// Created by 张远路 on 2018/7/26.
//

#include "SLAudioPlay.h"
#include "XLog.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

static SLObjectItf slObjectItf=NULL;
static SLEngineItf slEngineItf=NULL;
//创建混音器
static SLObjectItf objectMix=NULL;
//创建播放器
static SLObjectItf player=NULL;
static SLPlayItf playItf=NULL;
static SLAndroidSimpleBufferQueueItf pcmQueue=NULL;

SLAudioPlay::SLAudioPlay()
{
    buff=new unsigned char[1024*1024];
}
SLAudioPlay::~SLAudioPlay()
{
    delete buff;
    buff=0;
}

static SLEngineItf CreateSL()
{
    SLresult re;
    SLEngineItf engineItf;
    re=slCreateEngine(&slObjectItf,0,0,0,0,0);
    if(re!=SL_RESULT_SUCCESS) return NULL;
    re=(*slObjectItf)->Realize(slObjectItf,SL_BOOLEAN_FALSE);
    if(re!=SL_RESULT_SUCCESS) return NULL;
    re=(*slObjectItf)->GetInterface(slObjectItf,SL_IID_ENGINE,&engineItf);
    if(re!=SL_RESULT_SUCCESS) return NULL;
    return engineItf;
}
static void PcmCall(SLAndroidSimpleBufferQueueItf bufferQueueItf,void *context)
{
    SLAudioPlay *slAudioPlay=(SLAudioPlay*)context;
    if(!slAudioPlay)
    {
        XLOGE("PcmCall failed: context is null");
        return;
    }
    slAudioPlay->PlayCall((void *)bufferQueueItf);
}
void SLAudioPlay::PlayCall(void *bufQueue)
{
    //阻塞
    XData xData=GetData();
    if(xData.size<=0)
    {
        XLOGE("GetData() size is 0");
        return;
    }
    if(!buff)
    {
        return;
    }
    memcpy(buff,xData.data,xData.size);
    mux.lock();
    if(pcmQueue&&(*pcmQueue))
        (*pcmQueue)->Enqueue(pcmQueue,buff,xData.size);
    mux.unlock();
    xData.Drop();
}
void SLAudioPlay::Close()
{
    IAudioPlay::Clear();
    mux.lock();
    //停止播放
    if(playItf&&(*playItf))
    {
        (*playItf)->SetPlayState(playItf,SL_PLAYSTATE_STOPPED);
    }
    //清理播放队列
    if(pcmQueue&&(*pcmQueue))
    {
        (*pcmQueue)->Clear(pcmQueue);
    }
    //销毁player对象
    if(player&&(*player))
    {
        (*player)->Destroy(player);
    }
    //销毁混音器
    if(objectMix&&(*objectMix))
    {
        (*objectMix)->Destroy(objectMix);
    }
    //销毁播放引擎
    if(slObjectItf&&(*slObjectItf))
    {
        (*slObjectItf)->Destroy(slObjectItf);
    }
    slObjectItf=NULL;
    slEngineItf=NULL;
    objectMix=NULL;
    player=NULL;
    playItf=NULL;
    pcmQueue=NULL;
    mux.unlock();
}
bool SLAudioPlay::StartPlay(XParameter out)
{
    Close();
    mux.lock();
    //创建引擎
    slEngineItf=CreateSL();
    if(slEngineItf)
    {
        XLOGI("create SLEngine success\n");
    }
    else
    {
        mux.unlock();
        XLOGE("create SLEngine failed\n");
        return false;
    }
    //创建混音器
    SLresult ret=0;
    ret=(*slEngineItf)->CreateOutputMix(slEngineItf,&objectMix,0,0,0);
    if(ret!=SL_RESULT_SUCCESS)
    {
        mux.unlock();
        XLOGE("CreateOutputMix failed\n");
        return false;
    }
    ret=(*objectMix)->Realize(objectMix,SL_BOOLEAN_FALSE);
    if(ret!=SL_RESULT_SUCCESS)
    {
        mux.unlock();
        XLOGE("ObjectMix Realize failed\n");
        return false;
    }

    SLDataLocator_OutputMix outputMix={SL_DATALOCATOR_OUTPUTMIX,objectMix};
    SLDataSink audioSink={&outputMix,0};

    //配置音频信息
    //缓冲队列
    SLDataLocator_AndroidSimpleBufferQueue bufferQueue={SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,10};
    //音频格式
    SLDataFormat_PCM pcm={
            SL_DATAFORMAT_PCM,
            (SLuint32)out.channels,//声道数
            (SLuint32)out.sample_rate*1000,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT|SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN //字节序，小端
    };
    SLDataSource dataSource={&bufferQueue,&pcm};

    //创建播放器
    const SLInterfaceID ids[]={SL_IID_BUFFERQUEUE};
    const SLboolean req[]={SL_BOOLEAN_TRUE};
    ret=(*slEngineItf)->CreateAudioPlayer(slEngineItf,&player,&dataSource,&audioSink,
                                        sizeof(ids)/sizeof(SLInterfaceID),ids,req);
    if(ret!=SL_RESULT_SUCCESS)
    {
        mux.unlock();
        XLOGE("Create Audio Player failed!\n");
        return false;
    }
    else
    {
        XLOGI("Create Audio Player success!\n");
    }
    (*player)->Realize(player,SL_BOOLEAN_FALSE);
    //获取Player接口
    ret=(*player)->GetInterface(player,SL_IID_PLAY,&playItf);
    if(ret!=SL_RESULT_SUCCESS)
    {
        mux.unlock();
        XLOGE("Audio Player GetInterface failed!\n");
        return false;
    }
    else
    {
        XLOGI("Audio Player GetInterface success!\n");
    }
    ret=(*player)->GetInterface(player,SL_IID_BUFFERQUEUE,&pcmQueue);
    if(ret!=SL_RESULT_SUCCESS)
    {
        mux.unlock();
        XLOGE("Audio Player BufferQueue GetInterface failed!\n");
        return false;
    }
    else
    {
        XLOGI("Audio Player BufferQueue GetInterface success!\n");
    }

    //设置回调函数，在播放队列为空时调用
    (*pcmQueue)->RegisterCallback(pcmQueue,PcmCall,this);

    //设置为播放状态
    (*playItf)->SetPlayState(playItf,SL_PLAYSTATE_PLAYING);

    //启动队列回调
    (*pcmQueue)->Enqueue(pcmQueue,"",1);
    isExit= false;
    XLOGI("audio play StartPlay success");
    mux.unlock();
    return true;
}
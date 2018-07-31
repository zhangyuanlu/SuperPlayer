//
// Created by 张远路 on 2018/7/30.
//

#include "IPlayer.h"
#include "IDemux.h"
#include "IDecode.h"
#include "IResample.h"
#include "IAudioPlay.h"
#include "IVideoView.h"
#include "XLog.h"

IPlayer* IPlayer::Get(unsigned char index)
{
    static IPlayer iPlayer[256];
    return &iPlayer[index];
}

bool IPlayer::Open(const char *path)
{
    mut.lock();
    if(!iDemux||!iDemux->Open(path))
    {
        XLOGE("IDemux Open %s Failed",path);
        mut.unlock();
        return false;
    }
    if(!vDecode||!vDecode->Open(iDemux->GetVParam(),isDXVA))
    {
        XLOGE("VDecode Open %s Failed",path);
        //return false;//解码后可能是原始数据，故不必要返回false
    }
    if(!aDecode||!aDecode->Open(iDemux->GetAParam()))
    {
        XLOGE("ADecode Open %s Failed",path);
        //return false;
    }
    if(outPara.sample_rate<=0)
    {
        outPara=iDemux->GetAParam();
    }
    if(!iResample||!iResample->Open(iDemux->GetAParam(),outPara))
    {
        XLOGE("IResample Open %s Failed",path);
        //return false;
    }
    mut.unlock();
    return true;
}
bool IPlayer::StartPlay()
{
    mut.lock();
    if(!iDemux || !iDemux->Start())
    {
        XLOGE("demux->Start failed!");
        mut.unlock();
        return false;
    }
    if(aDecode)
        aDecode->Start();
    if(iAudioPlay)
        iAudioPlay->StartPlay(outPara);
    if(vDecode)
        vDecode->Start();
    XThread::Start();
    mut.unlock();
    return true;
}
void IPlayer::InitView(void *window)
{
    if(iVideoView)
        iVideoView->SetRender(window);
}

void IPlayer::Main()
{
    while(!isExit)
    {
        mut.lock();
        if(!iAudioPlay||!vDecode)
        {
            mut.unlock();
            XSleep(2);
            continue;
        }
        //同步
        //获取音频的pts后通知视频
        vDecode->synPts=iAudioPlay->pts;
        mut.unlock();
        XSleep(2);
    }
}
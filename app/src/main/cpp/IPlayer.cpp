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
void IPlayer::Close()
{
    mut.lock();

    //先关闭主体线程，再清理观察者
    //同步线程
    XThread::Stop();
    //解封装
    if(iDemux)
    {
        iDemux->Stop();
    }
    //解码
    if(vDecode)
    {
        vDecode->Stop();
    }
    if(aDecode)
    {
        aDecode->Stop();
    }
    if(iAudioPlay)
    {
        iAudioPlay->Stop();
    }
    //清理缓冲队列
    if(vDecode)
    {
        vDecode->Clear();
    }
    if(aDecode)
    {
        aDecode->Clear();
    }
    if(iAudioPlay)
    {
        iAudioPlay->Clear();
    }
    //清理资源
    if(iAudioPlay)
    {
        iAudioPlay->Close();
    }
    if(iVideoView)
    {
        iVideoView->Close();
    }
    if(vDecode)
    {
        vDecode->Close();
    }
    if(aDecode)
    {
        aDecode->Close();
    }
    if(iDemux)
    {
        iDemux->Close();
    }

    mut.unlock();
}
int IPlayer::GetTotalMs()
{
    int ms=0;
    mut.lock();
    if(iDemux)
    {
        ms=iDemux->totalMs;
    }
    mut.unlock();
    return ms;
}
double IPlayer::PlayPos()
{
    double pos=0.0;
    mut.lock();
    int total=0;
    if(iDemux)
    {
        total=iDemux->totalMs;
    }
    if(total>0)
    {
        if(vDecode)
        {
            pos=(double)vDecode->pts/(double)total;
        }
    }
    mut.unlock();
    return pos;
}
bool IPlayer::SeekPos(double pos)
{
    if(!iDemux)
    {
        return false;
    }
    bool ret=false;
    //暂停所有线程
    SetPause(true);
    mut.lock();
    //清理缓冲
    if(vDecode)
    {
        vDecode->Clear();
    }
    if(aDecode)
    {
        aDecode->Clear();
    }
    if(iAudioPlay)
    {
        iAudioPlay->Clear();
    }
    ret=iDemux->Seek(pos);  //seek跳转到关键帧
    //解码到实际需要显示的帧
    if(!vDecode)
    {
        mut.unlock();
        SetPause(false);
        return ret;
    }
    int seekPts=pos*iDemux->totalMs;
    while (!isExit)
    {
        XData pkt=iDemux->Read();
        if(pkt.size<=0)
        {
            break;
        }
        if(pkt.isAudio)
        {
            if(pkt.pts<seekPts)
            {
                pkt.Drop();
                continue;
            }
            //写入缓冲队列
            iDemux->Notify(pkt);
            continue;
        }
        //解码需要显示的帧之前的数据
        vDecode->SendPacket(pkt);
        pkt.Drop();
        XData data=vDecode->RecvFrame();
        if(data.size<=0)
        {
            continue;
        }
        if(data.pts>=seekPts)
        {
            //vDecode->Notify(pkt);
            break;
        }
    }
    mut.unlock();
    SetPause(false);
    return ret;
}
void IPlayer::SetPause(bool isPause)
{
    mut.lock();
    XThread::SetPause(isPause);
    if(iDemux)
    {
        iDemux->SetPause(isPause);
    }
    if(vDecode)
    {
        vDecode->SetPause(isPause);
    }
    if(aDecode)
    {
        aDecode->SetPause(isPause);
    }
    if(iAudioPlay)
    {
        iAudioPlay->SetPause(isPause);
    }
    mut.unlock();
}
bool IPlayer::Open(const char *path)
{
    Close();
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
    //if(outPara.sample_rate<=0)
    //{
        outPara=iDemux->GetAParam();
    //}
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

    if(iAudioPlay)
        iAudioPlay->StartPlay(outPara);

    if(aDecode)
        aDecode->Start();

    if(vDecode)
        vDecode->Start();

    if(!iDemux || !iDemux->Start())
    {
        XLOGE("demux->Start failed!");
        mut.unlock();
        return false;
    }
    XThread::Start();
    mut.unlock();
    return true;
}
void IPlayer::InitView(void *window)
{
    if(iVideoView){
        iVideoView->Close();
        iVideoView->SetRender(window);
    }
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
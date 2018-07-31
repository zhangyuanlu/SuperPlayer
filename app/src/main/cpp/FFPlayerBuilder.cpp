//
// Created by 张远路 on 2018/7/31.
//

#include "FFPlayerBuilder.h"
#include "FFDemux.h"
#include "FFDecode.h"
#include "FFResample.h"
#include "GLVideoView.h"
#include "SLAudioPlay.h"

IDemux *FFPlayerBuilder::CreateDemux()
{
    IDemux *iDemux=new FFDemux();
    return iDemux;
}
IDecode *FFPlayerBuilder::CreateDecode()
{
    IDecode *iDecode=new FFDecode();
    return iDecode;
}
IResample *FFPlayerBuilder::CreateResample()
{
    IResample *iResample=new FFResample();
    return iResample;
}
IVideoView *FFPlayerBuilder::CreateVideoView()
{
    IVideoView *iVideoView=new GLVideoView();
    return iVideoView;
}
IAudioPlay *FFPlayerBuilder::CreateAudioPlay()
{
    IAudioPlay *iAudioPlay=new SLAudioPlay();
    return iAudioPlay;
}
IPlayer *FFPlayerBuilder::CreatePlayer(unsigned char index)
{
    return IPlayer::Get(index);
}
//初始化硬解码
void FFPlayerBuilder::InitDXVA(void *vm)
{
    FFDecode::InitDXVA(vm);
}
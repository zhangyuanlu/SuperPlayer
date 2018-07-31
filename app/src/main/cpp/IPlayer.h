//
// Created by 张远路 on 2018/7/30.
//

#ifndef SUPERPLAYER_IPLAYER_H
#define SUPERPLAYER_IPLAYER_H


#include "XThread.h"
#include "XParameter.h"

class IDemux;
class IDecode;
class IResample;
class IAudioPlay;
class IVideoView;

class IPlayer : public XThread
{
public:
    static IPlayer *Get(unsigned char index=0);
    virtual bool Open(const char *path);
    virtual bool StartPlay();
    virtual void InitView(void *window);

    bool isDXVA= true;
    IDemux *iDemux=0;
    IDecode *aDecode=0;
    IDecode *vDecode=0;
    IResample *iResample=0;
    IVideoView *iVideoView=0;
    IAudioPlay *iAudioPlay=0;
    XParameter outPara; //音频输出配置

protected:
    IPlayer(){};
};


#endif //SUPERPLAYER_IPLAYER_H

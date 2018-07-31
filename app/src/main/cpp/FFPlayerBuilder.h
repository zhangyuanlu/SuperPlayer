//
// Created by 张远路 on 2018/7/31.
//

#ifndef SUPERPLAYER_FFPLAYERBUILDER_H
#define SUPERPLAYER_FFPLAYERBUILDER_H

#include "IPlayerBuilder.h"

class FFPlayerBuilder: public IPlayerBuilder
{
public:
    static FFPlayerBuilder *Get()
    {
        static FFPlayerBuilder ffPlayerBuilder;
        return &ffPlayerBuilder;
    }
    //初始化硬解码
    static void InitDXVA(void *vm);
protected:
    FFPlayerBuilder(){}
    virtual IDemux *CreateDemux();
    virtual IDecode *CreateDecode();
    virtual IResample *CreateResample();
    virtual IVideoView *CreateVideoView();
    virtual IAudioPlay *CreateAudioPlay();
    virtual IPlayer *CreatePlayer(unsigned char index=0);
};


#endif //SUPERPLAYER_FFPLAYERBUILDER_H

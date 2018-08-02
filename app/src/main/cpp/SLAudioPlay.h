//
// Created by 张远路 on 2018/7/26.
//

#ifndef SUPERPLAYER_SLAUDIOPLAY_H
#define SUPERPLAYER_SLAUDIOPLAY_H


#include "IAudioPlay.h"

class SLAudioPlay : public IAudioPlay{
public:
    virtual bool StartPlay(XParameter out);
    virtual void Close();
    void PlayCall(void *bufQueue);

    SLAudioPlay();
    virtual ~SLAudioPlay();

protected:
    unsigned char *buff=0;
    std::mutex mux;
};


#endif //SUPERPLAYER_SLAUDIOPLAY_H

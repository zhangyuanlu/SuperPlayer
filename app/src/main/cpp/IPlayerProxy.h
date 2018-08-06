//
// Created by 张远路 on 2018/7/31.
//

#ifndef SUPERPLAYER_IPLAYERPROXY_H
#define SUPERPLAYER_IPLAYERPROXY_H


#include <mutex>
#include "IPlayer.h"

class IPlayerProxy : public IPlayer
{
public:
    static IPlayerProxy *Get()
    {
        static IPlayerProxy proxy;
        return &proxy;
    }
    void Init(void *vm=0);
    virtual bool Open(const char *path);
    virtual void Close();
    virtual bool StartPlay();
    virtual void InitView(void *window);
    virtual double PlayPos();
    virtual bool SeekPos(double pos);
    virtual void SetPause(bool isPause);
    virtual bool IsPause();

protected:
    IPlayerProxy(){}
    IPlayer *player=0;
    std::mutex mux;
};


#endif //SUPERPLAYER_IPLAYERPROXY_H

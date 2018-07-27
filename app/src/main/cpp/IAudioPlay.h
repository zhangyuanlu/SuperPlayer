//
// Created by 张远路 on 2018/7/26.
//

#ifndef SUPERPLAYER_IAUDIOPLAY_H
#define SUPERPLAYER_IAUDIOPLAY_H


#include <list>
#include "IObserver.h"
#include "XParameter.h"

class IAudioPlay : public IObserver{
public:
    //缓冲满之后阻塞
    virtual void Update(XData xData);

    //获取缓冲数据，如没有则阻塞
    virtual XData GetData();

    virtual bool StartPlay(XParameter out)=0;

    //最大缓冲
    int maxFrame=100;

protected:
    std::list <XData> frames;
    std::mutex framesMutex;
};


#endif //SUPERPLAYER_IAUDIOPLAY_H

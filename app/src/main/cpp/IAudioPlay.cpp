//
// Created by 张远路 on 2018/7/26.
//

#include "IAudioPlay.h"
#include "XLog.h"

XData IAudioPlay::GetData()
{
    XData d;

    while(!isExit)
    {
        framesMutex.lock();
        if(!frames.empty())
        {
            d = frames.front();
            frames.pop_front();
            framesMutex.unlock();
            return d;
        }
        framesMutex.unlock();
        XSleep(1);
    }
    return d;
}
void IAudioPlay::Update(XData data)
{
    //XLOGI("IAudioPlay::Update %d",data.size);
    //压入缓冲队列
    if(data.size<=0|| !data.data) return;
    while(!isExit)
    {
        framesMutex.lock();
        if(frames.size() > maxFrame)
        {
            framesMutex.unlock();
            XSleep(1);
            continue;
        }
        frames.push_back(data);
        framesMutex.unlock();
        break;
    }
}
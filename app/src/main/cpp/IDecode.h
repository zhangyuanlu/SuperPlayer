//
// Created by 张远路 on 2018/7/23.
//

#ifndef SUPERPLAYER_IDECODE_H
#define SUPERPLAYER_IDECODE_H

#include "XParameter.h"
#include "IObserver.h"
#include "list"

//解码接口，支持硬解码
class IDecode :public IObserver{
public:
    //打开解码器
    virtual bool Open(XParameter xParameter)=0;

    //future模型，发送数据到线程解码
    virtual bool SendPacket(XData xData)=0;

    //从线程中获取解码结果，再次调用会复用上次空间，线程不安全
    virtual XData RecvFrame()=0;

    //由主体Notify的数据,阻塞
    virtual void Update(XData xData);

    bool isAudio=false;

    //最大的队列缓冲
    int maxList=100;

protected:
    virtual void Main();
    //读取缓冲
    std::list<XData> packs;
    std::mutex packsMutex;
};


#endif //SUPERPLAYER_IDECODE_H

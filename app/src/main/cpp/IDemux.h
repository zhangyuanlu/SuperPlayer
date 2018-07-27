//
// Created by 张远路 on 2018/7/20.
//

#ifndef SUPERPLAYER_IDEMUX_H
#define SUPERPLAYER_IDEMUX_H

#include "XData.h"
#include "XThread.h"
#include "IObserver.h"
#include "XParameter.h"

//解封装接口
class IDemux:public IObserver{
public:
    //打开文件或者流媒体rmtp http rtsp
    virtual bool Open(const char *url)=0;

    //获取视频参数
    virtual XParameter GetVParam()=0;

    //获取音频参数
    virtual XParameter GetAParam()=0;

    //读取一帧数据，数据由调用者清理
    virtual XData Read()=0;

    //总时长（毫秒）
    int totalMs=0;
protected:
    virtual void Main();

};


#endif //SUPERPLAYER_IDEMUX_H

//
// Created by 张远路 on 2018/7/20.
//

#ifndef SUPERPLAYER_FFDEMUX_H
#define SUPERPLAYER_FFDEMUX_H

#include "IDemux.h"
struct AVFormatContext;

class FFDemux :public IDemux{
public:
    //构造函数
    FFDemux();

    //打开文件或者流媒体rmtp http rtsp
    virtual bool Open(const char *url);

    virtual void Close();

    //获取视频参数
    virtual XParameter GetVParam();

    //获取音频参数
    virtual XParameter GetAParam();

    //读取一帧数据，数据由调用者清理
    virtual XData Read();

private:
    AVFormatContext *formatContext=0;
    std::mutex mux;
    int audioStream=1;
    int videoStream=0;
};


#endif //SUPERPLAYER_FFDEMUX_H

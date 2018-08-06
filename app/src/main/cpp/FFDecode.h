//
// Created by 张远路 on 2018/7/23.
//

#ifndef SUPERPLAYER_FFDECODE_H
#define SUPERPLAYER_FFDECODE_H


#include "XParameter.h"
#include "IDecode.h"

struct AVCodecContext;
struct AVFrame;
class FFDecode :public IDecode{
public:
    //初始化硬解码
    static void InitDXVA(void *vm);

    //打开解码器
    virtual bool Open(XParameter xParameter,bool isDXVA=false);
    virtual void Close();
    virtual void Clear();

    //future模型，发送数据到线程解码
    virtual bool SendPacket(XData xData);

    //从线程中获取解码结果，再次调用会复用上次空间，线程不安全
    virtual XData RecvFrame();
protected:
    AVCodecContext *codecContext=0;
    AVFrame *frame=0;
    std::mutex mux;
};


#endif //SUPERPLAYER_FFDECODE_H

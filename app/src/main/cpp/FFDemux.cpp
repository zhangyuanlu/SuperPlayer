//
// Created by 张远路 on 2018/7/20.
//

#include "FFDemux.h"
#include "XLog.h"

extern "C" {
#include "libavformat/avformat.h"
}

FFDemux::FFDemux()
{
    static bool isFirst=true;
    if(isFirst)
    {
        isFirst= false;
        //注册所有的解封装器，4.0之后版本不需要
        av_register_all();
        //注册所有的解码器，4.0之后版本不需要
        avcodec_register_all();
        //初始化网络
        avformat_network_init();
        XLOGI("register ffmpeg");
    }
}

//打开文件，或者流媒体 rmtp http rtsp
bool FFDemux::Open(const char *url)
{
    XLOGI("Open file %s begin",url);
    int re = avformat_open_input(&formatContext,url,0,0);
    if(re != 0 )
    {
        char buf[1024] = {0};
        av_strerror(re,buf,sizeof(buf));
        XLOGE("FFDemux open %s failed!",url);
        return false;
    }
    XLOGI("FFDemux open %s success!",url);

    //读取文件信息
    re = avformat_find_stream_info(formatContext,0);
    if(re != 0 )
    {
        char buf[1024] = {0};
        av_strerror(re,buf,sizeof(buf));
        XLOGE("avformat_find_stream_info %s failed!",url);
        return false;
    }

    this->totalMs = formatContext->duration/(AV_TIME_BASE/1000);
    XLOGI("total ms = %d!",totalMs);
    GetVParam();
    GetAParam();
    return true;
}
//获取视频参数
XParameter FFDemux::GetVParam()
{
    if (!formatContext) {
        XLOGE("GetVPara failed! ic is NULL！");
        return XParameter();
    }
    //获取了视频流索引
    int re = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0);
    if (re < 0) {
        XLOGE("av_find_best_stream failed!");
        return XParameter();
    }
    videoStream = re;
    XParameter para;
    para.parameters = formatContext->streams[re]->codecpar;

    return para;
}
//获取音频参数
XParameter FFDemux::GetAParam()
{
    if (!formatContext) {
        XLOGE("GetVPara failed! ic is NULL！");
        return XParameter();
    }
    //获取了音频流索引
    int re = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0);
    if (re < 0) {
        XLOGE("av_find_best_stream failed!");
        return XParameter();
    }
    audioStream = re;
    XParameter para;
    para.parameters = formatContext->streams[re]->codecpar;
    para.channels = formatContext->streams[re]->codecpar->channels;
    para.sample_rate = formatContext->streams[re]->codecpar->sample_rate;
    return para;
}
//分数转为浮点数
static double r2d(AVRational r)
{
    return r.num==0||r.den==0?0:(double)r.num/(double)r.den;
}
//读取一帧数据，数据由调用者清理
XData FFDemux::Read()
{
    if(!formatContext)return XData();
    XData d;
    AVPacket *pkt = av_packet_alloc();
    int re = av_read_frame(formatContext,pkt);
    if(re != 0)
    {
        av_packet_free(&pkt);
        return XData();
    }
    //XLOGI("pack size is %d ptss %lld",pkt->size,pkt->pts);
    d.data = (unsigned char*)pkt;
    d.size = pkt->size;
    if(pkt->stream_index == audioStream)
    {
        d.isAudio = true;
    }
    else if(pkt->stream_index == videoStream)
    {
        d.isAudio = false;
    }
    else
    {
        av_packet_free(&pkt);
        return XData();
    }
    //转换pts
    pkt->pts=pkt->pts*(1000*r2d(formatContext->streams[pkt->stream_index]->time_base));
    pkt->dts=pkt->dts*(1000*r2d(formatContext->streams[pkt->stream_index]->time_base));
    d.pts=(long long int)pkt->pts;
    return d;
}
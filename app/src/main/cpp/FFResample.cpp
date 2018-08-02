//
// Created by 张远路 on 2018/7/26.
//
extern "C"{
#include <libswresample/swresample.h>
}

#include <libavcodec/avcodec.h>
#include "FFResample.h"
#include "XLog.h"
void FFResample::Close()
{
    mux.lock();
    if(aContext)
    {
        swr_free(&aContext);
    }
    mux.unlock();
}
bool FFResample::Open(XParameter in,XParameter out)
{
    Close();
    mux.lock();
    //音频重采样上下文初始化
    aContext = swr_alloc();
    aContext = swr_alloc_set_opts(aContext,
                              av_get_default_channel_layout(out.channels),
                              AV_SAMPLE_FMT_S16,out.sample_rate,
                              av_get_default_channel_layout(in.parameters->channels),
                              (AVSampleFormat)in.parameters->format,in.parameters->sample_rate,
                              0,0 );

    int re = swr_init(aContext);
    if(re != 0)
    {
        mux.unlock();
        XLOGE("swr_init failed!");
        return false;
    }
    else
    {
        XLOGI("swr_init success!");
    }
    outChannels = in.parameters->channels;
    outFormat = AV_SAMPLE_FMT_S16;
    mux.unlock();
    return true;

}

XData FFResample::Resample(XData indata)
{
    if(indata.size<=0 || !indata.data) return XData();
    mux.lock();
    if(!aContext){
        mux.unlock();
        return XData();
    }

    //XLOGE("indata size is %d",indata.size);
    AVFrame *frame = (AVFrame *)indata.data;

    //输出空间的分配
    XData out;
    int outsize = outChannels * frame->nb_samples * av_get_bytes_per_sample((AVSampleFormat)outFormat);
    if(outsize <=0)return XData();
    out.Alloc(outsize);
    uint8_t *outArr[2] = {0};
    outArr[0] = out.data;
    int len = swr_convert(aContext,outArr,frame->nb_samples,(const uint8_t **)frame->data,frame->nb_samples);
    if(len<=0)
    {
        mux.unlock();
        out.Drop();
        return XData();
    }
    out.pts=indata.pts;
    mux.unlock();
    //XLOGE("swr_convert success = %d",len);
    return out;
}
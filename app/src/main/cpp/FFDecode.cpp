//
// Created by 张远路 on 2018/7/23.
//
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavcodec/jni.h>
}
#include "FFDecode.h"
#include "XLog.h"

void FFDecode::InitDXVA(void *vm)
{
    av_jni_set_java_vm(vm,0);
}
void FFDecode::Close()
{
    IDecode::Clear();
    mux.lock();
    pts=0;
    if(frame)
    {
        av_frame_free(&frame);
    }
    if(codecContext)
    {
        avcodec_close(codecContext);
        avcodec_free_context(&codecContext);
    }
    mux.unlock();
}
void FFDecode::Clear()
{
    IDecode::Clear();
    mux.lock();
    if(codecContext)
    {
        avcodec_flush_buffers(codecContext);
    }
    mux.unlock();
}
bool FFDecode::Open(XParameter para,bool isDXVA)
{
    Close();
    if(!para.parameters) return false;
    AVCodecParameters *p = para.parameters;
    //1 查找解码器
    AVCodec *cd = avcodec_find_decoder(p->codec_id);
    if(isDXVA)
    {
        cd = avcodec_find_decoder_by_name("h264_mediacodec");
    }
    if(!cd)
    {
        XLOGE("avcodec_find_decoder %d failed!,isDXVA=%d",p->codec_id,isDXVA);
        return false;
    }
    XLOGI("avcodec_find_decoder success: %d!",isDXVA);
    //2 创建解码上下文，并复制参数
    mux.lock();
    codecContext = avcodec_alloc_context3(cd);
    avcodec_parameters_to_context(codecContext,p);

    codecContext->thread_count = 8;
    //3 打开解码器
    int re = avcodec_open2(codecContext,0,0);
    if(re != 0)
    {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re,buf,sizeof(buf)-1);
        XLOGE("%s",buf);
        return false;
    }

    if(codecContext->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        this->isAudio = false;
    }
    else
    {
        this->isAudio = true;
    }
    mux.unlock();
    XLOGI("avcodec_open2 success!");
    return true;
}
bool FFDecode::SendPacket(XData pkt)
{
    if(pkt.size<=0 || !pkt.data)return false;
    mux.lock();
    if(!codecContext)
    {
        mux.unlock();
        return false;
    }
    int re = avcodec_send_packet(codecContext,(AVPacket*)pkt.data);
    mux.unlock();
    if(re != 0)
    {
        return false;
    }

    return true;
}

//从线程中获取解码结果
XData FFDecode::RecvFrame()
{
    mux.lock();
    if(!codecContext)
    {
        mux.unlock();
        return XData();
    }
    if(!frame)
    {
        frame = av_frame_alloc();
    }
    int re = avcodec_receive_frame(codecContext,frame);
    if(re != 0)
    {
        mux.unlock();
        return XData();
    }
    XData d;
    d.data = (unsigned char *)frame;
    if(codecContext->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        d.size = (frame->linesize[0] + frame->linesize[1] + frame->linesize[2])*frame->height;
        d.width = frame->width;
        d.height = frame->height;
    }
    else
    {
        //样本字节数 * 单通道样本数 * 通道数
        d.size = av_get_bytes_per_sample((AVSampleFormat)frame->format)*frame->nb_samples*2;
    }
    d.format=frame->format;
    d.pts=frame->pts;
    pts=d.pts;
    //XLOGI("d.format=%d",d.format);
    memcpy(d.datas,frame->data,sizeof(d.datas));
    mux.unlock();
    return d;
}
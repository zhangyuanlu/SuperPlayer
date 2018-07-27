//
// Created by 张远路 on 2018/7/20.
//

#include "XData.h"
extern "C"{
#include <libavformat/avformat.h>
}
bool XData::Alloc(int size, const char *data)
{
    Drop();
    type=UCHAR_TYPE;
    if(size<=0)
    {
        return false;
    }
    this->data=new unsigned char[size];
    if(!this->data)
    {
        return false;
    }
    if(data)
    {
        memcpy(this->data,data,size);
    }
    this->size=size;
    return true;
}
void XData::Drop()
{
    if(!data)
    {
        return;
    }
    if(type==AVPACKET_TYPE)
    {
        av_packet_free((AVPacket**)&data);
    }
    else
    {
        delete data;
    }
    data=0;
    size=0;
}
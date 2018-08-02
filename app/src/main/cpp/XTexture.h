//
// Created by 张远路 on 2018/7/24.
//

#ifndef SUPERPLAYER_XTEXTURE_H
#define SUPERPLAYER_XTEXTURE_H

#include <libavutil/pixfmt.h>

enum XTextureType
{
    XTEXTURE_YUV420P=0, //Y4 U1 V1
    XTEXTURE_NV12=AV_PIX_FMT_NV12,   //Y4 UV1
    XTEXTURE_NV21=AV_PIX_FMT_NV21    //Y4 VU1
};

class XTexture {
public:
    static XTexture *Create();
    virtual bool Init(void *window,XTextureType type=XTEXTURE_YUV420P)=0;
    virtual void Draw(unsigned char *data[],int width,int height)=0;
    virtual void Drop()=0;
    virtual ~XTexture(){};
protected:
    XTexture(){};
};


#endif //SUPERPLAYER_XTEXTURE_H

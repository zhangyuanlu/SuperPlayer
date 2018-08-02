//
// Created by 张远路 on 2018/7/24.
//

#ifndef SUPERPLAYER_XSHADER_H
#define SUPERPLAYER_XSHADER_H

#include <libavutil/pixfmt.h>
#include <mutex>

enum XShaderType
{
    XShader_YUV420P=0, //Y4 U1 V1
    XShader_NV12=AV_PIX_FMT_NV12,   //Y4 UV1
    XShader_NV21=AV_PIX_FMT_NV21    //Y4 VU1
};

class XShader {
public:
    virtual bool Init(XShaderType type=XShader_YUV420P);
    virtual void Close();
    //获取材质并映射到内存
    virtual void GetTexture(unsigned int index,int width,int height, unsigned char *buf,bool isa=false);
    virtual void Draw();

protected:
    unsigned int vShader=0;
    unsigned int fShader=0;
    unsigned int program=0;
    unsigned int textures[100]={0};
    std::mutex mux;
};


#endif //SUPERPLAYER_XSHADER_H

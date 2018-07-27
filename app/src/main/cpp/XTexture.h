//
// Created by 张远路 on 2018/7/24.
//

#ifndef SUPERPLAYER_XTEXTURE_H
#define SUPERPLAYER_XTEXTURE_H


class XTexture {
public:
    static XTexture *Create();
    virtual bool Init(void *window)=0;
    virtual void Draw(unsigned char *data[],int width,int height)=0;
};


#endif //SUPERPLAYER_XTEXTURE_H

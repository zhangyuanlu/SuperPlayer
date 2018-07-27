//
// Created by 张远路 on 2018/7/24.
//

#ifndef SUPERPLAYER_XSHADER_H
#define SUPERPLAYER_XSHADER_H


class XShader {
public:
    virtual bool Init();
    //获取材质并映射到内存
    virtual void GetTexture(unsigned int index,int width,int height, unsigned char *buf);
    virtual void Draw();

protected:
    unsigned int vShader=0;
    unsigned int fShader=0;
    unsigned int program=0;
    unsigned int textures[100]={0};
};


#endif //SUPERPLAYER_XSHADER_H

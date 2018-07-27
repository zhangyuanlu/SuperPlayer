//
// Created by 张远路 on 2018/7/24.
//

#include "XTexture.h"
#include "XLog.h"
#include "XEGL.h"
#include "XShader.h"

class CXTexture:public XTexture{
public:
    XShader xShader;
    virtual bool Init(void *window)
    {
        if(!window)
        {
            XLOGE("XTexture Init failed: window is null");
            return false;
        }
        if(!XEGL::Get()->Init(window)) return false;
        xShader.Init();
        return true;
    }
    virtual void Draw(unsigned char *data[],int width,int height)
    {
        xShader.GetTexture(0,width,height,data[0]);         //Y
        xShader.GetTexture(1,width/2,height/2,data[1]);     //U
        xShader.GetTexture(2,width/2,height/2,data[2]);     //V
        xShader.Draw();
        XEGL::Get()->Draw();
    }
};
XTexture* XTexture::Create()
{
    return new CXTexture();
}
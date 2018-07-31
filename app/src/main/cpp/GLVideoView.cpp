//
// Created by 张远路 on 2018/7/24.
//

#include "GLVideoView.h"
#include "XTexture.h"

void GLVideoView::SetRender(void *win)
{
    windowView = win;
}
void GLVideoView::Render(XData data)
{

    if(!windowView) return;
    if(!xTexture)
    {
        xTexture = XTexture::Create();

        xTexture->Init(windowView,(XTextureType)data.format);
    }
    xTexture->Draw(data.datas,data.width,data.height);
}
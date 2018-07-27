//
// Created by 张远路 on 2018/7/24.
//

#ifndef SUPERPLAYER_GLVIDEOVIEW_H
#define SUPERPLAYER_GLVIDEOVIEW_H


#include "XData.h"
#include "IVideoView.h"

class XTexture;
class GLVideoView :public IVideoView{
public:
    virtual void SetRender(void *window);
    virtual void Render(XData xData);

protected:
    void *windowView=0;
    XTexture *xTexture=0;
};


#endif //SUPERPLAYER_GLVIDEOVIEW_H

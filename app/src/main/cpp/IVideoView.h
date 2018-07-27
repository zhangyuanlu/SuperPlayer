//
// Created by 张远路 on 2018/7/24.
//

#ifndef SUPERPLAYER_IVIDEOVIEW_H
#define SUPERPLAYER_IVIDEOVIEW_H


#include "XData.h"
#include "IObserver.h"

class IVideoView :public IObserver{
public:
    virtual void SetRender(void *window)=0;
    virtual void Render(XData xData)=0;
    virtual void Update(XData xData);
};


#endif //SUPERPLAYER_IVIDEOVIEW_H

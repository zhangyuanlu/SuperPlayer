//
// Created by 张远路 on 2018/7/26.
//

#ifndef SUPERPLAYER_FFRESAMPLE_H
#define SUPERPLAYER_FFRESAMPLE_H


#include "IResample.h"
struct SwrContext;
class FFResample : public IResample{
public:
    virtual bool Open(XParameter in,XParameter out=XParameter());
    virtual void Close();
    virtual XData Resample(XData inData);

protected:
    SwrContext *aContext=0;
    std::mutex mux;
};


#endif //SUPERPLAYER_FFRESAMPLE_H

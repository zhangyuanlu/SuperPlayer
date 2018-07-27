//
// Created by 张远路 on 2018/7/26.
//

#ifndef SUPERPLAYER_IRESAMPLE_H
#define SUPERPLAYER_IRESAMPLE_H


#include "IObserver.h"
#include "XParameter.h"

class IResample : public IObserver{
public:
    virtual bool Open(XParameter in,XParameter out=XParameter())=0;
    virtual XData Resample(XData inData)=0;
    virtual void Update(XData xData);
    int outChannels=2;
    int outFormat=1;

};


#endif //SUPERPLAYER_IRESAMPLE_H

//
// Created by 张远路 on 2018/7/26.
//

#include "IResample.h"
#include "XLog.h"

void IResample::Update(XData data)
{

    XData d = this->Resample(data);
    //XLOGI("his->Resample(data) %d",d.size);
    if(d.size > 0)
    {
        this->Notify(d);
    }
}
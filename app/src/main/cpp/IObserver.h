//
// Created by 张远路 on 2018/7/23.
//

#ifndef SUPERPLAYER_IOBSERVER_H
#define SUPERPLAYER_IOBSERVER_H

#include "XData.h"
#include "XThread.h"
#include <vector>
#include <mutex>

//观察者和主体
class IObserver :public XThread{
public:
    //观察者接收数据函数
    virtual void Update(XData data){}

    //主体函数 添加观察者
    void AddObs(IObserver *obs);

    //通知所有观察者
    void Notify(XData data);

protected:
    std::vector<IObserver *>obss;  //容器
    std::mutex mux; //保持线程安全
};


#endif //SUPERPLAYER_IOBSERVER_H

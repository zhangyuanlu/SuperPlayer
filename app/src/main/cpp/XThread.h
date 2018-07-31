//
// Created by 张远路 on 2018/7/20.
//

#ifndef SUPERPLAYER_XTHREAD_H
#define SUPERPLAYER_XTHREAD_H

void XSleep(int mis);//毫秒
//c++11 线程库
class XThread
{
public:
    //启动线程
    virtual bool Start();

    //通过控制isExit安全停止线程（不一定成功）
    virtual void Stop();

    //入口主函数
    virtual void Main(){}

protected:
    bool isExit=false;
    bool isRunning=false;

private:
    void ThreadMain();
};


#endif //SUPERPLAYER_XTHREAD_H

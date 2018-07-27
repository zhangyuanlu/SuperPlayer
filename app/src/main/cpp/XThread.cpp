//
// Created by 张远路 on 2018/7/20.
//

#include "XThread.h"
#include "XLog.h"
#include <thread>
using namespace std;
void XSleep(int mis)
{
    chrono::milliseconds du(mis);
    this_thread::sleep_for(du);
}
void XThread::Start()
{
    isExit=false;
    thread th(&XThread::ThreadMain,this);
    th.detach();    //放弃对新创建的该线程的控制
}
void XThread::ThreadMain()
{
    isRunning=true;
    XLOGI("线程函数进入！\n");
    Main();
    XLOGI("线程函数退出！\n");
    isRunning=false;

}
//通过控制isExit安全停止线程（不一定成功）
void XThread::Stop()
{
    isExit=true;
    for(int i=0;i<200;i++)
    {
        if(!isRunning)
        {
            XLOGI("停止线程成功");
            return;
        }
        XSleep(1);
    }
    XLOGI("停止线程超时");
}
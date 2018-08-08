//
// Created by 张远路 on 2018/7/31.
//

#include "IPlayerProxy.h"
#include "FFPlayerBuilder.h"
#include "XLog.h"

void IPlayerProxy::Init(void *vm)
{
    mux.lock();
    if(vm)
    {
        FFPlayerBuilder::InitDXVA(vm);
    }
    if(!player)
    {
        player=FFPlayerBuilder::Get()->BuilderPlayer();
    }
    mux.unlock();
}
void IPlayerProxy::Close()
{
    mux.lock();
    if(player)
    {
        player->Close();
    }
    mux.unlock();
}
double IPlayerProxy::PlayPos()
{
    double pos=0.0;
    mux.lock();
    if(player)
    {
        pos=player->PlayPos();
    }
    mux.unlock();
    return pos;
}
bool IPlayerProxy::SeekPos(double pos)
{
    bool ret=false;
    mux.lock();
    if(player)
    {
        ret=player->SeekPos(pos);
    }
    mux.unlock();
    return ret;
}
bool IPlayerProxy::IsPause()
{
    bool ret=false;
    mux.lock();
    if(player)
    {
        ret=player->IsPause();
    }
    mux.unlock();
    return ret;
}
void IPlayerProxy::SetPause(bool isPause)
{
    mux.lock();
    if(player)
    {
        player->SetPause(isPause);
    }
    mux.unlock();
}
bool IPlayerProxy::Open(const char *path)
{
    bool re=false;
    mux.lock();
    if(player)
    {
        player->isDXVA=isDXVA;
        re=player->Open(path);
    }
    mux.unlock();
    return re;
}
bool IPlayerProxy::StartPlay()
{
    bool re= false;
    mux.lock();
    if(player)
    {
        re=player->StartPlay();
    }
    mux.unlock();
    return re;
}
void IPlayerProxy::InitView(void *window)
{
    mux.lock();
    if(player)
    {
        player->InitView(window);
    }
    mux.unlock();
}
int IPlayerProxy::GetTotalMs()
{
    int re=0;
    mux.lock();
    if(player)
    {
        re=player->GetTotalMs();
    }
    mux.unlock();
    return re;
}
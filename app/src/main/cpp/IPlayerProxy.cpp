//
// Created by 张远路 on 2018/7/31.
//

#include "IPlayerProxy.h"
#include "FFPlayerBuilder.h"

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
bool IPlayerProxy::Open(const char *path)
{
    bool re=false;
    mux.lock();
    if(player)
    {
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
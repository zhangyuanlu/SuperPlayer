//
// Created by 张远路 on 2018/7/24.
//

#ifndef SUPERPLAYER_XEGL_H
#define SUPERPLAYER_XEGL_H


class XEGL {
public:
    virtual bool Init(void *window)=0;
    virtual void Draw()=0;
    static XEGL *Get();

protected:
    XEGL(){}
};


#endif //SUPERPLAYER_XEGL_H

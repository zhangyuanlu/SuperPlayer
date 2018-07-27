//
// Created by 张远路 on 2018/7/20.
//

#ifndef SUPERPLAYER_XDATA_H
#define SUPERPLAYER_XDATA_H

enum XDataType{
    AVPACKET_TYPE=0,
    UCHAR_TYPE=1
};
struct XData {
    int type=0;
    unsigned char *data=0;
    unsigned char *datas[8]={0};
    int size=0;
    int width=0;
    int height=0;
    bool isAudio=false;
    bool Alloc(int size,const char *data=0);
    void Drop();
};


#endif //SUPERPLAYER_XDATA_H

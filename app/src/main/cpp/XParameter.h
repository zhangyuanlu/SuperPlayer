//
// Created by 张远路 on 2018/7/23.
//

#ifndef SUPERPLAYER_XPARAMETER_H
#define SUPERPLAYER_XPARAMETER_H


struct AVCodecParameters;
class XParameter {
public:
    AVCodecParameters *parameters=0;
    int channels=2;
    int sample_rate=44100;
};


#endif //SUPERPLAYER_XPARAMETER_H

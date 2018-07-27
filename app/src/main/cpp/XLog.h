//
// Created by 张远路 on 2018/7/20.
//

#ifndef SUPERPLAYER_XLOG_H
#define SUPERPLAYER_XLOG_H

#ifdef ANDROID
#include "android/log.h"
#define XLOGD(...) __android_log_print(ANDROID_LOG_DEBUG,"superplayer",__VA_ARGS__)
#define XLOGI(...) __android_log_print(ANDROID_LOG_INFO,"superplayer",__VA_ARGS__)
#define XLOGW(...) __android_log_print(ANDROID_LOG_ERROR,"superplayer",__VA_ARGS__)
#define XLOGE(...) __android_log_print(ANDROID_LOG_ERROR,"superplayer",__VA_ARGS__)
#else
#define XLOGD(...) printf(,"superplayer",__VA_ARGS__)
#define XLOGI(...) printf(,"superplayer",__VA_ARGS__)
#define XLOGE(...) printf(,"superplayer",__VA_ARGS__)
#endif
class XLog {

};

#endif //SUPERPLAYER_XLOG_H

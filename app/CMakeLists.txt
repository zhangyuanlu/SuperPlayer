# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp
             src/main/cpp/IDemux.cpp
             src/main/cpp/FFDemux.cpp
             src/main/cpp/XData.cpp
             src/main/cpp/XLog.cpp
             src/main/cpp/XThread.cpp
             src/main/cpp/IObserver.cpp
             src/main/cpp/IDecode.cpp
             src/main/cpp/FFDecode.cpp
             src/main/cpp/XParameter.cpp
             src/main/cpp/IVideoView.cpp
             src/main/cpp/GLVideoView.cpp
             src/main/cpp/XShader.cpp
             src/main/cpp/XEGL.cpp
             src/main/cpp/XTexture.cpp
             src/main/cpp/IResample.cpp
             src/main/cpp/FFResample.cpp
             src/main/cpp/IAudioPlay.cpp
             src/main/cpp/SLAudioPlay.cpp
             src/main/cpp/IPlayer.cpp
             src/main/cpp/IPlayerBuilder.cpp
             src/main/cpp/FFPlayerBuilder.cpp
             src/main/cpp/IPlayerProxy.cpp)

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

#添加ffmpeg相关库
include_directories(include)

set(distribution_DIR ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI})

add_library( avutil
             SHARED
             IMPORTED )
set_target_properties( avutil
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/libavutil.so)

add_library( swresample
             SHARED
             IMPORTED )
set_target_properties( swresample
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/libswresample.so)

add_library( avcodec
             SHARED
             IMPORTED )
set_target_properties( avcodec
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/libavcodec.so)

add_library( avfilter
             SHARED
             IMPORTED )
set_target_properties( avfilter
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/libavfilter.so)

add_library( swscale
             SHARED
             IMPORTED )
set_target_properties( swscale
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/libswscale.so)

add_library( avformat
             SHARED
             IMPORTED )
set_target_properties( avformat
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/libavformat.so)


target_link_libraries( # Specifies the target library.
                       native-lib
                       android     #获取窗口句柄需要
                       OpenSLES
                       GLESv2
                       EGL
                       avutil       #工具库(大部分需要)
                       swresample    #音频采样数据格式转换
                       avcodec      #编解码(重要)
                       avfilter     #滤镜特效处理
                       swscale      #视频像素数据格式转换
                       avformat     #封装格式处理
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )
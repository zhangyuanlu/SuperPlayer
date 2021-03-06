//
// Created by 张远路 on 2018/7/24.
//

#include "XShader.h"
#include "XLog.h"
#include <GLES2/gl2.h>

//顶点着色器GLSL
#define GET_STR(x) #x
static const char *vertexShader=GET_STR(
        attribute vec4 aPosition; //顶点坐标
        attribute vec2 aTexCoord; //材质顶点坐标
        varying vec2 vTexCoord; //输出的材质坐标
        void main(){
            vTexCoord=vec2(aTexCoord.x,1.0-aTexCoord.y);
            gl_Position=aPosition;
        }
);
//片元着色器,软解码和部分x86硬解码
static const char *fragYUV420P=GET_STR(
        precision mediump float;    //精度
        varying vec2 vTexCoord;     //顶点着色器传递过来的坐标
        uniform sampler2D yTexture; //输入的材质（不透明灰度，单像素）
        uniform sampler2D uTexture;
        uniform sampler2D vTexture;
        void main(){
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture,vTexCoord).r;
            yuv.g = texture2D(uTexture,vTexCoord).r - 0.5;
            yuv.b = texture2D(vTexture,vTexCoord).r - 0.5;
            rgb = mat3(1.0,     1.0,    1.0,
                       0.0,-0.39465,2.03211,
                       1.13983,-0.58060,0.0)*yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb,1.0);
        }
);
//片元着色器,软解码和部分x86硬解码
static const char *fragNV12 = GET_STR(
        precision mediump float;    //精度
        varying vec2 vTexCoord;     //顶点着色器传递的坐标
        uniform sampler2D yTexture; //输入的材质（不透明灰度，单像素）
        uniform sampler2D uvTexture;
        void main(){
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture,vTexCoord).r;
            yuv.g = texture2D(uvTexture,vTexCoord).r - 0.5;
            yuv.b = texture2D(uvTexture,vTexCoord).a - 0.5;
            rgb = mat3(1.0,     1.0,    1.0,
                       0.0,-0.39465,2.03211,
                       1.13983,-0.58060,0.0)*yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb,1.0);
        }
);

//片元着色器,软解码和部分x86硬解码
static const char *fragNV21 = GET_STR(
        precision mediump float;    //精度
        varying vec2 vTexCoord;     //顶点着色器传递的坐标
        uniform sampler2D yTexture; //输入的材质（不透明灰度，单像素）
        uniform sampler2D uvTexture;
        void main(){
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture,vTexCoord).r;
            yuv.g = texture2D(uvTexture,vTexCoord).a - 0.5;
            yuv.b = texture2D(uvTexture,vTexCoord).r - 0.5;
            rgb = mat3(1.0,     1.0,    1.0,
                       0.0,-0.39465,2.03211,
                       1.13983,-0.58060,0.0)*yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb,1.0);
        }
);
static GLuint InitShader(const char *code,GLint type){
    //创建Shader
    GLuint shader=glCreateShader(type);
    if(shader==0){
        XLOGW("glCreateShader failed!\n");
        return 0;
    }
    //加载Shader
    glShaderSource(shader,
                   1,//shader数量
                   &code,//shader代码
                   0);//代码长度
    //编译shader
    glCompileShader(shader);
    //获取编译情况
    GLint status;
    glGetShaderiv(shader,GL_COMPILE_STATUS,&status);
    if(status==0){
        XLOGW("glCompileShader failed!\n");
        return 0;
    }
    XLOGI("glCompileShader success!\n");
    return shader;
}

void XShader::Close()
{
    mux.lock();
    //释放shader
    if(program)
    {
        glDeleteProgram(program);
    }
    if(fShader)
    {
        glDeleteShader(fShader);
    }
    if(vShader)
    {
        glDeleteShader(vShader);
    }
    //释放材质
    for(int i=0;i< sizeof(textures)/ sizeof(unsigned int);i++)
    {
        if(textures[i])
        {
            glDeleteTextures(1,&textures[i]);
        }
        textures[i]=0;
    }
    mux.unlock();
}

bool XShader::Init(XShaderType type)
{
    Close();
    mux.lock();
    //顶点和片元Shader初始化
    vShader=InitShader(vertexShader,GL_VERTEX_SHADER);
    if(vShader==0)
    {
        mux.unlock();
        XLOGE("InitVShader failed");
        return false;
    }
    XLOGI("InitVShader success");
    switch (type)
    {
        case XShader_YUV420P:
            fShader=InitShader(fragYUV420P,GL_FRAGMENT_SHADER);
            break;
        case XShader_NV12:
            fShader=InitShader(fragNV12,GL_FRAGMENT_SHADER);
            break;
        case XShader_NV21:
            fShader=InitShader(fragNV21,GL_FRAGMENT_SHADER);
            break;
        default:
            XLOGE("XShader format is wrong!");
            mux.unlock();
            return false;
    }

    if(fShader==0)
    {
        mux.unlock();
        XLOGE("InitFShader failed");
        return false;
    }
    XLOGI("InitFShader success");

    //创建渲染程序
    program=glCreateProgram();
    if(program==0){
        mux.unlock();
        XLOGE("glCreateProgram failed!\n");
        return false;
    }
    //渲染程序中加入着色器代码
    glAttachShader(program,vShader);
    glAttachShader(program,fShader);
    //链接程序
    glLinkProgram(program);
    GLint status=0;
    glGetProgramiv(program,GL_LINK_STATUS,&status);
    if(status!=GL_TRUE){
        mux.unlock();
        XLOGE("glLinkProgram failed!\n");
        return false;
    }
    glUseProgram(program);
    XLOGI("glLinkProgram success!\n");

    //加入三维顶点数据 两个三角形组成正方形
    static float vers[]={
            1.0f ,-1.0f,0.0f,
            -1.0f,-1.0f,0.0f,
            1.0f ,1.0f ,0.0f,
            -1.0f,1.0f ,0.0f
    };
    GLuint apos = (GLuint)glGetAttribLocation(program,"aPosition");
    glEnableVertexAttribArray(apos);
    //传递顶点
    glVertexAttribPointer(apos,3,GL_FLOAT,GL_FALSE,12,vers);
    //加入材质坐标数据
    static float txts[]={
            1.0f,0.0f,//右下
            0.0f,0.0f,
            1.0f,1.0f,
            0.0f,1.0f
    };
    GLuint atex=(GLuint)glGetAttribLocation(program,"aTexCoord");
    glEnableVertexAttribArray(atex);
    glVertexAttribPointer(atex,2,GL_FLOAT,GL_FALSE,8,txts);

    //材质纹理初始化
    //设置纹理层
    glUniform1i(glGetUniformLocation(program,"yTexture"),0);    //纹理第一层
    switch (type)
    {
        case XShader_YUV420P:
            glUniform1i(glGetUniformLocation(program,"uTexture"),1);    //纹理第二层
            glUniform1i(glGetUniformLocation(program,"vTexture"),2);    //纹理第三层
            break;
        case XShader_NV12:
        case XShader_NV21:
            glUniform1i(glGetUniformLocation(program,"uvTexture"),1);    //纹理第二层
            break;
    }

    mux.unlock();
    XLOGI("init shader success");
    return true;
}

void XShader::Draw()
{
    mux.lock();
    if(!program)
    {
        mux.unlock();
        return;
    }
    //三维绘制
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    mux.unlock();
}
void XShader::GetTexture(unsigned int index, int width, int height, unsigned char *buf,bool isa)
{
    unsigned int format=GL_LUMINANCE;
    if(isa)
    {
        format=GL_LUMINANCE_ALPHA;
    }
    mux.lock();
    if(textures[index]==0)
    {
        //材质初始化
        glGenTextures(1,&textures[index]);
        //设置纹理属性
        glBindTexture(GL_TEXTURE_2D,textures[index]);
        //缩小的过滤器
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        //设置纹理的格式和大小
        glTexImage2D(GL_TEXTURE_2D,
                     0,              //基本细节，默认0
                     format,   //GPU内部格式，亮度、灰度图
                     width,height,   //拉伸到全屏
                     0,              //边框
                     format,   //数据的像素格式，亮度、灰度图，要与上面的一致
                     GL_UNSIGNED_BYTE,//像素的数据类型
                     NULL             //纹理的数据
        );
    }

    //激活第index层纹理
    glActiveTexture(GL_TEXTURE0+index);
    glBindTexture(GL_TEXTURE_2D,textures[index]);
    //替换纹理内容
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,format,GL_UNSIGNED_BYTE,buf);
    mux.unlock();
}
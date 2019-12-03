//
//  decoder.hpp
//  FFmpegAudioDecoder
//
//  Created by Ternence on 2019/11/22.
//  Copyright © 2019 machao. All rights reserved.
//

#ifndef decoder_hpp
#define decoder_hpp

#define byte        uint8_t
#define MAX(a, b)   (((a) > (b)) ? (a) : (b))
#define MIN(a, b)   (((a) < (b)) ? (a) : (b))

#define LOGI(...)  printf("  ");printf(__VA_ARGS__); printf("\t -  <%s> \n", LOG_TAG);

#include <stdio.h>
typedef struct AudioPacket {
    static const int AUDIO_PACKET_ACTION_PLAY  = 0;
    static const int AUDIO_PACKET_ACTION_PAUSE = 100;
    static const int AUDIO_PACKET_ACTION_SEEK  = 101;
    
    short *buffer;
    int size;
    float position;
    int action;
    
    
    float extra_param1;
    float extra_param2;
    
    AudioPacket() {
        buffer = NULL;
        size = 0;
        position = -1;
        action = 0;
        extra_param1 = 0;
        extra_param2 = 0;
    }
    ~AudioPacket() {
        if (NULL != buffer) {
            delete [] buffer;
            buffer = NULL;
        }
    }
} AudioPacket;

#define OUT_PUT_CHANNELS    2

extern "C" {
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/avutil.h"
    #include "libavutil/samplefmt.h"
    #include "libavutil/common.h"
    #include "libavutil/channel_layout.h"
    #include "libavutil/opt.h"
    #include "libavutil/imgutils.h"
    #include "libavutil/mathematics.h"
    #include "libswscale/swscale.h"
    #include "libswresample/swresample.h"

}

class Decoder {
private:
    //如果使用了快进或者快退命令，则先设置以下参数
    bool seek_req;
    bool seek_resp;
    float seek_seconds;
    
    float actualSeekPosition;
    
    AVFormatContext *avFormatContext;
    AVCodecContext  *avCodecContext;
    int stream_index;
    float timeBase;
    AVFrame *pAudioFrame;
    AVPacket packet;
    
    char *filePath;
    bool seek_success_read_frame_success;
    int packetBufferSize;
    
    /** 每次解码出来的audioBiffer以及这个audioBuffer的时间戳以及当前类对于这个audioBuffer的操作情况*/
    short *audioBuffer;
    float position;
    int audioBufferCursor;
    int audioBufferSize;
    float duration;
    bool isNeedFirstFrameCorrectFlag;
    float firstFrameCorrectionInsecs;
    
    SwrContext *swrContext;
    void *swrBuffer;
    int swrBufferSize;
    
    int init(const char* fileString);
    int readSamples(short *samples, int size);
    int readFrame();
    bool audioCodecIsSupported();
 
public:
    Decoder();
    virtual ~Decoder(); //虚函数是指一个类中你希望重载的成员函数 ，当你用一个  基类指针或引用   指向一个继承类对象的时候，调用一个虚函数时, 实际调用的是继承类的版本
    
    //获取采样率及比特率
    virtual int getMusicMeta(const char *fileString, int *metaData);
    //初始化这个decoder，即打开指定的mp3文件
    virtual void init(const char *fileString, int packetBufferSizeParam);
    virtual AudioPacket *decodePacket();
    
    //销毁这个decoder
    virtual void destroy();
    void setSeekReq(bool seekReqParam){
        seek_req = seekReqParam;
        if (seek_req) {
            seek_resp = false;
        }
    };
    bool hasSeekReq() {
        return seek_req;
    }
    bool hasSeekResp() {
        return seek_resp;
    }
    
    /** 设置播放到什么位置，单位是秒，但是后边3位小数，其实是精确到毫秒*/
    void setPosition(float seconds) {
        actualSeekPosition = -1;
        this->seek_seconds = seconds;
        this->seek_req = true;
        this->seek_resp = false;
    };
    
    float getActualSeekPosition() {
        float ret = actualSeekPosition;
        if (ret != -1) {
            actualSeekPosition = -1;
        }
        return  ret;
    }
    
    virtual void seek_frame();
    
};

#endif /* decoder_hpp */

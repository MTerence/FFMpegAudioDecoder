//
//  decoder_controller.hpp
//  FFmpegAudioDecoder
//
//  Created by Ternence on 2019/11/22.
//  Copyright © 2019 machao. All rights reserved.
//

#ifndef decoder_controller_hpp
#define decoder_controller_hpp

#include <stdio.h>
#include "decoder.hpp"

#define CHANNEL_PER_FRAME   2
#define BITS_PER_CHANNEL    16
#define BITS_PER_BYTE       8

/** decode data to queue and queue size*/
#define QUEUE_SIZE_MAX_THRESHOLD    25
#define QUEUE_SIZE_MIN_THRESHOLD    20

class DecoderController {
protected:
    FILE *pcmFile;
    
    /** 伴奏的解码器*/
    Decoder *decoder;
    
    /** 伴奏和原唱的采样率与解码伴奏和原唱的每个packet的大小*/
    int sampleRate;
    int packetBufferSize;
    
public:
    DecoderController();
    ~DecoderController();
    
    /** 初始化两个decoer，并且根据上一步算出的采样率，计算出伴奏和原唱的bufferSize*/
    void Init(const char *path, const char* pcmFilePath);
    /** 解码操作*/
    void Decoder();
    
    /** 销毁这个controller*/
    void Destroy();
};

#endif /* decoder_controller_hpp */

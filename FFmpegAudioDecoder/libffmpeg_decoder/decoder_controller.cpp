//
//  decoder_controller.cpp
//  FFmpegAudioDecoder
//
//  Created by Ternence on 2019/11/22.
//  Copyright © 2019 machao. All rights reserved.
//

#include "decoder_controller.hpp"

DecoderController:: DecoderController() {
    decoder = NULL;
    pcmFile = NULL;
}

DecoderController::~DecoderController() {
}

void DecoderController::Init(const char *path, const char *pcmFilePath) {
    //初始化两个decoder
    class Decoder *tempDecoder = new class Decoder();
    int metaData[2];
    tempDecoder->getMusicMeta(path, metaData);
    delete tempDecoder;
    
    //初始化伴奏和采样率
    sampleRate = metaData[0];
    int byteCountPerSec = sampleRate * CHANNEL_PER_FRAME * BITS_PER_CHANNEL / BITS_PER_BYTE;
    packetBufferSize = (int) ((byteCountPerSec / 2) * 0.2);
    decoder = new class Decoder();
    decoder->init(path, packetBufferSize);
    pcmFile = fopen(pcmFilePath, "wb+");
}

void DecoderController::Decoder() {
    while (true) {
        AudioPacket *packet = decoder->decodePacket();
        if (-1 == packet->size) {
            break;
        }
        fwrite(packet->buffer, sizeof(short), packet->size, pcmFile);
    }
}

void DecoderController::Destroy() {
    if (NULL != decoder) {
        decoder->destroy();
        delete decoder;
        decoder = NULL;
    }
    
    if (NULL !=  pcmFile) {
        fclose(pcmFile);
        pcmFile = NULL;
    }
}

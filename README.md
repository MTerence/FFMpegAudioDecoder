# FFmpegAudioDecoder

####初始化两个decoer，并且根据上一步算出的采样率，计算出伴奏和原唱的bufferSize
```
int Decoder::init(const char *audioFile) {
    LOGI("enter Decoder::init");
    audioBuffer = NULL;
    position = -1.0f;
    audioBufferCursor = 0;
    audioBufferSize = 0;
    swrContext = NULL;
    swrBuffer = NULL;
    swrBufferSize = 0;
    seek_success_read_frame_success = true;
    isNeedFirstFrameCorrectFlag = true;
    firstFrameCorrectionInsecs = 0.0f;

    avcodec_register_all();
    av_register_all();
    avFormatContext = avformat_alloc_context();
    //打开输入文件
    LOGI("open file %s...", audioFile);

    if (NULL == filePath) {
        int length = strlen(audioFile);
        filePath = new char[length+1]();
        //由于最后一个是‘\0’, 所有memset的长度要设置为length+1
        memset(filePath, 0, length+1);
        memcpy(filePath, audioFile, length+1);
    }

    int result = avformat_open_input(&avFormatContext, audioFile, NULL, NULL);
    if (result != 0) {
        LOGI("can't open file %s result is %d", audioFile, result);
    } else {
        LOGI("open file %s success and result is %d", audioFile, result);
    }
    avFormatContext->max_analyze_duration = 50000;

    //检查在文件中的流的信息
    result = avformat_find_stream_info(avFormatContext, NULL);
    if (result < 0) {
        LOGI("fail avformat_find_stream_info result is %d", result);
    } else {
        LOGI("success avformat_find_stream_info result is %d", result);
    }
    stream_index = av_find_best_stream(avFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    LOGI("stream_index is %d", stream_index);
    //没有音频
    if (stream_index == -1) {
        LOGI("no audio stream");
        return -1;
    }

    //音频流
    AVStream *audioStream = avFormatContext->streams[stream_index];
    if (audioStream->time_base.den && audioStream->time_base.num)
        timeBase = av_q2d(audioStream->time_base);
    else if (audioStream->codec->time_base.den && audioStream->codec->time_base.num)
        timeBase = av_q2d(audioStream->codec->time_base);
    //获得音频流的解码器上下文
    avCodecContext = audioStream->codec;
    //根据解码器上下文找到解码器
    LOGI("avCodecContext->codec_id is %d AV_CODEC_ID_AAC is %d", avCodecContext->codec_id, AV_CODEC_ID_AAC);
    AVCodec *avCodec = avcodec_find_decoder(avCodecContext->codec_id);
    if (avCodec == NULL) {
        return -1;
    }

    //打开解码器
    result = avcodec_open2(avCodecContext, avCodec, NULL);
    if (result < 0) {
        LOGI("fail avcodec_open result is %d", result);
        return -1;
    } else {
        LOGI("success avcodec_open result is %d", result);
    }

    //4.判断是否需要resampler
    if (!audioCodecIsSupported()) {
        LOGI("because of audio Codec is not supported so we will init swresampler...");

        swrContext = swr_alloc_set_opts(NULL, av_get_default_channel_layout(OUT_PUT_CHANNELS), AV_SAMPLE_FMT_S16,  avCodecContext->sample_rate, av_get_default_channel_layout(avCodecContext->channels), avCodecContext->sample_fmt, avCodecContext->sample_rate, 0, NULL);
        if (!swrContext || swr_init(swrContext)) {
            if (swrContext)
                swr_free(&swrContext);
                avcodec_close(avCodecContext);
                LOGI("init resampler failed...");
                return -1;
            }
    }

    LOGI("channels is %d sampleRate is %d", avCodecContext->channels, avCodecContext->sample_rate);
    pAudioFrame = avcodec_alloc_frame();

    return 1;
}

```

####解码操作
```

    AudioPacket *Decoder::decodePacket() {
    short *samples = new short[packetBufferSize];
    int stereoSampleSize = readSamples(samples, packetBufferSize);
    AudioPacket *samplePacket = new AudioPacket();
    if (stereoSampleSize > 0) {
        //构造成一个packet
        samplePacket->buffer = samples;
        samplePacket->size = stereoSampleSize;
        /** 这里由于每一个packet的大小不一样有可能是200ms，y但是这样子position就有可能不准确了*/
        samplePacket->position = position;
    } else {
        samplePacket->size = -1;
    }
        return samplePacket;
    }
```


####销毁解码器
```
    void Decoder::destroy() {
        if (NULL != swrBuffer) {
            free(swrBuffer);
            swrBuffer = NULL;
            swrBufferSize = 0;
        }

        if (NULL != swrContext) {
            swr_free(&swrContext);
            swrContext = NULL;
        }

        if (NULL != pAudioFrame) {
            av_free(pAudioFrame);
            pAudioFrame = NULL;
        }

        if (NULL != avCodecContext) {
            avcodec_close(avCodecContext);
            avCodecContext = NULL;
        }

        if (NULL != avFormatContext) {
            LOGI("leave LiveReceiver::destory");
            avformat_free_context(avFormatContext);
            avFormatContext = NULL;
        }
    }
```

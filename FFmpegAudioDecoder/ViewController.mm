//
//  ViewController.m
//  FFmpegAudioDecoder
//
//  Created by Ternence on 2019/11/22.
//  Copyright © 2019 machao. All rights reserved.
//

#import "ViewController.h"
#import "decoder_controller.hpp"
#import "CommonUtil.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    UIButton *decoderBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    decoderBtn.frame = CGRectMake(([UIScreen mainScreen].bounds.size.width - 100)/2, ([UIScreen mainScreen].bounds.size.height - 50)/2, 100, 50);
    [decoderBtn setTitle:@"Decoder" forState:UIControlStateNormal];
    [decoderBtn setBackgroundColor:[UIColor lightGrayColor]];
    [decoderBtn addTarget:self action:@selector(onDecoderBtnClick) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:decoderBtn];
    
}

- (void)onDecoderBtnClick{
    const char *mp3FilePath = [[CommonUtil bundlePath:@"131.aac"] cStringUsingEncoding:NSUTF8StringEncoding];
    const char *pcmFilePath = [[CommonUtil documentsPath:@"131.pcm"] cStringUsingEncoding:NSUTF8StringEncoding];
    
    DecoderController *decoderController = new DecoderController();
    decoderController->Init(mp3FilePath, pcmFilePath);
    decoderController->Decoder();
    decoderController->Destroy();
    delete decoderController;
    NSLog(@"After decode Test...");
}


@end

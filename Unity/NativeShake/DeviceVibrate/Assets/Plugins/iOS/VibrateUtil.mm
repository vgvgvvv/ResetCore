#import <AudioToolbox/AudioServices.h>

extern "C"{

    void Shake(){
        AudioServicesPlaySystemSound(1352);
        // // 震动模式, 依次设置震动与停止时间，单位ms
        // NSArray *pattern =  @[@(YES), @(20), @(NO), @(10), @(YES), @(30)];
        // // 震动强度
        // NSNumber *intensity = [NSNumber numberWithInt:2];
        // NSDictionary *vibrate = @{
        //     @"VibePattern" : pattern,
        //     @"Intensity" : intensity 
        // };
	
        // AudioServicesPlaySystemSoundWithVibration(4095, nil, vibrate);
    }

}
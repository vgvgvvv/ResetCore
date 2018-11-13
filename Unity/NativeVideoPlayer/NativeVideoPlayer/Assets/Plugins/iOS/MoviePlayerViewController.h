//
//  ViewController.h
//  movieplayer
//
//  Created by wooduan on 16/11/11.
//  Copyright © 2016年 wooduan. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVKit/AVKit.h>
#import <AVFoundation/AVFoundation.h>

@interface MoviePlayerViewController : AVPlayerViewController
{
    BOOL manualClose;
}

-(BOOL) isManualClose;
@end


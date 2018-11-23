#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <err.h>

#import "UnityAppController.h"
#import "MoviePlayerViewController.h"


@interface TestAppController : UnityAppController
@end


IMPL_APP_CONTROLLER_SUBCLASS (TestAppController)

@implementation TestAppController

static TestAppController* instance;

//应用启动
- (BOOL)application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
    [super application:application didFinishLaunchingWithOptions:launchOptions];
    instance = self;
    return YES;
}

//应用将要进入激活状态
- (void)applicationDidBecomeActive:(UIApplication*)application
{
    [super applicationDidBecomeActive:application];
    
}

//应用将要进入非激活状态
- (void)applicationWillResignActive:(UIApplication*)application
{
    [super applicationWillResignActive:application];

}

//应用进入后台
- (void)applicationDidEnterBackground:(UIApplication*)application
{
    [super applicationDidEnterBackground:application];
    
}

//应用即将回到前台
- (void)applicationWillEnterForeground:(UIApplication*)application
{
    [super applicationWillEnterForeground:application];
  
}

//应用受到内存警告
- (void)applicationDidReceiveMemoryWarning:(UIApplication*)application
{
    [super applicationDidReceiveMemoryWarning:application];
    
}

//应用将关闭
- (void)applicationWillTerminate:(UIApplication*)application
{
    [super applicationWillTerminate:application];
}


- (void) playMovie
{
    [self PlayMovieWithName:nil andArgs:nil];
}

//播放动画
- (void) PlayMovieWithName : (NSString *)name andArgs : (NSString*) args
{
    //播放动画
    MoviePlayerViewController * mp = [[MoviePlayerViewController alloc] init];
    
    //IOS8.0以下的版本不支持AVPlayer控件，必须使用MPMoviePlayerController播放，以后再改
    if(mp == nil)
    {
        NSLog(@"avplayer can not be found , skip the movie");
        UnitySendMessage("Canvas", "Hide", "");
        
        return;
    }
    else
    {
        [self.rootViewController presentViewController:mp animated:NO completion:nil];
    }
    
    // if(mp.isManualClose){
    //     UnitySendMessage("Canvas", "Hide", "");
    // }else{
    //     UnitySendMessage("Canvas", "Hide", "");
    // }
}


#if defined(__cplusplus)
extern "C" {
#endif
    
    //开始播放开场动画
    void iosPlayMovie()
    {
        NSLog(@"ios play movie");
        
        TestAppController * app = instance;
        [app performSelectorOnMainThread:@selector(playMovie) withObject:app waitUntilDone:NO];
    }

    //实现接收到Unity消息接口
    void iosReceiveUnityMessage(const char * name, const char * args)
    {
        //封装参数
        NSString * nName = [NSString stringWithUTF8String:name];
        NSString * nArgs = [NSString stringWithUTF8String:args];
        
        NSLog(@"ios receive unity message, name : %@, args : %@", nName, nArgs);
        
    }
    
    //实现接收到Unity数据接口
    void iosReceiveUnityData(const char * name, const char * args, const char** data, const int length)
    {
        //封装参数
        NSString * nName = [NSString stringWithUTF8String:name];
        NSString * nArgs = [NSString stringWithUTF8String:args];
        NSData * nData = [NSData dataWithBytes:(const void *)data length:(sizeof(unsigned char) * length)];
        
        NSLog(@"ios receive unity data, name : %@, args : %@", nName, nArgs);
    }
    
#if defined(__cplusplus)
}
#endif

@end

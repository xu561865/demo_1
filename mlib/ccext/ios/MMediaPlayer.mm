//
//  MMediaPlayer.cpp
//  legend
//
//  Created by adminstrator on 13-11-18.
//
//

#include "MMediaPlayer.h"
#include "LConstants.h"

#include "EAGLView.h"
#import <MediaPlayer/MediaPlayer.h>
#include "MFileUtils.h"


MPMoviePlayerController *g_player = nil;

@interface MediaPlayerObserver : NSObject
@property(nonatomic, assign) mlib::MMediaPlayer *mediaPlayer;
@end

@implementation MediaPlayerObserver


- (void)onMovieEnded
{
    g_player.view.hidden = YES;
    
    self.mediaPlayer->dispatchEvent(MEDIA_PLAYER_ENDED);
    
}

- (void)onMovieNaturalSizeAvailable
{
    CGSize winSize = [[EAGLView sharedEGLView] frame].size;
    CGSize vedioSize = [g_player naturalSize];
    float scaleFactor = mlib::get_content_scale_factor() / [[EAGLView sharedEGLView] contentScaleFactor];
    vedioSize = CGSizeMake(vedioSize.width * scaleFactor, vedioSize.height * scaleFactor);
    g_player.view.frame = CGRectMake((winSize.width - vedioSize.width) / 2, (winSize.height - vedioSize.height) / 2, vedioSize.width, vedioSize.height);
    
    g_player.view.hidden = NO;
}

@end



MediaPlayerObserver *g_observer = nil;

MLIB_NS_BEGIN

MMediaPlayer::MMediaPlayer()
{
    strNameMark="";
}
MMediaPlayer::~MMediaPlayer()
{
    
}


MMediaPlayer * MMediaPlayer::SharedInstance()
{
    static MMediaPlayer inst;
    return &inst;
}



void MMediaPlayer::playVideo( std::string  strName )
{
    strNameMark = strName;
    
    if (g_player == nil)
    {
        g_player = [[MPMoviePlayerController alloc] init];
//        g_player.view.backgroundColor = [UIColor clearColor];
//        g_player.view.opaque = NO;
//        g_player.backgroundView.backgroundColor = [UIColor clearColor];
//        g_player.backgroundView.opaque = NO;
        [[EAGLView sharedEGLView] addSubview:g_player.view];
        g_player.view.hidden = YES;
    }
    
    std::string strFull = FILE_UTILS->getFullPath(strName);
    g_player.contentURL = [NSURL fileURLWithPath:[NSString stringWithUTF8String:strFull.c_str()]];

    if (g_observer == nil)
    {
        g_observer = [[MediaPlayerObserver alloc] init];
        g_observer.mediaPlayer = this;
    }
    
    [[NSNotificationCenter defaultCenter] addObserver:g_observer
                                             selector:@selector(onMovieEnded)
                                                 name:MPMoviePlayerPlaybackDidFinishNotification
                                               object:g_player];
    
    [[NSNotificationCenter defaultCenter] addObserver:g_observer
                                             selector:@selector(onMovieNaturalSizeAvailable)
                                                 name:MPMoviePlayerLoadStateDidChangeNotification
                                               object:g_player];
    
    [g_player setMovieSourceType:MPMovieSourceTypeFile];
    
    
    g_player.controlStyle = MPMovieControlStyleNone;
    g_player.repeatMode = MPMovieRepeatModeNone;
    g_player.scalingMode = MPMovieScalingModeNone;
    
    [g_player play];
}


void MMediaPlayer::stopVideo()
{
    if (g_player != nil)
        [g_player stop];
}

void MMediaPlayer::continueIfPlaying()
{
    if (g_player != nil)
    {
        if (!g_player.view.hidden)
        {
            [g_player play];
        }
    }
}

MLIB_NS_END



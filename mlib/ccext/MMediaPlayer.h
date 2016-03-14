//
//  MMediaPlayer.h
//  legend
//
//  Created by adminstrator on 13-11-18.
//
//

#ifndef __legend__MMediaPlayer__
#define __legend__MMediaPlayer__

#include <iostream>
#include "mlib_ccext.h"

MLIB_NS_BEGIN

#define MEDIA_PLAYER_ENDED "media_player_ended"


class MMediaPlayer : public mlib::MEventDispatcher
{
public:
    
    static MMediaPlayer * SharedInstance();

    
    MMediaPlayer();
    ~MMediaPlayer();
    void playVideo(std::string  strName);
    void stopVideo();
    void continueIfPlaying();

public:
    std::string  strNameMark;

private:
    std::function<void()> _callbackVideoEnded;
};

#define MPLAYER (mlib::MMediaPlayer::SharedInstance())


MLIB_NS_END

#endif /* defined(__legend__MMediaPlayer__) */

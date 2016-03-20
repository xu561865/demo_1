//
//  MSpeedCheck.h
//  legend
//
//  Created by Chen Wei on 15/6/3.
//
//

#ifndef __legend__MSpeedCheck__
#define __legend__MSpeedCheck__


#include <string>
#include "mlib_helpers.h"
#include "cocos2d.h"
#include "mlib.h"
USING_NS_CC;



class MSpeedCheck
: public CCNode
, public mlib::MEventDispatcher
{
public:
    static const std::string EVENT_SPEED_CHECK_BINGO;
    
    
    enum class State
    {
        IDLE,
        START,
        CHECKING,
    };
    
    enum class Result
    {
        FIRST,
        OK,
        BINGO
    };
    
    
    CREATE_FUNC(MSpeedCheck);
    static MSpeedCheck * create(long checkDuration, long ignoreDelta, int bingoContinuousCount = 3);
    
    MSpeedCheck();
    MSpeedCheck(long checkDuration, long ignoreDelta, int bingoContinuousCount = 3);
    virtual ~MSpeedCheck();
    virtual bool init();
    
    void setCheckData(long checkDuration, long ignoreDelta, int bingoContinuousCount = 3);
    void start();
    inline bool isContinuousCountEnough() { return _continuous_count >= _bingo_continuous_count; }
    
    void step(float delta);
    
    void scheduleStep();
    void unscheduleStep();
    
    
protected:
    void nextLoopCheck();
    
    
    State _state;
    
    long long _tick;//milliseconds
    
    long _check_duration;//milliseconds
    long _ignore_delta;//milliseconds
    
    long long _time_boot_begin;//milliseconds
    struct timeval _time_begin;
    
    int _continuous_count;
    int _bingo_continuous_count;
    Result _last_check_result;
};


#endif /* defined(__legend__MSpeedCheck__) */

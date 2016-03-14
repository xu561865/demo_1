//
//  MSpeedCheck.cpp
//  legend
//
//  Created by Chen Wei on 15/6/3.
//
//

#include "MSpeedCheck.h"
#include "MUtils.h"


const std::string MSpeedCheck::EVENT_SPEED_CHECK_BINGO = "event_speed_check_bingo";


MSpeedCheck * MSpeedCheck::create(long checkDuration, long ignoreDelta, int bingoContinuousCount)
{
    auto check = new MSpeedCheck(checkDuration, ignoreDelta, bingoContinuousCount);
    check->autorelease();
    
    if (check->init())
    {
        return check;
    }
    
    return nullptr;
}


MSpeedCheck::MSpeedCheck()
:_state(State::IDLE)
,_tick(0)
,_time_boot_begin(0)
,_check_duration(1e4)
,_ignore_delta(100)
,_continuous_count(0)
,_bingo_continuous_count(3)
,_last_check_result(Result::FIRST)
{
    
}


MSpeedCheck::MSpeedCheck(long checkDuration, long ignoreDelta, int bingoContinuousCount)
: MSpeedCheck()
{
    setCheckData(checkDuration, ignoreDelta, bingoContinuousCount);
}


MSpeedCheck::~MSpeedCheck()
{
    
}


bool MSpeedCheck::init()
{
    if(!CCNode::init())
    {
        return false;
    }
    
    scheduleStep();
    
    return true;
}


void MSpeedCheck::setCheckData(long checkDuration, long ignoreDelta, int bingoContinuousCount)
{
    _check_duration = labs(checkDuration);
    _ignore_delta = labs(ignoreDelta);
    _bingo_continuous_count = abs(bingoContinuousCount);
}


void MSpeedCheck::scheduleStep()
{
    schedule(schedule_selector(MSpeedCheck::step));
}


void MSpeedCheck::unscheduleStep()
{
    unschedule(schedule_selector(MSpeedCheck::step));
}


void MSpeedCheck::start()
{
    _state = State::START;
    _continuous_count = 0;
    _last_check_result = Result::FIRST;
}


void MSpeedCheck::nextLoopCheck()
{
    _state = State::START;
}


void MSpeedCheck::step(float delta)
{
    switch (_state)
    {
        case State::START:
        {
            _state = State::CHECKING;
            
            _tick = 0;
            
            _time_boot_begin = mlib::get_milliseconds_since_boot();
            gettimeofday(&_time_begin, nullptr);
        }break;
            
        case State::CHECKING:
        {
            _tick += (delta * 1e3);
            
            if (_tick >= _check_duration)
            {
                auto time_boot_end = mlib::get_milliseconds_since_boot();
                struct timeval _time_end;
                gettimeofday(&_time_end, nullptr);
                
                const auto time_boot_interval = time_boot_end - _time_boot_begin;// 真实的时间差
                
                const long long time_end = _time_end.tv_sec * 1e3 + _time_end.tv_usec / 1e3;
                const long long time_start = _time_begin.tv_sec * 1e3 + _time_begin.tv_usec / 1e3;
                const auto time_interval = time_end - time_start;
                
                //M_DEBUG("speed checking 0 >> boot " << time_boot_interval << " = " << time_boot_end << " - " << _time_boot_begin);
                
                //M_DEBUG("speed checking 0 >> time " << time_interval << " = " << time_end << " - " << time_start);
                
                //M_DEBUG("speed checking 0 >> time(origin) end[ " << _time_end.tv_sec << ", " << _time_end.tv_usec  << " ], [ " << _time_begin.tv_sec << ", " << _time_begin.tv_usec << " ]");
                
                //M_DEBUG("speed checking 1 >> " << time_interval << ", " << time_boot_interval);
                
                if (time_boot_interval >= time_interval)
                {
                    //M_DEBUG("speed checking 1 >> ok");
                    _last_check_result = Result::OK;
                    nextLoopCheck();
                    break;
                }
                
                auto time_offset = time_interval - time_boot_interval;
                
                //M_DEBUG("speed checking 2 >> " << time_offset << ", " << _ignore_delta);
                
                if (time_offset <= _ignore_delta)
                {
                    //M_DEBUG("speed checking 2 >> ok");
                    _last_check_result = Result::OK;
                    nextLoopCheck();
                    break;
                }
                
                
                if (_last_check_result == Result::FIRST || _last_check_result == Result::BINGO)
                {
                    _continuous_count++;
                    
                    if (isContinuousCountEnough())
                    {
                        dispatchEvent(MSpeedCheck::EVENT_SPEED_CHECK_BINGO);
                        _state = State::IDLE;
                    }
                    else
                    {
                        nextLoopCheck();
                    }
                }
                else
                {
                    _continuous_count = 0;
                    _state = State::START;
                }
                
                _last_check_result = Result::BINGO;
                
                
                M_ERROR("speed checking >> ! bingo ! >> " << time_interval << ", " << time_boot_interval << ", " << time_offset << ", " << _ignore_delta << ", " << _continuous_count << ", " << _bingo_continuous_count);
            }
        }break;
            
        default: break;
    }
}


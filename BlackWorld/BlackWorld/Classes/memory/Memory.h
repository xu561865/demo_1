#ifndef MEMORY_H
#define MEMORY_H

#include <map>
#include <vector>

#include "mlib.h"
#include "cocos2d.h"
#include "mlib_ccext.h"
#include "ModelServer.h"


MLIB_DEFINE_EVENT(EVENT_IAP_COMPLETED);
MLIB_DEFINE_EVENT(EVENT_NETWORK_ERROR);
MLIB_DEFINE_EVENT(EVENT_TAB_HIDDEN);
MLIB_DEFINE_EVENT(EVENT_NEWBIE_SET_NAME_END)
MLIB_DEFINE_EVENT(EVENT_STAGE_VIEW_WILL_HIDE);

class Memory : public mlib::MEventDispatcher
{
public:
    static std::vector<std::string> g_tips;

public:
    static Memory * SharedInstance();
    Memory();
    ~Memory();
    
    void saveBasic();
    bool loadBasic();
    
    void save();
    bool load();
    
    bool clear();
    
    void reset();
    
    ModelServer * getCurServer();
    std::string getCurAPI();
    
    time_t serverTime();
    void updateServerTime(time_t serverTime);
    
    int32_t getTimeZoneOffsetBetweenServerAndClient(); // in seconds
    
public:
    MLIB_DECLARE_PROPERTY(std::string, userId);
    MLIB_DECLARE_PROPERTY(std::string, userPwd);
    MLIB_DECLARE_PROPERTY(int, registerState);
    
    MLIB_DECLARE_PROPERTY(unsigned int, level);
    MLIB_DECLARE_PROPERTY(unsigned int, money);
    MLIB_DECLARE_PROPERTY(int, loginState);
    
};

#define MEM_INFO (Memory::SharedInstance())

#endif // MEMORY_H

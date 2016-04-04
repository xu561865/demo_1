#ifndef MEMORY_H
#define MEMORY_H

#include <map>
#include <vector>

#include "mlib.h"
#include "cocos2d.h"
#include "mlib_ccext.h"
#include "ModuleServer.h"


MLIB_DEFINE_EVENT(EVENT_IAP_COMPLETED);
MLIB_DEFINE_EVENT(EVENT_NETWORK_ERROR);
MLIB_DEFINE_EVENT(EVENT_TAB_HIDDEN);
MLIB_DEFINE_EVENT(EVENT_NEWBIE_SET_NAME_END)
MLIB_DEFINE_EVENT(EVENT_STAGE_VIEW_WILL_HIDE);

class LMemory : public mlib::MEventDispatcher
{
public:
    static std::vector<std::string> g_tips;

public:
    static LMemory * SharedInstance();
    LMemory();
    ~LMemory();
    
    void saveBasic();
    bool loadBasic();
    
    void save();
    bool load();
    
    bool clear();
    
    void reset();
    
    ModuleServer * getCurServer();
    std::string getCurAPI();
    
    time_t serverTime();
    void updateServerTime(time_t serverTime);
    
    int32_t getTimeZoneOffsetBetweenServerAndClient(); // in seconds
    
};

#define MEM_INFO (Memory::SharedInstance())

#endif // MEMORY_H

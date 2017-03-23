#ifndef _MODEL_SERVER_H_
#define _MODEL_SERVER_H_

#include <iostream>
#include "cocos2d.h"
#include "mlib_ccext.h"

USING_NS_CC;

typedef enum
{
    kServerStatus_Normal = 0,
    kServerStatus_Busy = 1,
}ServerStatus_t;


class ModelServer : public CCObject
{
public:
    MLIB_CREATE_CCOBJECT(ModelServer);
    
    static ModelServer * serverInfoFromDict(CCDictionary *dict);
    
    std::string api();
    std::string configUrl();
    bool isBusy();
    
    MLIB_DECLARE_PROPERTY(uint32_t, serverId);
    MLIB_DECLARE_PROPERTY(std::string, serverName);
    MLIB_DECLARE_PROPERTY(std::string, url);
    MLIB_DECLARE_PROPERTY(uint32_t, serverStatus);
};

#endif /* defined(_MODEL_SERVER_H_) */

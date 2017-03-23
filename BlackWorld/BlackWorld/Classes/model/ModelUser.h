#ifndef _MODEL_USER_H_
#define _MODEL_USER_H_

#include <iostream>
#include "cocos2d.h"
#include "mlib_ccext.h"
#include "json.h"



class ModelUser : public cocos2d::CCObject
{
public:
    MLIB_CREATE_CCOBJECT(ModelUser);
    
    ModelUser();
    virtual ~ModelUser();
    
    static ModelUser * createWithJson(Json::Value json);
    void parseJson(Json::Value json);
    
    
    MLIB_DECLARE_PROPERTY(uint32_t, userId);
    MLIB_DECLARE_PROPERTY(uint32_t, vipTime);
    MLIB_DECLARE_PROPERTY(std::string, nickname);
    MLIB_DECLARE_PROPERTY(uint32_t, level);
    MLIB_DECLARE_PROPERTY(uint32_t, exp);
    MLIB_DECLARE_PROPERTY(int32_t, coin);
    MLIB_DECLARE_PROPERTY(int32_t, cash);
    
};

#endif /* defined(_MODEL_USER_H_) */

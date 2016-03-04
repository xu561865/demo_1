//
//  OASSdk.cpp
//  Slots
//
//  Created by bbf on 15-1-8.
//
//

#include "OASSdk.h"
#include <OasgamesSDK/OasgamesSDK.h>
#import "Sdk.h"


NS_CC_LEVER_EXT_BEGIN;


static OASSdk* s_sharedOASSdkInstance = NULL;

OASSdk::OASSdk()
{
    
}

OASSdk::~OASSdk()
{
    
}

OASSdk* OASSdk::getInstance()
{
    if (s_sharedOASSdkInstance == NULL)
    {
        s_sharedOASSdkInstance = new OASSdk();
    }
    
    return s_sharedOASSdkInstance;
}

void OASSdk::autoLogin()
{
    [[Sdk getInstance] login: nil resultCallback:^(BOOL success,  NSDictionary *info, NSString *message){
    
        OASUserInfo *userInfo = [info objectForKey:@"userInfo"];
        //OASUserInfo *userInfo = (OASUserInfo *)info;

        NSLog(@"userInfo -> userID = %@ token = %@ type = %@ platform = %@ platformToken = %@",userInfo.userID,userInfo.token,userInfo.type,userInfo.platform,userInfo.platformToken);
        

        NSString *t = [[NSString alloc]initWithFormat:@"%d", userInfo.type.intValue];
        LuaValueDict dict;

        std::string userID = [userInfo.userID UTF8String];                  //new std::string([userInfo.userID UTF8String]);
        std::string token = [userInfo.token UTF8String];                    //new std::string([userInfo.token UTF8String]);
        std::string type = [t UTF8String];                                  //new std::string([t UTF8String]);
        std::string platform = [userInfo.platform UTF8String];              //new std::string([userInfo.platform UTF8String]);
        std::string platformToken = [userInfo.platformToken UTF8String];    //new std::string([userInfo.platformToken UTF8String]);
        
        
        dict.insert(dict.end(), LuaValueDict::value_type("userID", LuaValue::stringValue(userID)));
        dict.insert(dict.end(), LuaValueDict::value_type("token", LuaValue::stringValue(token)));
        dict.insert(dict.end(), LuaValueDict::value_type("type", LuaValue::stringValue(type)));
        dict.insert(dict.end(), LuaValueDict::value_type("platform", LuaValue::stringValue(platform)));
        dict.insert(dict.end(), LuaValueDict::value_type("platformToken", LuaValue::stringValue(platformToken)));

        OASSdk::getInstance()->sendEvent(OasHandlerType::LOGIN, dict);
        
        [t release];
        
    }];
}

void OASSdk::share(cocos2d::ValueMap params) 
{
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    for (auto iter = params.cbegin(); iter != params.cend(); ++iter)
    {
        NSString *key = [NSString stringWithUTF8String:iter->first.c_str()];
        NSString *value = [NSString stringWithUTF8String:iter->second.asString().c_str()];
        [dict setObject:value forKey:key];
    }
    
    [[Sdk getInstance] share:dict resultCallback:^(BOOL success, NSDictionary *userInfo, NSString *message){
        
        NSLog(@"share");
        LuaValueDict dict;
        
        if (success) {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(1)));
        } else {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(0)));
            dict.insert(dict.end(), LuaValueDict::value_type("erromsg", LuaValue::stringValue([message UTF8String])));
        }
            
        OASSdk::getInstance()->sendEvent(OasHandlerType::SHARE, dict);
    }];
    
}
void OASSdk::showMenu(bool isShow, int postion)
{
    [[Sdk getInstance] showMenu:isShow positon: MenuPosition(postion)];
}


void OASSdk::pay(const char* identifier, double amountInCents, const char* serverID, const char* roleID, const char* extend)
{
    NSString *idt = [NSString stringWithUTF8String:identifier];
    NSString *sid = [NSString stringWithUTF8String:serverID];
    NSString *rid = [NSString stringWithUTF8String:roleID];
    NSString *et = [NSString stringWithUTF8String:extend];
    
    [[Sdk getInstance] pay:idt amount:amountInCents serverID:sid roleID:rid extend:et resultCallback:^(BOOL success, NSDictionary *userInfo, NSString *message){
        
        LuaValueDict dict;
        if (success) {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(1)));
        } else {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(0)));
            dict.insert(dict.end(), LuaValueDict::value_type("erromsg", LuaValue::stringValue([message UTF8String])));
        }
        
        OASSdk::getInstance()->sendEvent(OasHandlerType::PAY, dict);
    }];
}

void OASSdk::setUserInfo(const char* roleID, const char* serverID, const char* serverType, const char* serverName, const char* username)
{
    NSString *sid = [NSString stringWithUTF8String:serverID];
    NSString *sname = [NSString stringWithUTF8String:serverName];
    NSString *uname = [NSString stringWithUTF8String:username];
    NSString *rid = [NSString stringWithUTF8String:roleID];
    NSString *st = [NSString stringWithUTF8String:serverType];
    
    [[OASPlatform sharedInstance] setUserInfoWithServerID:sid serverName:sname serverType:st username:uname roleID:rid];
    
}

void OASSdk::switchUser()
{
    [[OASPlatform sharedInstance] switchUser];
}

void OASSdk::cleanGameInfo()
{
    [[OASPlatform sharedInstance] cleanGameInfo];
}

void OASSdk::trackEvent(const char* eventToken, cocos2d::ValueMap params)
{
    NSString *et = [NSString stringWithUTF8String:eventToken];
    
    NSMutableDictionary *dict = [[NSMutableDictionary alloc]initWithCapacity:20];
    for (auto iter = params.cbegin(); iter != params.cend(); ++iter)
    {
        NSString *key = [NSString stringWithUTF8String:iter->first.c_str()];
        NSString *value = [NSString stringWithUTF8String:iter->second.asString().c_str()];
        [dict setObject:value forKey:key];
    }
    
    [OASPlatform trackEvent:et withParameters:dict];
    [dict release];
} 

void OASSdk::trackForAdv(double amountInCents, const char* eventToken, cocos2d::ValueMap params)
{
    NSString *et = [NSString stringWithUTF8String:eventToken];
    
    NSMutableDictionary *dict = [[NSMutableDictionary alloc]initWithCapacity:20];
    for (auto iter = params.cbegin(); iter != params.cend(); ++iter)
    {
        NSString *key = [NSString stringWithUTF8String:iter->first.c_str()];
        NSString *value = [NSString stringWithUTF8String:iter->second.asString().c_str()];
        [dict setObject:value forKey:key];
    }
    
    [OASPlatform trackRevenue:amountInCents forEvent:et withParameters:dict];
    [dict release];
}

void OASSdk::trackForPay(double amountInCents, const char* transactionId, const char* eventToken, cocos2d::ValueMap params)
{
    NSString *ti = [NSString stringWithUTF8String:transactionId];
    NSString *et = [NSString stringWithUTF8String:eventToken];
    
    NSMutableDictionary *dict = [[NSMutableDictionary alloc]initWithCapacity:20];
    for (auto iter = params.cbegin(); iter != params.cend(); ++iter)
    {
        NSString *key = [NSString stringWithUTF8String:iter->first.c_str()];
        NSString *value = [NSString stringWithUTF8String:iter->second.asString().c_str()];
        [dict setObject:value forKey:key];
    }
    
    [OASPlatform trackRevenue:amountInCents transactionId:ti forEvent:et withParameters:dict];
    [dict release];
}

void OASSdk::registerHandler(OasHandlerType type, int handle)
{
    _mapHandlers[type] = handle;
    
}

void OASSdk::unregisterHandler(OasHandlerType type)
{
    auto iter = _mapHandlers.find(type);
    if (_mapHandlers.end() != iter) {
        _mapHandlers.erase(type);
    }
}

void OASSdk::sendEvent(OasHandlerType type, LuaValueDict info)
{
    
    auto iter = _mapHandlers.find(type);
    if (_mapHandlers.end() != iter)
    {
        LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
        pStack->pushLuaValueDict(info);
        pStack->executeFunctionByHandler(iter->second, 1);
        pStack->clean();
    }
}


// 新增
void OASSdk::postStatusUpdateWithShareDialogComplete()
{
    [[Sdk getInstance]postStatusUpdateWithShareDialogComplete:^(BOOL status, NSString *error) {
        LuaValueDict dict;
        
        if (status) {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(1)));
        } else {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(0)));
            dict.insert(dict.end(), LuaValueDict::value_type("errormsg", LuaValue::stringValue([error UTF8String])));
        }
        OASSdk::getInstance()->sendEvent(OasHandlerType::POSTSTATUS, dict);
        
    }];
}
     
void OASSdk::shareNewFeedWithMessaga(const char* messga)
{
    NSString *m = [NSString stringWithUTF8String:messga];
    [[Sdk getInstance]shareNewFeedWithMessaga:m complete:^(BOOL status, NSString *error) {
        
        LuaValueDict dict;
        
        if (status) {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(1)));
        } else {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(0)));
             dict.insert(dict.end(), LuaValueDict::value_type("errormsg", LuaValue::stringValue([error UTF8String])));
        }
        OASSdk::getInstance()->sendEvent(OasHandlerType::NEWFEED, dict);
     }];
    
}

void OASSdk::shareWithImage(const char* path)
{
    NSString *p = [NSString stringWithUTF8String:path];
    [[Sdk getInstance] shareWithImage:p complete:^(BOOL status, NSString *error) {
        
        LuaValueDict dict;
        
        if (status) {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(1)));
        } else {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(0)));
            dict.insert(dict.end(), LuaValueDict::value_type("errormsg", LuaValue::stringValue([error UTF8String])));
        }
        
        OASSdk::getInstance()->sendEvent(OasHandlerType::SHAREIMAGE, dict);
    }];
}

void OASSdk::getFrindsWithLimit(int limit, bool isNext, bool frombegin)
{

    [[Sdk getInstance] getFrindsWithLimit:limit isNext:isNext fromBegin:frombegin complete:^(BOOL status, id result, NSString * error) {
        
        NSDictionary * r = (NSDictionary *) result;
        
        LuaValueDict info;
        for (NSString *key in r) {
            info.insert(info.end(), LuaValueDict::value_type([key UTF8String], LuaValue::stringValue([r[key] UTF8String])));
        }
        
        LuaValueDict dict;
        if (status) {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(1)));
            dict.insert(dict.end(), LuaValueDict::value_type("data", LuaValue::dictValue(info)));
        } else {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(0)));
            dict.insert(dict.end(), LuaValueDict::value_type("errormsg", LuaValue::stringValue([error UTF8String])));
        }
        OASSdk::getInstance()->sendEvent(OasHandlerType::GETFRIENDS, dict);
    }];
}
void OASSdk::getInvitedFriendsWithLimit(int limit, bool isNext, bool frombegin)
{
    [[Sdk getInstance] getInvitedFriendsWithLimit:limit isNext:isNext fromBegin:frombegin complete:^(BOOL status, id result, NSString * error) {
        
        NSDictionary * r = (NSDictionary *) result;
        
        LuaValueDict info;
        for (NSString *key in r) {
            info.insert(info.end(), LuaValueDict::value_type([key UTF8String], LuaValue::stringValue([r[key] UTF8String])));
        }
        
        LuaValueDict dict;
        if (status) {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(1)));
            dict.insert(dict.end(), LuaValueDict::value_type("data", LuaValue::dictValue(info)));
        } else {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(0)));
            dict.insert(dict.end(), LuaValueDict::value_type("errormsg", LuaValue::stringValue([error UTF8String])));
        }
        OASSdk::getInstance()->sendEvent(OasHandlerType::GETINVITEDFRIENDS, dict);
    }];
}
void OASSdk::setAppRequestWithActiontype(const char* actiontype, const char* uids, const char* message, const char* objectid)
{
    NSString* at = [NSString stringWithUTF8String:actiontype];
    NSString* ot;
    if (objectid) {
        ot = [NSString stringWithUTF8String:objectid];
    } else {
        ot = nil;
    }
    NSString* ut = [NSString stringWithUTF8String:uids];
    NSString* m = [NSString stringWithUTF8String:message];
    
    [[Sdk getInstance] setAppRequestWithActiontype:at objectid:ot uids:ut message:m complete:^(BOOL status, id result, NSString * error) {
        
        NSDictionary * r = (NSDictionary *) result;
        
        LuaValueDict info;
        for (NSString *key in r) {
            info.insert(info.end(), LuaValueDict::value_type([key UTF8String], LuaValue::stringValue([r[key] UTF8String])));
        }
        
        LuaValueDict dict;
        if (status) {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(1)));
            dict.insert(dict.end(), LuaValueDict::value_type("data", LuaValue::dictValue(info)));
        } else {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(0)));
            dict.insert(dict.end(), LuaValueDict::value_type("errormsg", LuaValue::stringValue([error UTF8String])));
        }
        OASSdk::getInstance()->sendEvent(OasHandlerType::APPREQUESTS, dict);
    }];
}
NS_CC_LEVER_EXT_END;
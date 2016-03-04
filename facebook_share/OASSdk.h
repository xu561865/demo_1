//
//  OASSdk
//  Slots
//
//  Created by bbf on 15-1-7.
//
//

#ifndef Slots_OASSdk_h
#define Slots_OASSdk_h

#include "cocos2d.h"
#include "CCLuaStack.h"
#include "CCLuaEngine.h"
#include "../LeverExtensionMacros.h"
#include <iostream>

NS_CC_LEVER_EXT_BEGIN

class OASSdk : public Ref
{

public:
    
    OASSdk();
    
    ~OASSdk();
    
public:
    
    enum class OasHandlerType
    {
        LOGIN,
        SHARE,
        PAY,
        BIN,
        REGISTANDLOGIN,
        POSTSTATUS,
        NEWFEED,
        SHAREIMAGE,
        GETFRIENDS,
        GETINVITEDFRIENDS,
        APPREQUESTS
    };

    static OASSdk* getInstance();
    void autoLogin();
    void share(cocos2d::ValueMap params);
    void showMenu(bool isShow, int position);
    void pay(const char* identifier, double amountInCents, const char* serverID, const char* roleID, const char* extend);
    void setUserInfo(const char* roleID, const char* serverID = "", const char* serverType = "", const char* serverName = "", const char* username = "");
    void switchUser();
    void cleanGameInfo();
    
    void postStatusUpdateWithShareDialogComplete();
    void shareNewFeedWithMessaga(const char* messga);
    void shareWithImage(const char* path);
    
    void getFrindsWithLimit(int limit, bool isNext, bool frombegin);
    void getInvitedFriendsWithLimit(int limit, bool isNext, bool frombegin);
    void setAppRequestWithActiontype(const char* actiontype, const char* uids, const char* message, const char* objectid = nullptr);
    
    void trackEvent(const char* eventToken, cocos2d::ValueMap params = cocos2d::ValueMapNull);
    void trackForAdv(double amountInCents, const char* eventToken, cocos2d::ValueMap params = cocos2d::ValueMapNull);
    void trackForPay(double amountInCents, const char* transactionId, const char* eventToken, cocos2d::ValueMap params = cocos2d::ValueMapNull);
    
    void registerHandler(OasHandlerType type, int handle);
    void unregisterHandler(OasHandlerType type);
    
    void sendEvent(OasHandlerType type, LuaValueDict info);
    
    typedef int Handler;
    typedef std::map<OasHandlerType, Handler> MapHandlers;
    
private:
    
    MapHandlers _mapHandlers;
};

NS_CC_LEVER_EXT_END;


#endif

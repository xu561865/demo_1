#ifndef Client_LayerLogin_h
#define Client_LayerLogin_h

#include"cocos2d.h"
#include"cocos-ext.h"
#include "SocketManager.h"
#include "CData.h"
#include "LodingLayer.h"
#include "TeXiao.h"

using namespace cocos2d;

class LayerLogin
: public CCLayer
, public extension::CCBSelectorResolver
, public extension::CCBMemberVariableAssigner
, public extension::CCNodeLoaderListener
{
public:
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, extension::CCBValue* pCCBValue);
    virtual void onNodeLoaded(CCNode * pNode, extension::CCNodeLoader * pNodeLoader);
    
    LayerLogin();
    virtual ~LayerLogin();
    CREATE_FUNC(LayerLogin);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    static CCScene* scene();
    
    CCMenuItemImage* pMenuItemLogin;
    CCMenuItemImage* pMenuItemStart;
    CCSprite * texiao;
    bool hasRole;
    Loading * load;
    CCSprite * logo;
    CCSize ms_winSize;
    char * denglu1;
    char * sendData;
    
    void receiveLoginData();
    void sendPersonalData();
    void receivePersonalData();
    void login();
    
    CCSprite *pSpriteDialogLogin;
private:
    void menuItemCallbackLogin(CCObject* pSender);
    void menuItemCallbackStart(CCObject* pSender);
    
    
    extension::CCEditBox* editBoxUsername;
    extension::CCEditBox* editBoxPassword;
};

class LayerLoginLoader : public extension::CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(LayerLoginLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(LayerLogin);
};

#endif
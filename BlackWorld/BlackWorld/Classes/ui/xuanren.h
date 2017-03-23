#ifndef __client1__xuanren__
#define __client1__xuanren__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "json.h"
#include "LodingLayer.h"


USING_NS_CC;
USING_NS_CC_EXT;

class SelectRole
: public CCLayer
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCNodeLoaderListener
{
public:
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, extension::CCBValue* pCCBValue);
    virtual void onNodeLoaded(CCNode * pNode, extension::CCNodeLoader * pNodeLoader);

    ~SelectRole();
    CREATE_FUNC(SelectRole);
    virtual bool init();
    virtual void onExit();
    
    static CCScene* getScene();
    
    int flag;
    Loading * load;
    CCLabelTTF * jieshao;
    CCSprite *renwu;
    CCMenuItemImage *btn1;
    CCMenuItemImage *btn2;
    CCMenuItemImage *btn3;
    CCMenuItemImage *selectBtn;
    CCNode* pPopNode;
    extension::CCEditBox * enterUsrName;
    
    void EnterName(CCObject* pSender);
    void FirstPerson(CCObject* pSender);
    void SecondPerson(CCObject* pSender);
    void ThridPerson(CCObject* pSender);
    void replacScene(CCObject* pSender);
    void receiveRoleInfo();
    void receiveHomeInfo();
};

class SelectRoleLoader : public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SelectRoleLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SelectRole);
};

#endif /* defined(__client1__xuanren__) */
